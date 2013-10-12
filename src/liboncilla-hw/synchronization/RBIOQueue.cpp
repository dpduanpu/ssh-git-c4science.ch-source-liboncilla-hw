/*
 * \file RBIOQueue.cpp
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#include "RBIOQueue.h"
#include <native/timer.h>

namespace liboncilla {
namespace hw {

RBIOQueue::RBIOQueue(unsigned int priority)
	: Queue(priority,true)
	, d_nextDate(0){
	ServoGenerator g;
	g.LoadAllFiles();
	g.CreateServoFromDefinition(d_servos);
}

RBIOQueue::~RBIOQueue(){
}

void RBIOQueue::DownstreamData(){
	d_nextDate = rt_timer_read() + 20e6;
	for(ServoByNode::iterator s = d_servoByNode.begin();
	    s != d_servoByNode.end();
	    ++s){
		s->second->SetCommand(s->first->getLastPositionCommand()->deg(0));
	}
}

void RBIOQueue::UpstreamData(){
	//nothing to do
}

void RBIOQueue::PerformIO(){
	xeno_call(rt_task_sleep_until,d_nextDate);
}

void RBIOQueue::InitializeIO(){
	//nothing to do
}

void RBIOQueue::DeinitializeIO(){
	//nothing to do
}

void RBIOQueue::RegisterL0(rci::oncilla::Leg l , const L0::Ptr& node){

	ServoGenerator::ServoByName::const_iterator fi = d_servos.find(LegPrefix(l));

	if ( fi == d_servos.end() ) {
		throw std::runtime_error("Could not find suitable device for servo '" + LegPrefix(l) + "'.");
	}

	d_servoByNode[node] = fi->second;
}

} /* namespace hw */
} /* namespace liboncilla */
