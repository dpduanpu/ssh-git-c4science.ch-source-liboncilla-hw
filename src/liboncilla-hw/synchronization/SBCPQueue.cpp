/***
 * \file SBCPQueue.cpp
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#include "SBCPQueue.h"

#include <glog/logging.h>

#include <libsbcp/utils/Config.h>

#include <liboncilla-hw/config/Config.h>

namespace liboncilla {
namespace hw {

SBCPQueue::SBCPQueue(const Config & config) 
	: Queue(config.Main().SBCPPriority(), true)
	, d_bus() 
	, d_errorThreshold(config.Motors().CommunicationErrorThreshold()) {
	sbcp::Config sbcpConfig;
	sbcpConfig.LoadAllFiles();

	// create links to the 4 motorboards
	unsigned int requiredFrameSize = 4;   // 4 consecutive packets send at oncefrom roboard to master communication board
	unsigned int requiredPacketSize = 32; // Size of the biggest packet you will send, the actual maximum might be higher

	d_bus = sbcpConfig.OpenDefaultBusWithFrame(requiredFrameSize,
	                                           requiredPacketSize);

	if (!d_bus) {
		throw std::runtime_error("SBCPQueue::SBCPQueue() Can't load bus due to missing configuration.");
	} else {
		DLOG(INFO) << "Bus Workflow: " << d_bus->CurrentWorkflow();
	}

	const MotorDriverGroup & devices = config.Motors().Devices();
	const BrushlessParameterGroup & motorConfig = config.Motors().Params();
	int16_t timestep = config.Main().Timestep();

	d_motordrivers[rci::oncilla::LEFT_FORE] = 
		OpenAndConfigureMotorDriver(devices.LeftFore(), motorConfig, timestep);
	d_motordrivers[rci::oncilla::RIGHT_FORE] = 
		OpenAndConfigureMotorDriver(devices.RightFore(), motorConfig, timestep);
	d_motordrivers[rci::oncilla::LEFT_HIND] = 
		OpenAndConfigureMotorDriver(devices.LeftHind(), motorConfig, timestep);
	d_motordrivers[rci::oncilla::RIGHT_HIND] = 
		OpenAndConfigureMotorDriver(devices.RightHind(), motorConfig, timestep);

}

SBCPQueue::~SBCPQueue() {
}

void SBCPQueue::DownstreamData() {
	for (MotorAndEncoderByL1L2::const_iterator motAndEnc = d_motAndEncByL1L2.begin(); 
	     motAndEnc != d_motAndEncByL1L2.end();
	     ++motAndEnc) {
		motAndEnc->second.motor->GoalPosition().Set(motAndEnc->first->nodeToQueueJointPosition());
	}
}

void SBCPQueue::UpstreamData() {
	//keep this all small as possible KISS -> Fast
	for (MotorAndEncoderByL1L2::const_iterator motAndEnc = d_motAndEncByL1L2.begin(); 
	     motAndEnc != d_motAndEncByL1L2.end();
	     ++motAndEnc) {
		uint16_t pos(motAndEnc->second.encoder->PositionAndStatus().Get() & 0x3fff);
		uint16_t status((motAndEnc->second.encoder->PositionAndStatus().Get() & 0xc000) >> 14);
		int16_t motPos(motAndEnc->second.motor->PresentPosition().Get());
		motAndEnc->first->queueToNodeJointPosition(pos, status, motPos);
	}

	for (MotorDriverByL0::const_iterator mdv = d_mdvByL0.begin();
	     mdv != d_mdvByL0.end();
	     ++mdv) {
			mdv->first->queueToNodeForces(mdv->second->Force(0).Get(),
			                              mdv->second->Force(1).Get(), 
			                              mdv->second->Force(2).Get());
	}

	for (MagneticEncoderByL3::const_iterator enc = d_encByL3.begin();
	     enc != d_encByL3.end(); 
	     ++enc) {
			enc->first->queueToNodeJointPosition(enc->second->PositionAndStatus().Get() & 0x3fff,
			                                     (enc->second->PositionAndStatus().Get() & 0xc000) >> 14);
	}
}

void SBCPQueue::PerformIO() {
  sbcp::ScheduledWorkflow & w = this->d_bus->Scheduled();
  static int nbErrors(0);
  // Maybe we could catch here communictaion errors, i.e. sbcp::MultipleTransferError
  try {
    w.StartTransfers();
    w.WaitForTransfersCompletion();
  } catch( const sbcp::MultipleTransferError & e) {
    nbErrors += e.Errors().size();
    if( d_errorThreshold >= 0 && d_errorThreshold <= nbErrors ) {
      throw;
    }
    std::cerr << "SBCP Error ("<< nbErrors << "/" << d_errorThreshold << ") : " <<e.what() << std::endl;
  }
}



void SBCPQueue::InitializeIO() {
	//first make a sanity check to avoid problem in
	//Upstream/DownstreamData. It will throw exception here instead of
	//issuing hard to debug segfault form the Callback
	for(MotorAndEncoderByL1L2::const_iterator motAndEnc = d_motAndEncByL1L2.begin();
	    motAndEnc != d_motAndEncByL1L2.end();
	    ++motAndEnc) {
		if( motAndEnc->second.motor && motAndEnc->second.encoder ) {
			continue;
		}
		throw std::logic_error("Internal error: SBCPQueue::d_motAndEncByL1L2 is not sane!");
	}

	for(MotorDriverByL0::const_iterator mdv = d_mdvByL0.begin();
	    mdv != d_mdvByL0.end();
	    ++mdv) {
		if( !mdv->second) {
			throw std::logic_error("Internal error: SBCPQueue::d_mdvByL0 is not sane!");
		}
	}

	for(MagneticEncoderByL3::const_iterator enc = d_encByL3.begin();
	    enc != d_encByL3.end(); 
	    ++enc) {
		if (!enc->second) {
			throw std::logic_error("Internal error: SBCPQueue::d_encByL3 is not sane!");
		}
	}
	//Upstream/DownstreamData should not issue segfault for any reason now !	


	// Switch on the motors
	for (MotordriverByLeg::iterator mdv = d_motordrivers.begin();
	     mdv != d_motordrivers.end(); 
	     ++mdv) {
		DLOG(INFO) << "Setting " << LegPrefix(mdv->first) << " motors on.";
		mdv->second->Motor1().MotorControlMode().Set(sbcp::amarsi::MotorDriver::Motor::SMOOTH_POSITION);
		mdv->second->Motor2().MotorControlMode().Set(sbcp::amarsi::MotorDriver::Motor::SMOOTH_POSITION);
	}
	

	// Enable scheduled workflow
	sbcp::ScheduledWorkflow & w = this->d_bus->Scheduled();

	// Append devices to transfer
	for (MotordriverByLeg::iterator mdv = d_motordrivers.begin();
	     mdv != d_motordrivers.end(); 
	     ++mdv) {
		if (!mdv->second) {
			DLOG(INFO) << "SBCPQueue::InitializeIO: Ignoring motor driver "
			           << LegPrefix(mdv->first);
			continue;
		}

		DLOG(INFO) <<  "Appending scheduled device " << LegPrefix(mdv->first);
		w.AppendScheduledDevice(std::static_pointer_cast<sbcp::Device>(mdv->second));
	}

	DLOG(INFO) << "SBCP Scheduled Workflow enabled and configured.";
}

void SBCPQueue::DeinitializeIO() {
	// Disable scheduled woirkflow. It will WaitForTransferCompletion.
	d_bus->Lazy();
	DLOG(INFO) << "SBCP Scheduled Workflow disabled.";
	// disable motor smooth position control. Anyway the board
	// firmware will do it in 10 timestep.
	for (MotordriverByLeg::iterator mdv = d_motordrivers.begin();
	     mdv != d_motordrivers.end(); 
	     ++mdv) {
		DLOG(INFO) << "Setting " << LegPrefix(mdv->first) << " motors off."; 
		mdv->second->Motor1().MotorControlMode().Set(sbcp::amarsi::MotorDriver::Motor::COAST);
		mdv->second->Motor2().MotorControlMode().Set(sbcp::amarsi::MotorDriver::Motor::COAST);
	}
	
}

void SBCPQueue::RegisterL0(rci::oncilla::Leg l, const L0::Ptr & node) {
	// check if this map contains this leg
	MotordriverByLeg::const_iterator fi = d_motordrivers.find(l);
	if (fi == d_motordrivers.end() || !fi->second ) {
		throw(std::runtime_error("Failed to open motordriver, upon initialization of this Queue, for motordriver "
		                         + LegPrefix(l)));
	}

	d_mdvByL0[node] = fi->second;
}

void SBCPQueue::RegisterL1(rci::oncilla::Leg l, const L1L2::Ptr & node) {
	bool isLeftLeg = (l == rci::oncilla::LEFT_FORE) || (l == rci::oncilla::LEFT_HIND);
	bool isHip = true;

	//check if this map contains this leg
	MotordriverByLeg::const_iterator fi = d_motordrivers.find(l);
	if (fi == d_motordrivers.end() || !fi->second) {
		throw(std::runtime_error("Failed to open motordriver earlier, upon initialization of this Queue, for motordriver "
		                         + LegPrefix(l)));
	}

	node->initialize(isLeftLeg, isHip,
	                 fi->second->Motor1().PositionLimit().Get() & 0x7fff,
	                 fi->second->Motor1().PresentPosition().Get());
	
	/// \todo hardcoded mapping, could be configurable
	MotorAndEncoder m1;
	m1.motor = &(fi->second->Motor1());
	m1.encoder = &(fi->second->Q1());

	d_motAndEncByL1L2[node] = m1;
}

void SBCPQueue::RegisterL2(rci::oncilla::Leg l, const L1L2::Ptr & node) {
	bool isRightLeg = (l == rci::oncilla::RIGHT_FORE) || (l == rci::oncilla::RIGHT_HIND);
	bool isHip = false;

	//check if this map contains this leg
	MotordriverByLeg::const_iterator fi = d_motordrivers.find(l);
	if (fi == d_motordrivers.end() || !fi->second) {
		throw(std::runtime_error("Failed to open motordriver earlier, upon initialization of this Queue, for motordriver "
		                         + LegPrefix(l)));
	}


	node->initialize(isRightLeg, 
	                 isHip,
	                 fi->second->Motor2().PositionLimit().Get() & 0x7fff,
	                 fi->second->Motor2().PresentPosition().Get());

	/// \todo hardcoded mapping, could be configurable
	MotorAndEncoder m2;
	m2.motor = &(fi->second->Motor2());
	m2.encoder = &(fi->second->Q2());

	d_motAndEncByL1L2[node] = m2;
}

void SBCPQueue::RegisterL3(rci::oncilla::Leg l, const L3::Ptr & node) {
	//check if this map contains this leg
	MotordriverByLeg::const_iterator fi = d_motordrivers.find(l);
	if (fi == d_motordrivers.end() || !fi->second) {
		throw(std::runtime_error("Failed to open motordriver, upon initialization of this Queue, for motordriver "
		                         + LegPrefix(l)));
	}

	d_encByL3[node] = &(fi->second->Q3());
}

bool AllMotorsCalibrated(const sbcp::amarsi::MotorDriver::Ptr & mdv) {
	return mdv->Motor1().RuntimeCalibrated() && mdv->Motor2().RuntimeCalibrated();
 }

void SBCPQueue::CalibrateMotorDrivers() {
	typedef std::set< sbcp::amarsi::MotorDriver::Ptr> SetOfMDV;
	
	SetOfMDV uncalibrated;
	for(MotordriverByLeg::const_iterator mdv = d_motordrivers.begin();
	    mdv != d_motordrivers.end();
	    ++mdv) {
		if (!mdv->second) {
			continue;
		}
		if (AllMotorsCalibrated(mdv->second)) {
			DLOG(INFO) << LegPrefix(mdv->first) << " motors are already runtime calibrated";
		} else {
			DLOG(INFO) << "Calibrating " << LegPrefix(mdv->first) << " motors";
			mdv->second->CalibrateMotors();
			uncalibrated.insert(mdv->second);
		}
	}


	while(!uncalibrated.empty()) {
		usleep(1e5);
		SetOfMDV toTest = uncalibrated;
		for(SetOfMDV::const_iterator mdv = toTest.begin();
		    mdv != toTest.end();
		    ++mdv) {
			if(AllMotorsCalibrated(*mdv)) {
				DLOG(INFO) << "All motor of board " << (int)(*mdv)->ID() << " calibrated";
				uncalibrated.erase(*mdv);
			}
		}

	}

	DLOG(INFO) << "All motors calibrated";

}

sbcp::amarsi::MotorDriver::Ptr 
SBCPQueue::OpenAndConfigureMotorDriver(const MotorDriverSection & def, 
                                       const BrushlessParameterGroup & params,
                                       int16_t expectedTsInMs) {
	DLOG(INFO) << "SBCPQueue::OpenAndConfigureMotorDriver()";
	

	if (!d_bus) {
		throw std::logic_error("SBCPQueue::d_bus private member should be initialized in"
		                       "constructor. Please report this internal bug");
	}

    this->d_bus->Lazy();

	sbcp::amarsi::MotorDriver::Ptr res = d_bus->OpenDevice< sbcp::amarsi::MotorDriver > (def.BoardID());

	if (!res) {
		// board is just not here, buit we will throw later if it is required.
		DLOG(INFO) << "Could not find an AMARSi motordriver board with ID " << (int)def.BoardID() << "on the bus";
		return res;
	}


	SetMotorParameters(params, def.M1Params(), expectedTsInMs, res->Motor1());
	SetMotorParameters(params, def.M2Params(), expectedTsInMs, res->Motor2());

	return res;
}

void SBCPQueue::SetMotorParameters(const BrushlessParameterGroup & paramGroup,
                                   const std::string & paramName, int16_t expectedTsInMs,
                                   sbcp::amarsi::MotorDriver::Motor & motor) {
	DLOG(INFO) << "SBCPQueue::SetMotorParameters()";
	//due to a bad design of biorob-cpp dynamic section we should do this
	std::shared_ptr<BrushlessParameterSection> params =
		const_cast<BrushlessParameterGroup &>(paramGroup).SubSection(paramName);
	
	if (!params) {
		throw std::runtime_error("Unknown motor parameter group '" + paramName + "'");
	}
	
	// Do no set the Motor::MotorControlMode(). Indeed it could fails
	// if the motor are not calibrated. Furthermore, when entering
	// smooth position, the board is waiting for realtime periodic
	// command. So we should set this mode as close as possible of the
	// ScheduledWorkflow switch (i.e. InitializeIO() ).

	motor.PGain().Set(params->PGain());
	motor.IGain().Set(params->IGain());
	motor.DGain().Set(params->DGain());

	motor.Stiffness().Set(params->Stiffness());
	motor.Damping().Set(params->Damping());
	motor.Preload().Set(params->PreCompression());

	motor.MaxTorque().Set(params->MaxTorque());
	motor.MaxSpeed().Set(params->MaxSpeed());
	motor.MaxAcceleration().Set(params->MaxAcceleration());

	motor.SmoothPositionUpdate().Set(expectedTsInMs);
}


} /* namespace hw */
} /* namespace liboncilla */
