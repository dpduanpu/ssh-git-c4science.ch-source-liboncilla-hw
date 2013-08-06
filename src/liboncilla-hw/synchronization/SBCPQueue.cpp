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

SBCPQueue::SBCPQueue(const Config & config)
	: Queue(config.Main().SBCPPriority(),true), d_bus() {
	
	sbcp::Config sbcpConfig;
	sbcpConfig.LoadAllFiles();

	// create links to the 4 motorboards
	unsigned int requiredFrameSize = 4; //(=4 consecutive packets send at once from roboard to master communication board)
	// create links to the 4 motorboards
	unsigned int requiredPacketSize = 32; //(Size of the biggest packet you will send, the actual maximum might be higher)

	d_bus = sbcpConfig.OpenDefaultBusWithFrame(requiredFrameSize,requiredPacketSize);	

	if (!d_bus) {
		throw std::runtime_error("SBCPQueue::SBCPQueue() Can't load bus due to missing configuration.");
	} else {
		std::cout << "Bus Workflow: "<< d_bus->CurrentWorkflow() << std::endl;
	}

	const MotorDriverGroup & devices = config.Motors().Devices();
	const BrushlessParameterGroup & motorConfig = config.Motors().Params();
	int16_t timestep = config.Main().Timestep();

	d_motordrivers[rci::oncilla::LEFT_FORE]  = OpenAndConfigureMotorDriver(1,// todo arne: devices.LeftFore(),
	                                                                       motorConfig,
                                                                           timestep);
	d_motordrivers[rci::oncilla::RIGHT_FORE] = OpenAndConfigureMotorDriver(2,// todo arne: devices.RightFore(),
	                                                                       motorConfig,
                                                                           timestep);
	d_motordrivers[rci::oncilla::LEFT_HIND]  = OpenAndConfigureMotorDriver(3,// todo arne: devices.LeftHind(),
	                                                                       motorConfig,
                                                                           timestep);
	d_motordrivers[rci::oncilla::RIGHT_HIND] = OpenAndConfigureMotorDriver(4,// todo arne: devices.RightHind(),
	                                                                       motorConfig,
                                                                           timestep);

}

SBCPQueue::~SBCPQueue(){
}

void SBCPQueue::DownstreamData(){

	for(MotorAndEncoderByL1L2::const_iterator motAndEnc = d_motAndEncByL1L2.begin();
	    motAndEnc != d_motAndEncByL1L2.end();
	    ++motAndEnc) { 
		motAndEnc->second.motor->GoalPosition().Set(motAndEnc->first->nodeToQueueJointPosition());
	}


}

void SBCPQueue::UpstreamData(){
	for(MotorAndEncoderByL1L2::const_iterator motAndEnc = d_motAndEncByL1L2.begin();
	    motAndEnc != d_motAndEncByL1L2.end();
	    ++motAndEnc) { 
		uint16_t pos(motAndEnc->second.encoder->PositionAndStatus().Get() & 0x3fff);
		uint16_t status((motAndEnc->second.encoder->PositionAndStatus().Get() & 0xc000) >> 14);
		int16_t  motPos(motAndEnc->second.motor->PresentPosition().Get() );
		motAndEnc->first->queueToNodeJointPosition(pos,status,motPos);
	}

	for(MotorDriverByL0::const_iterator mdv = d_mdvByL0.begin();
	    mdv != d_mdvByL0.end();
	    ++mdv) {
		mdv->first->queueToNodeForces(mdv->second->Force(0).Get(),
		                              mdv->second->Force(1).Get(),
		                              mdv->second->Force(2).Get() );
	}
	    
	for(MagneticEncoderByL3::const_iterator enc = d_encByL3.begin();
	    enc != d_encByL3.end();
	    ++enc) {

		enc->first->queueToNodeJointPosition(enc->second->PositionAndStatus().Get() & 0x3fff ,
		                                     (enc->second->PositionAndStatus().Get() & 0xc000) >> 14);

	}

}

void SBCPQueue::PerformIO(){
	/// \todo : perform the IO
}

void SBCPQueue::InitializeIO(){
	/// \todo Alexandre :)
}

void SBCPQueue::RegisterL0(rci::oncilla::Leg l, const L0::Ptr & node){
	// check if this map contains this leg
	MotordriverByLeg::const_iterator fi = d_motordrivers.find(l);
	if(fi == d_motordrivers.end()){ 
		throw(std::runtime_error("Failed to open motordriver, upon initialization of this Queue, for motordriver " + LegPrefix(l)));
	}
	
	d_mdvByL0[node] = fi->second;


}

