/*
 * \file SBCPQueue.h
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#pragma once

#include "Queue.h"
#include <liboncilla-hw/nodes/Nodes.h>

#include <libsbcp/bus/Bus.h>
#include <libsbcp/devices/amarsi/MotorDriver.h>


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

	typedef std::map<liboncilla::hw::L0::Ptr, sbcp::amarsi::MotorDriver::Ptr > MotorDriverByL0;

	struct MotorAndEncoder{
		sbcp::amarsi::MotorDriver::Motor           * motor;
		sbcp::amarsi::MotorDriver::MagneticEncoder * encoder;
	};

	typedef std::map<liboncilla::hw::L1L2::Ptr, MotorAndEncoder > MotorAndEncoderByL1L2;
	typedef std::map<liboncilla::hw::L3::Ptr , sbcp::amarsi::MotorDriver::MagneticEncoder *> MagneticEncoderByL3;

	typedef std::map<rci::oncilla::Leg, sbcp::amarsi::MotorDriver::Ptr> MotordriverByLeg;

	MotorDriverByL0       d_mdvByL0;
	MotorAndEncoderByL1L2 d_motAndEncByL1L2;
	MagneticEncoderByL3   d_encByL3;

	MotordriverByLeg d_motordrivers;

	std::tr1::shared_ptr<sbcp::Bus> d_bus;
};

} /* namespace hw */
} /* namespace liboncilla */
