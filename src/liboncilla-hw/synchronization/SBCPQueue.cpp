/***
 * \file SBCPQueue.cpp
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#include "SBCPQueue.h"

namespace liboncilla {
namespace hw {

SBCPQueue::SBCPQueue(unsigned int priority)
	: Queue(priority,true){
	// create links to the 4 motorboards
	unsigned int requiredFrameSize = 4; //(=4 consecutive packets send at once from roboard to master communication board)
	// create links to the 4 motorboards
	unsigned int requiredPacketSize = 32; //(Size of the biggest packet you will send, the actual maximum might be higher)
	std::tr1::shared_ptr<Bus> sbcpbus = OpenDefaultBusWithFrame(requiredFrameSize, requiredPacketSize);
	

	//TODO: do this config-file-wise
	d_motordriverByLeg[rci::oncilla::LEFT_FORE] = sbcpbus->OpenDevice(0x01);
	d_motordriverByLeg[rci::oncilla::RIGHT_FORE] = sbcpbus->OpenDevice(0x02);
	d_motordriverByLeg[rci::oncilla::LEFT_HIND] = sbcpbus->OpenDevice(0x03);
	d_motordriverByLeg[rci::oncilla::RIGHT_HIND] = sbcpbus->OpenDevice(0x04);
	/*for(int i = 0; i<4; i++){
		e = false;
		//TODO: biorob-cpp-log
		if(d_motordrivers[i] == NULL){th
			cerr << "Failed to open device " << i << " because of things.";
		}
	}
	if(e){
		std::stringstream msg = "";
		msg << "Error opening motordriver board index = " << i;  
		throw(std::runtime_error(msg.str()));
	}*/
	

}

SBCPQueue::~SBCPQueue(){
	// TODO Auto-generated destructor stub
}

void SBCPQueue::DownstreamData(){
	for(Motordriver::const_iterator i = d_motordriverByLeg.begin();
	    i != d_motordriverByLeg.end();
	    ++i){
		i->second->Motor1()->GoalPosition()->Set(L1NodeByLeg[i->first]->getJointPositionHardwareCoordinates());//double
		i->second->Motor2()->GoalPosition()->Set(L2NodeByLeg[i->first]->getJointPositionHardwareCoordinates());//double
	}
}

void SBCPQueue::UpstreamData(){
	for(Motordriver::const_iterator i = d_motordriverByLeg.begin();	    i != d_motordriverByLeg.end();	    ++i){
		
		// TODO: actually, one of those is a torque		
		L0NodeByLeg[i->first]->updateForcesHardwareCoordinates( i->second->Force(0),i->second->Force(1),i->second->Force(2) );

		L1NodeByLeg[i->first]->updateJointPositionHardwareCoordinates(i->second->Q1()->PositionAndStatus()->Get() & 0x3fff ,
																	 (i->second->Q1()->PositionAndStatus()->Get() & 0xc000) >> 14,
																	  i->second->Motor1()->PresentPosition()->Get());
		
		L2NodeByLeg[i->first]->updateJointPositionHardwareCoordinates(i->second->Q2()->PositionAndStatus()->Get() & 0x3fff ,
																	 (i->second->Q2()->PositionAndStatus()->Get() & 0xc000) >> 14,
																	  i->second->Motor2()->PresentPosition()->Get());
		
		L3NodeByLeg[i->first]->updateJointPositionHardwareCoordinates(i->second->Q3()->PositionAndStatus()->Get() & 0x3fff ,
																	 (i->second->Q3()->PositionAndStatus()->Get() & 0xc000) >> 14);
	}
}

void SBCPQueue::PerformIO(){
	xeno_call(rt_task_sleep_until,d_nextDate);
}

void SBCPQueue::InitializeIO(){
	//nothing to do
}

void SBCPQueue::RegisterL0(rci::oncilla::Leg l, const L0::Ptr & node){
	
	//check if this map contains this leg
	Motordriver::const_iterator fi = d_motordriverByLeg.find(l);
	if(fi == d_motordriverByLeg.end()){ 
		throw(std::runtime_error("Failed to open motordriver earlier, upon initialization of this Queue, for motordriver " + LegPrefix(l)));
	}
	d_l0ByNode[node] = fi->second;
}

void SBCPQueue::RegisterL1(rci::oncilla::Leg l, const L1L2::Ptr & node){
	
	//check if this map contains this leg
	Motordriver::const_iterator fi = d_motordriverByLeg.find(l);
	if(fi == d_motordriverByLeg.end()){ 
		throw(std::runtime_error("Failed to open motordriver earlier, upon initialization of this Queue, for motordriver " + LegPrefix(l)));
	}
	d_l1ByNode[node] = fi->second;
}

void SBCPQueue::RegisterL2(rci::oncilla::Leg l, const L1L2::Ptr & node){
	
	//check if this map contains this leg
	Motordriver::const_iterator fi = d_motordriverByLeg.find(l);
	if(fi == d_motordriverByLeg.end()){ 
		throw(std::runtime_error("Failed to open motordriver earlier, upon initialization of this Queue, for motordriver " + LegPrefix(l)));
	}
	d_l2ByNode[node] = fi->second;
}

void SBCPQueue::RegisterL3(rci::oncilla::Leg l, const L3::Ptr & node){
	
	//check if this map contains this leg
	Motordriver::const_iterator fi = d_motordriverByLeg.find(l);
	if(fi == d_motordriverByLeg.end()){ 
		throw(std::runtime_error("Failed to open motordriver earlier, upon initialization of this Queue, for motordriver " + LegPrefix(l)));
	}
	d_l3ByNode[node] = fi->second;
}

} /* namespace hw */
} /* namespace liboncilla */
