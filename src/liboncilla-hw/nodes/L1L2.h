#pragma once

#include <liboncilla/nodes/L1L2.h>

#include "liboncilla-hw/nodes/DeepCopyable.h"

namespace liboncilla {
namespace hw {

using namespace rci;

class L1L2 : public rci::oncilla::L1L2 {
public:
	typedef boost::shared_ptr<L1L2> Ptr;
	L1L2(rci::oncilla::Synchronizer & s , const std::string & name);
	virtual ~L1L2();

	virtual void setControlMode(ControlModePtr controlMode);

	virtual bool setJointImpedance(JointImpedancePtr);
	virtual bool setJointVelocity(JointVelocitiesPtr velocity);
	virtual bool setJointTorque(JointTorquesPtr position);
	
	void unsafeUpdateJointPosition(unsigned int index,double value);

	void initialize(bool isReversed, 
	                bool isHip, 
	                int16_t motorPositionLimit, 
	                int16_t motorCurrentPosition);

	double nodeToQueueJointPosition(); // The next function should not be public, but friends of the SBCPQueue...
	void queueToNodeJointPosition(int16_t magneticEncoderVal,
	                              int16_t magneticEncoderStatus, 
	                              int16_t motorEncoderVal);

private:
	bool d_isHip, d_isReversed;
	double d_motorPositionLimit;
};

} /* namespace hw */
} /* namespace liboncilla */
