#include "L3.h"

#include "liboncilla-hw/Common.h"

using namespace liboncilla::hw;
using namespace rci;

L3::L3(const std::string & name)
	: rci::oncilla::L3(name){
}

L3::~L3(){
}

JointAnglesPtr L3::getJointPosition() const{
	double sign = 1, offset = 0; // TODO: BUT THIS SHOULD BE CHANGED
	d_userUpJointAngle = sign * (d_hwUpJointAngle + offset) / 4096.0 * (2 * M_PI);
	rci::JointAnglesPrt result = rci::JointAngles::fromRad(d_userUpJointAngle);
	return result;
}

void L3::updateJointPositionHardwareCoordinates(int magneticEncoderVal, int magneticEncoderStatus){
	d_magneticEncoderVal = magneticEncoderVal;
	d_magneticEncoderStatus = magneticEncoderStatus;

	d_hwUpJointAngle = d_magneticEncoderVal;
}

void L3::deepCopyResources(){
	_latestJointPosition = JointAngles::copy(*_latestJointPosition);
}

