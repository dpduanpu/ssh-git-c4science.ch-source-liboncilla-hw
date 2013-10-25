/***
 * \file SBCPQueue.cpp
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#include "SBCPQueue.h"

#include <libsbcp/utils/Config.h>

#include <liboncilla-hw/config/Config.h>

namespace liboncilla {
namespace hw {

SBCPQueue::SBCPQueue(const Config & config) :
		Queue(config.Main().SBCPPriority(), true), d_bus() {
    //std::cout << "SBCPQueue::SBCPQueue()" << std::endl;
	sbcp::Config sbcpConfig;
	sbcpConfig.LoadAllFiles();

	// create links to the 4 motorboards
	unsigned int requiredFrameSize = 4; //(=4 consecutive packets send at oncefrom roboard to master communication board)
	// create links to the 4 motorboards
	unsigned int requiredPacketSize = 32; //(Size of the biggest packet you will send, the actual maximum might be higher)

	d_bus = sbcpConfig.OpenDefaultBusWithFrame(requiredFrameSize,
			requiredPacketSize);

	if (!d_bus) {
		throw std::runtime_error(
				"SBCPQueue::SBCPQueue() Can't load bus due to missing configuration.");
	} else {
		std::cout << "Bus Workflow: " << d_bus->CurrentWorkflow() << std::endl;
	}

	const MotorDriverGroup & devices = config.Motors().Devices();
	const BrushlessParameterGroup & motorConfig = config.Motors().Params();
	int16_t timestep = config.Main().Timestep();

	d_motordrivers[rci::oncilla::LEFT_FORE] = OpenAndConfigureMotorDriver(
			devices.LeftFore(), motorConfig, timestep);
	d_motordrivers[rci::oncilla::RIGHT_FORE] = OpenAndConfigureMotorDriver(
			devices.RightFore(), motorConfig, timestep);
	d_motordrivers[rci::oncilla::LEFT_HIND] = OpenAndConfigureMotorDriver(
			devices.LeftHind(), motorConfig, timestep);
	d_motordrivers[rci::oncilla::RIGHT_HIND] = OpenAndConfigureMotorDriver(
			devices.RightHind(), motorConfig, timestep);

}

SBCPQueue::~SBCPQueue() {
    //std::cout << "SBCPQueue::~SBCPQueue()" << std::endl;
}

void SBCPQueue::DownstreamData() {
	//std::cout << "------ SBCPQueue::DownstreamData() ------" << std::endl;
	for (MotorAndEncoderByL1L2::const_iterator motAndEnc =
			d_motAndEncByL1L2.begin(); motAndEnc != d_motAndEncByL1L2.end();
			++motAndEnc) {
		motAndEnc->second.motor->GoalPosition().Set(
				motAndEnc->first->nodeToQueueJointPosition());
		//std::cout << "set position: " << motAndEnc->first->nodeToQueueJointPosition() << std::endl;
		//std::cout << " set position: " << motAndEnc->second.motor->GoalPosition().Get() << std::endl;
	}
	//std::cout << "------ SBCPQueue::DownstreamData() ------" << std::endl << std::endl;
}

void SBCPQueue::UpstreamData() {
	//std::cout << "------ SBCPQueue::UpstreamData() ------" << std::endl;
	for (MotorAndEncoderByL1L2::const_iterator motAndEnc =
			d_motAndEncByL1L2.begin(); motAndEnc != d_motAndEncByL1L2.end();
			++motAndEnc) {
		uint16_t pos(
				motAndEnc->second.encoder->PositionAndStatus().Get() & 0x3fff);
		uint16_t status(
				(motAndEnc->second.encoder->PositionAndStatus().Get() & 0xc000)
						>> 14);
		int16_t motPos(motAndEnc->second.motor->PresentPosition().Get());
		motAndEnc->first->queueToNodeJointPosition(pos, status, motPos);
	}

	for (MotorDriverByL0::const_iterator mdv = d_mdvByL0.begin();
			mdv != d_mdvByL0.end(); ++mdv) {
		if (mdv->second == NULL) {
			// TODO: Throw? std::cout << "SBCPQueue::InitializeIO: Ignoring motor driver "<<md->second<<std::endl;
		} else {
			mdv->first->queueToNodeForces(mdv->second->Force(0).Get(),
					mdv->second->Force(1).Get(), mdv->second->Force(2).Get());
		}
	}

	for (MagneticEncoderByL3::const_iterator enc = d_encByL3.begin();
			enc != d_encByL3.end(); ++enc) {
		if (enc->second == NULL) {
			// TODO: Throw? std::cout << "SBCPQueue::InitializeIO: Ignoring motor driver "<<md->second<<std::endl;
		} else {
			enc->first->queueToNodeJointPosition(
					enc->second->PositionAndStatus().Get() & 0x3fff,
					(enc->second->PositionAndStatus().Get() & 0xc000) >> 14);
		}
	}
	//std::cout << "------ SBCPQueue::UpstreamData() ------" << std::endl << std::endl;
}

void SBCPQueue::PerformIO() {
    // std::cout << "------ SBCPQueue::PerformIO() ------" << std::endl;
	sbcp::ScheduledWorkflow & w = this->d_bus->Scheduled();

	// Start transfer
	w.StartTransfers();

	// Wait for transfer to complete
	w.WaitForTransfersCompletion();
	//std::cout << "------ SBCPQueue::PerformIO() ------" << std::endl << std::endl;
}

void SBCPQueue::InitializeIO() {
	// std::cout << "------ SBCPQueue::InitializeIO() ------" << std::endl;
	// Enable scheduled woirkflow
	sbcp::ScheduledWorkflow & w = this->d_bus->Scheduled();

	// Append devices to transfer
	for (MotordriverByLeg::iterator md = d_motordrivers.begin();
			md != d_motordrivers.end(); ++md) {

		if (md->second == NULL) {
			std::cout << "SBCPQueue::InitializeIO: Ignoring motor driver "
					<< md->second << std::endl;
		} else {
			std::cout << "appending scheduled device " << md->second
					<< std::endl;
			w.AppendScheduledDevice(
					std::tr1::static_pointer_cast<sbcp::Device>(md->second));
		}
	}
	//std::cout << "------ SBCPQueue::InitializeIO() ------" << std::endl << std::endl;
}

void SBCPQueue::DeinitializeIO() {
	// std::cout << "------ SBCPQueue::DeinitializeIO() ------" << std::endl;
	// Disable scheduled woirkflow
	this->d_bus->Lazy();
	// std::cout << "------ SBCPQueue::DeinitializeIO() ------" << std::endl << std::endl;
}

void SBCPQueue::RegisterL0(rci::oncilla::Leg l, const L0::Ptr & node) {
	// check if this map contains this leg
	MotordriverByLeg::const_iterator fi = d_motordrivers.find(l);
	if (fi == d_motordrivers.end()) {
		throw(std::runtime_error(
				"Failed to open motordriver, upon initialization of this Queue, for motordriver "
						+ LegPrefix(l)));
	}

	d_mdvByL0[node] = fi->second;
}

void SBCPQueue::RegisterL1(rci::oncilla::Leg l, const L1L2::Ptr & node) {
	bool isLeftLeg =
			((l == rci::oncilla::LEFT_FORE) || (l == rci::oncilla::LEFT_HIND)) ?
					true : false;
	bool isHip = true;

	//check if this map contains this leg
	MotordriverByLeg::const_iterator fi = d_motordrivers.find(l);
	if (fi == d_motordrivers.end()) {
		throw(std::runtime_error(
				"Failed to open motordriver earlier, upon initialization of this Queue, for motordriver "
						+ LegPrefix(l)));
	}

	if (fi->second == NULL) {
		// TODO: Throw?
		std::cout << "SBCPQueue::RegisterL1: Ignoring L1 of leg "
				<< LegPrefix(l) << ". Is it connected?" << std::endl;
		return;
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
	bool isRightLeg =
			((l == rci::oncilla::RIGHT_FORE) || (l == rci::oncilla::RIGHT_HIND)) ?
					true : false;
	bool isHip = false;

	//check if this map contains this leg
	MotordriverByLeg::const_iterator fi = d_motordrivers.find(l);
	if (fi == d_motordrivers.end()) {
		throw(std::runtime_error(
				"Failed to open motordriver earlier, upon initialization of this Queue, for motordriver "
						+ LegPrefix(l)));
	}

	if (fi->second == NULL) {
		// TODO: Throw?
		std::cout << "SBCPQueue::RegisterL2: Ignoring L2 of leg "
				<< LegPrefix(l) << ". Is it connected?" << std::endl;
		return;
	}

	node->initialize(isRightLeg, isHip,
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
	if (fi == d_motordrivers.end()) {
		throw(std::runtime_error(
				"Failed to open motordriver, upon initialization of this Queue, for motordriver "
						+ LegPrefix(l)));
	}

	if (fi->second == NULL) {
		// TODO: Throw?
		std::cout << "SBCPQueue::RegisterL3: Ignoring L3 of leg "
				<< LegPrefix(l) << ". Is it connected?" << std::endl;
		return;
	}

	d_encByL3[node] = &(fi->second->Q3());
}

sbcp::amarsi::MotorDriver::Ptr SBCPQueue::OpenAndConfigureMotorDriver(
		const MotorDriverSection & def, const BrushlessParameterGroup & params,
		int16_t expectedTsInMs) {
    std::cout << "SBCPQueue::OpenAndConfigureMotorDriver()" << std::endl;

    this->d_bus->Lazy();

	if (!d_bus) {
		throw std::runtime_error(
				"SBCPQueue::OpenAndConfigureMotorDriver() Can't"
						" load bus due to missing configuration.");
	} else {
		std::cout << "SBCPQueue::OpenAndConfigureMotorDriver() Bus Workflow: "
				<< d_bus->CurrentWorkflow() << std::endl;
	}

	sbcp::amarsi::MotorDriver::Ptr res = d_bus->OpenDevice
			< sbcp::amarsi::MotorDriver > (def.BoardID());

	if (!res) {
		// board is just not here, buit we will throw later if it is required.
		std::cout << "SBCPQueue::OpenAndConfigureMotorDriver() Ignoring board, "
                << "doesn't seem to be connected." << std::endl;
		return res;
	}

    bool m1Calibrated(GetCalibrationStatus(res->Motor1(),1));
	bool m2Calibrated(GetCalibrationStatus(res->Motor2(),2));
	if(! (m1Calibrated || m2Calibrated)){
		res->CalibrateMotors();
	} else {
		std::cout << "Motor already calibrated"<<std::endl;
	}

	while(!(m1Calibrated && m2Calibrated)){
		usleep(1e5);
		m1Calibrated = GetCalibrationStatus(res->Motor1(), 1);
		m2Calibrated = GetCalibrationStatus(res->Motor2(), 2);
	}
	std::cout << "Motor calibrated" << std::endl << std::endl;

	SetMotorParameters(params, def.M1Params(), expectedTsInMs, res->Motor1());
	SetMotorParameters(params, def.M2Params(), expectedTsInMs, res->Motor2());


	res->Motor1().MaxTorque().Set(300); // 300
	res->Motor1().MaxSpeed().Set(20000); // 32000
	res->Motor1().MaxAcceleration().Set(400); // 800

	res->Motor2().MaxTorque().Set(300);
	res->Motor2().MaxSpeed().Set(20000);
	res->Motor2().MaxAcceleration().Set(400);

	return res;
}

void SBCPQueue::SetMotorParameters(const BrushlessParameterGroup & paramGroup,
		const std::string & paramName, int16_t expectedTsInMs,
		sbcp::amarsi::MotorDriver::Motor & motor) {
    std::cout << "SBCPQueue::SetMotorParameters()" << std::endl;
	//due to a bad design of biorob-cpp dynamic section we should do this
	std::tr1::shared_ptr<BrushlessParameterSection> params =
			const_cast<BrushlessParameterGroup &>(paramGroup).SubSection(
					paramName);

	if (!params) {
		throw std::runtime_error(
				"Unknown motor parameter group '" + paramName + "'");
	}

	motor.MotorControlMode().Set(
			sbcp::amarsi::MotorDriver::Motor::SMOOTH_POSITION);

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

bool SBCPQueue::GetCalibrationStatus(sbcp::amarsi::MotorDriver::Motor & m, int number) {
	uint16_t val(m.MotorControlMode().Get());
	return val & (1 << 9);
}

} /* namespace hw */
} /* namespace liboncilla */
