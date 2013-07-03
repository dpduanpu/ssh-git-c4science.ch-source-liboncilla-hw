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
	setIsReversed(isReversed);
	setIsHip(isHip);
	setMaxMotorEncoderVal(maxMotorEncoderVal);

	if(d_isHip){
		d_hwDownJointAngles = d_maxMotorEncoderVal / 2.0;
	}
	else{
		d_hwDownJointAngles = d_isReversed? d_maxMotorEncoderVal: 0;
	}
}

void L1L2::setMaxMotorEncoderVal(double Val){
	d_MaxMotorEncoderVal = Val;
}

void L1L2::setIsHip(bool Val){
	d_isHip = Val;
}

void L1L2::setControlMode(ControlModePtr controlMode){
	NOT_IMPLEMENTED_STUB(L1L2, setControlMode);
}

bool L1L2::setJointImpedance(JointImpedancePtr){
	NOT_IMPLEMENTED_STUB(L1L2, setJointImpedance);
}

bool L1L2::setJointPosition(JointAnglesPtr position){
	if(!JointAnglesPtr)
		return false;

	d_userDownJointAngle = position->rad();

	// do the conversion, set the result
	if(d_isHip){
		d_hwDownJointAngle = (d_isReversed? -1: 1) * d_userDownJointAngle / (2.0 * M_PI) * 4096.0 + d_maxMotorEncoderVal / 2.0;
	}	
	else{
		d_hwDownJointAngle = d_isReversed? (maxMotorEncoderVal - userDownJointAngle * maxMotorEncoderVal): (userDownJointAngle * maxMotorEncoderVal);
	}
	return true;
}

double L1L2::getJointPositionHardwareCoordinates(){
	return d_hwDownJointAngles;
}

bool L1L2::setJointVelocity(JointVelocitiesPtr velocity){
	NOT_IMPLEMENTED_STUB(L1L2, setJointVelocity);
}

bool L1L2::setJointTorque(JointTorquesPtr position){
	NOT_IMPLEMENTED_STUB(L1L2, setJointTorque);
}

JointAnglesPtr L1L2::getJointPosition() const{
	hwUpJointangle = d_motorEncoderVal;

	// conversion
	if(d_isHip){
		userUpJointAngle = (d_isReversed? -1: 1) * (hwUpJointangle - d_maxMotorEncoderVal / 2.0) / 4096.0 * (2.0 * M_PI);
	}
	else{
		userUpJointAngle = d_isReversed? (1.0 - hwUpJointangle / d_maxMotorEncoderVal): (hwUpJointangle / d_maxMotorEncoderVal);
	}

	// set
	rci::JointAnglesPrt result = rci::JointAngles::fromRad(userUpJointAngle);
	return result;
}

void L1L2::updateJointPositionHardwareCoordinates(int magneticEncoderVal, int magneticEncoderStatus, int motorEncoderVal){
	d_magneticEncoderVal = magneticEncoderVal;
	d_magneticEncoderStatus = magneticEncoderStatus;
	d_motorEncoderVal = motorEncoderVal;
}

void L1L2::deepCopyResources(){
	_lastCommandedPosition = JointAngles::copy(*_lastCommandedPosition);

	_latestJointPosition = JointAngles::copy(*_latestJointPosition);

	_lastCommandedVelocity = JointVelocitiesPtr(
	        new JointVelocities(_lastCommandedVelocity->_values));

	_lastCommandedTorque = JointTorquesPtr(
	        new JointTorques(_lastCommandedTorque->_values));

	_lastCommandedImpedance = JointImpedancePtr(
	        new JointImpedance(_lastCommandedImpedance->_values));
}


void L1L2::unsafeUpdateJointPosition(unsigned int index, double value){
	_latestJointPosition->setFromRad(index,value);
}

} /* namespace hw */
} /* namespace liboncilla */
