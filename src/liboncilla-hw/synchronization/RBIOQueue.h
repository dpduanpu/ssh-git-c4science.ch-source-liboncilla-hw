/*
 * \file RBIOQueue.h
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#pragma once

#include "Queue.h"

#include <liboncilla-hw/nodes/Nodes.h>

#include <biorob-rbio/servo/ServoConfig.h>

namespace liboncilla {
namespace hw {

class RBIOQueue : public Queue {
public:
	RBIOQueue(unsigned int priority);
	virtual ~RBIOQueue();

	virtual void DownstreamData();
	virtual void UpstreamData();
	virtual void PerformIO();
	virtual void InitializeIO();
	virtual void DeinitializeIO();

	void RegisterL0(rci::oncilla::Leg l, const L0::Ptr & node);
private:
	typedef std::vector<std::string> ServoNameByIndex;
	typedef std::map<L0::Ptr, Servo::Ptr> ServoByNode;

	ServoGenerator::ServoByName d_servos;
	ServoByNode d_servoByNode;
	RTIME d_nextDate;
};

} /* namespace hw */
} /* namespace liboncilla */

