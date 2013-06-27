/*
 * \file SBCPQueue.h
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#pragma once

#include "Queue.h"
#include <liboncilla-hw/nodes/Nodes.h>

namespace liboncilla {
namespace hw {

class SBCPQueue : public Queue {
public:
	SBCPQueue(unsigned int priority);
	virtual ~SBCPQueue();

	virtual void DownstreamData();
	virtual void UpstreamData();
	virtual void PerformIO();
	virtual void InitializeIO();

	void RegisterL0(rci::oncilla::Leg l, const L0::Ptr & node);
	void RegisterL1(rci::oncilla::Leg l, const L1L2::Ptr & node);
	void RegisterL2(rci::oncilla::Leg l, const L1L2::Ptr & node);
	void RegisterL3(rci::oncilla::Leg l, const L3::Ptr & node);

};

} /* namespace hw */
} /* namespace liboncilla */

