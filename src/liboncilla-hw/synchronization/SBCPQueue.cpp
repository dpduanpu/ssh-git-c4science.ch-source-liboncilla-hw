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
}

SBCPQueue::~SBCPQueue(){
	// TODO Auto-generated destructor stub
}

void SBCPQueue::DownstreamData(){
	for(Motordriver::const_iterator i = d_motordriverByLeg.begin();
	    i != d_motordriverByLeg.end();
	    ++i){
		i->second->Motor1()->GoalPosition()->Set(L1NodeByLeg[i->first]->nodeToQueueJointPosition());//double
		i->second->Motor2()->GoalPosition()->Set(L2NodeByLeg[i->first]->nodeToQueueJointPosition());//double
	}
}

void SBCPQueue::UpstreamData(){
	for(Motordriver::const_iterator i = d_motordriverByLeg.begin();	i != d_motordriverByLeg.end(); ++i){
		// TODO: actually, one of those is a torque		
		d_l0NodeByLeg[i->first]->queueToNodeForces(i->second->Force(0),i->second->Force(1),i->second->Force(2) );

		d_l1NodeByLeg[i->first]->queueToNodeJointPosition(
			i->second->Q1()->PositionAndStatus()->Get() & 0x3fff ,
			(i->second->Q1()->PositionAndStatus()->Get() & 0xc000) >> 14,
			i->second->Motor1()->PresentPosition()->Get());
		
		d_l2NodeByLeg[i->first]->queueToNodeJointPosition(
			i->second->Q2()->PositionAndStatus()->Get() & 0x3fff ,
			(i->second->Q2()->PositionAndStatus()->Get() & 0xc000) >> 14,
			i->second->Motor2()->PresentPosition()->Get());
		
		d_l3NodeByLeg[i->first]->queueToNodeJointPosition(
			i->second->Q3()->PositionAndStatus()->Get() & 0x3fff ,
			(i->second->Q3()->PositionAndStatus()->Get() & 0xc000) >> 14);
	}
}

void SBCPQueue::PerformIO(){
	// nothing to do
}

void SBCPQueue::InitializeIO(){
	// TODO: Alexandre :)
}

void SBCPQueue::RegisterL0(rci::oncilla::Leg l, const L0::Ptr & node){
	// check if this map contains this leg
	Motordriver::const_iterator fi = d_motordriverByLeg.find(l);
	if(fi == d_motordriverByLeg.end()){ 
		throw(std::runtime_error("Failed to open motordriver earlier, upon initialization of this Queue, for motordriver " + LegPrefix(l)));
	}
	d_l0ByNode[node] = fi->second;
}

void SBCPQueue::RegisterL1(rci::oncilla::Leg l, const L1L2::Ptr & node){
	bool isLeftLeg = ((l == rci::oncilla::LEFT_FORE) || (l == rci::oncilla::LEFT_HIND))? true: false;
	
	//check if this map contains this leg
	Motordriver::const_iterator fi = d_motordriverByLeg.find(l);
	if(fi == d_motordriverByLeg.end()){ 
		throw(std::runtime_error("Failed to open motordriver earlier, upon initialization of this Queue, for motordriver " + LegPrefix(l)));
	}
	d_l1ByNode[node] = fi->second;
	d_l1ByNode[node]->initialize(!isLeftLeg, true, fi->first->Motor1()->PositionLimit()->Get());
}

void SBCPQueue::RegisterL2(rci::oncilla::Leg l, const L1L2::Ptr & node){
	bool isLeftLeg = ((l == rci::oncilla::LEFT_FORE) || (l == rci::oncilla::LEFT_HIND))? true: false;
	
	//check if this map contains this leg
	Motordriver::const_iterator fi = d_motordriverByLeg.find(l);
	if(fi == d_motordriverByLeg.end()){ 
		throw(std::runtime_error("Failed to open motordriver earlier, upon initialization of this Queue, for motordriver " + LegPrefix(l)));
	}
	d_l2ByNode[node] = fi->second;
	d_l2ByNode[node]->initialize(!isLeftLeg, false, fi->first->Motor2()->PositionLimit()->Get());
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
