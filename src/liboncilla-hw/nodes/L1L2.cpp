#include "L1L2.h"

#include "liboncilla-hw/Common.h"

namespace liboncilla {
namespace hw {

using namespace rci;

L1L2::L1L2(rci::oncilla::Synchronizer & s , const std::string & name)
		: rci::oncilla::L1L2(s, name){
}

L1L2::~L1L2(){
}

void L1L2::initialize(bool isReversed, bool isHip, double maxMotorEncoderVal){
	d_isReversed = isReversed;
	d_isHip = isHip;
	d_MaxMotorEncoderVal = maxMotorEncoderVal;
	setJointPosition({0.0});
}

void L1L2::setControlMode(ControlModePtr controlMode){
	NOT_IMPLEMENTED_STUB(L1L2, setControlMode);
}

bool L1L2::setJointImpedance(JointImpedancePtr){
	NOT_IMPLEMENTED_STUB(L1L2, setJointImpedance);
}

double L1L2::nodeToQueueJointPosition(){
	//This is what the node sends this queue
	double userDownJointAngle = getLastPositionCommand()->rad()
	double hwDownJointAngle;
	// do the conversion, set the result
	if(d_isHip){
		hwDownJointAngle = (d_isReversed? -1: 1) * userDownJointAngle / (2.0 * M_PI) * 4096.0 + d_maxMotorEncoderVal / 2.0;
	}	
	else{
		hwDownJointAngle = d_isReversed? (maxMotorEncoderVal - userDownJointAngle * maxMotorEncoderVal): (userDownJointAngle * maxMotorEncoderVal);
	}
	return hwDownJointAngle;
}

bool L1L2::setJointVelocity(JointVelocitiesPtr velocity){
	NOT_IMPLEMENTED_STUB(L1L2, setJointVelocity);
}

bool L1L2::setJointTorque(JointTorquesPtr position){
	NOT_IMPLEMENTED_STUB(L1L2, setJointTorque);
}

JointAnglesPtr L1L2::getJointPosition() const{
}

void L1L2::queueToNodeJointPosition(int magneticEncoderVal, int magneticEncoderStatus, int motorEncoderVal){
	
	// We do the conversion between the hardware and liboncilla-coordinates here
	d_magneticEncoderVal = magneticEncoderVal;
	d_magneticEncoderStatus = magneticEncoderStatus;
	d_motorEncoderVal = motorEncoderVal;
	
	double userUpJointAngle = 0.0;
	double hwUpJointangle = (double) d_motorEncoderVal;

	//TODO: check status and report if necessary
	
	// conversion
	if(d_isHip){
		userUpJointAngle = (d_isReversed? -1: 1) * (hwUpJointangle - d_maxMotorEncoderVal / 2.0) / 4096.0 * (2.0 * M_PI);
	}
	else{
		userUpJointAngle = d_isReversed? (1.0 - hwUpJointangle / d_maxMotorEncoderVal): (hwUpJointangle / d_maxMotorEncoderVal);
	}

	// set from relative motor encoder values
	_latestJointPosition->setFromRad(MotorAxisIndex,userUpJointAngle);
	
	// TODO: convert magnetic encoder values
	// set from magnetic encoder values
	_latestJointPosition->setFromRad(MagneticEncoderIndex, ((double) magneticEncoderVal) / 4096.0 * 2.0 * M_PI);
	
	return result;
	
}

void L1L2::unsafeUpdateJointPosition(unsigned int index, double value){
	_latestJointPosition->setFromRad(index,value);
}

} /* namespace hw */
} /* namespace liboncilla */