void SBCPQueue::RegisterL1(rci::oncilla::Leg l, const L1L2::Ptr & node){
	bool isRightLeg = ((l == rci::oncilla::RIGHT_FORE) || (l == rci::oncilla::RIGHT_HIND))? true: false;
	bool isHip = true;
	//check if this map contains this leg
	MotordriverByLeg::const_iterator fi = d_motordrivers.find(l);
	if(fi == d_motordrivers.end()){ 
		throw(std::runtime_error("Failed to open motordriver earlier, upon initialization of this Queue, for motordriver " + LegPrefix(l)));
	}

	node->initialize(isRightLeg,
	                 isHip, 
	                 fi->second->Motor1().PositionLimit().Get() & 0x7fff,
	                 fi->second->Motor1().PresentPosition().Get());

	/// \todo hardcoded mapping, could be configurable
	MotorAndEncoder m1;
	m1.motor   = &(fi->second->Motor1());
	m1.encoder = &(fi->second->Q1());

	d_motAndEncByL1L2[node] = m1;
   
}

void SBCPQueue::RegisterL2(rci::oncilla::Leg l, const L1L2::Ptr & node){
	bool isRightLeg = ((l == rci::oncilla::RIGHT_FORE) || (l == rci::oncilla::RIGHT_HIND))? true: false;

	bool isHip = false;
	//check if this map contains this leg
	MotordriverByLeg::const_iterator fi = d_motordrivers.find(l);
	if(fi == d_motordrivers.end()){ 
		throw(std::runtime_error("Failed to open motordriver earlier, upon initialization of this Queue, for motordriver " + LegPrefix(l)));
	}

	node->initialize(isRightLeg,
	                 isHip, 
	                 fi->second->Motor2().PositionLimit().Get() & 0x7fff,
	                 fi->second->Motor2().PresentPosition().Get());

	/// \todo hardcoded mapping, could be configurable
	MotorAndEncoder m2;
	m2.motor   = &(fi->second->Motor2());
	m2.encoder = &(fi->second->Q2());

	d_motAndEncByL1L2[node] = m2;
	
}

void SBCPQueue::RegisterL3(rci::oncilla::Leg l, const L3::Ptr & node){
	//check if this map contains this leg
	MotordriverByLeg::const_iterator fi = d_motordrivers.find(l);
	if(fi == d_motordrivers.end()){ 
		throw(std::runtime_error("Failed to open motordriver, upon initialization of this Queue, for motordriver " + LegPrefix(l)));
	}
	
	d_encByL3[node] = &(fi->second->Q3());
	
}

sbcp::amarsi::MotorDriver::Ptr 
SBCPQueue::OpenAndConfigureMotorDriver(int id,// todo arne: const MotorDriverSection & def,
                                       const BrushlessParameterGroup & params,
                                       int16_t expectedTsInMs) {

	if (!d_bus) {
		throw std::runtime_error("SBCPQueue::OpenAndConfigureMotorDriver() Can't load bus due to missing configuration.");
	} else {
		std::cout << "SBCPQueue::OpenAndConfigureMotorDriver() Bus Workflow: "<< d_bus->CurrentWorkflow() << std::endl;
	}

	sbcp::amarsi::MotorDriver::Ptr  res = d_bus->OpenDevice<sbcp::amarsi::MotorDriver>(id);// todo arne: def.BoardID());
	

	if(!res) {
		// board is just not here, buit we will throw later if it is required.
		return res;
	}

	/*SetMotorParameters(params,
	                   def.M1Params(),
	                   expectedTsInMs,
	                   res->Motor1());
	SetMotorParameters(params,
	                   def.M2Params(),
	                   expectedTsInMs,
	                   res->Motor2());*/


	return res;
}

void SBCPQueue::SetMotorParameters(const BrushlessParameterGroup & paramGroup,
                                   const std::string & paramName,
                                   int16_t expectedTsInMs,
                                   sbcp::amarsi::MotorDriver::Motor & motor) {
	//due to a bad design of biorob-cpp dynamic section we should do this
	std::tr1::shared_ptr<BrushlessParameterSection> params = const_cast<BrushlessParameterGroup & >(paramGroup).SubSection(paramName);
	
	if(!params) {
		throw std::runtime_error("Unknown motor parameter group '" + paramName + "'" );
	}

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
