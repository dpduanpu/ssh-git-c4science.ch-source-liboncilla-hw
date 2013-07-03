/*
 * \file SBCPQueue.h
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#pragma once

#include "Queue.h"
#include <liboncilla-hw/nodes/Nodes.h>

#include <libsbcp/utils/Config.h>
#include <libsbcp/devices/amarsi/MotorDriver.h>

#include <libsbcp/bus/Bus.h>
#include <libsbcp/devices/motordriver_board/MotorDriver.h>
#include <sbcpd/si/ft2232h.h>

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

private:
	//std::tr1::shared_ptr<sbcp::amarsi::MotorDriver> d_motordrivers[4];
	
	typedef std::map<rci::oncilla::Leg, L0::Ptr> L0NodeByLeg;
	typedef std::map<rci::oncilla::Leg, L1::Ptr> L1NodeByLeg;
	typedef std::map<rci::oncilla::Leg, L2::Ptr> L2NodeByLeg;
	typedef std::map<rci::oncilla::Leg, L3::Ptr> L3NodeByLeg;
	
	typedef std::map<rci::oncilla::Leg, sbcp::amarsi::MotorDriver::Ptr> MotordriverByLeg;
	
	L0ByNode d_l0ByNode;
	L1ByNode d_l1ByNode;
	L2ByNode d_l2ByNode;
	L3ByNode d_l3ByNode;

	MotordriverByLeg d_motordriverByLeg;
};

} /* namespace hw */
} /* namespace liboncilla */

