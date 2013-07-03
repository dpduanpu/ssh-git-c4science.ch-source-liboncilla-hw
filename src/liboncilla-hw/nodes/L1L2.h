#pragma once

#include <liboncilla/nodes/L1L2.h>

#include "liboncilla-hw/nodes/DeepCopyable.h"

namespace liboncilla {
namespace hw {

using namespace rci;

class L1L2 : public rci::oncilla::L1L2, public DeepCopyable {
public:
	typedef boost::shared_ptr<L1L2> Ptr;
	L1L2(rci::oncilla::Synchronizer & s , const std::string & name);
	virtual ~L1L2();

	virtual void setControlMode(ControlModePtr controlMode);

	virtual bool setJointImpedance(JointImpedancePtr);
	virtual bool setJointPosition(JointAnglesPtr position);
	virtual bool setJointVelocity(JointVelocitiesPtr velocity);
	virtual bool setJointTorque(JointTorquesPtr position);

	virtual JointAnglesPtr getJointPosition() const;

	virtual void deepCopyResources();

	void unsafeUpdateJointPosition(unsigned int index,double value);

	void setMaxMotorEncoderVal(double Val);
	void setIsHip(bool Val);
	void setIsReversed(bool Val);

	void initialize(bool isReversed, bool isHip, double maxMotorEncoderVal);

	double getJointPositionHardwareCoordinates(); // The next 3 functions should not be public, but friends of the SBCPQueue...
	void updateJointPositionHardwareCoordinates(int magneticEncoderVal, int magneticEncoderStatus, int motorEncoderVal);

private:
	bool d_isHip, d_isReversed;
	double d_maxMotorEncoderVal;
	double d_userDownJointAngle, d_userUpJointAngle;
	double d_hwDownJointAngle, d_hwUpJointAngle; // Down means downstream and Up means upstream
	int d_magneticEncoderVal, d_magneticEncoderStatus, d_motorEncoderVal;
};

} /* namespace hw */
} /* namespace liboncilla */
