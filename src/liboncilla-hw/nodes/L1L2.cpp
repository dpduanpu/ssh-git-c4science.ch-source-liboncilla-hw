#include "L1L2.h"

#include "liboncilla-hw/Common.h"

namespace liboncilla {
namespace hw {

using namespace rci;

L1L2::L1L2(rci::oncilla::Synchronizer & s , const std::string & name)
	: rci::oncilla::L1L2(s, name)
	, d_isHip(false)
	, d_isReversed(false)
	, d_motorPositionLimit(0){
}

L1L2::~L1L2(){
}

	void L1L2::initialize(bool isReversed, bool isHip, int16_t motorPositionLimit, int16_t currentPosition){
	d_isReversed = isReversed;
	d_isHip = isHip;
	d_motorPositionLimit = motorPositionLimit;

	//sets the default motor position to the current position.

	setJointPosition(rci::JointAngles::fromRad(2 * M_PI / 4096.0 * currentPosition) );
}

void L1L2::setControlMode(ControlModePtr controlMode){
	NOT_IMPLEMENTED_STUB(L1L2, setControlMode);
}

bool L1L2::setJointImpedance(JointImpedancePtr){
	NOT_IMPLEMENTED_STUB(L1L2, setJointImpedance);
}

double L1L2::nodeToQueueJointPosition(){
	//This is what the node sends this queue
	double userDownJointAngle = getLastPositionCommand()->rad();
	double hwDownJointAngle;
	// do the conversion, set the result
	if(d_isHip){
		hwDownJointAngle = (d_isReversed? -1: 1) * userDownJointAngle / (2.0 * M_PI) * 4096.0 + d_motorPositionLimit / 2.0;
	}	
	else{
		hwDownJointAngle = d_isReversed ? ( d_motorPositionLimit * (1.0 - userDownJointAngle ) )
		                                : ( d_motorPositionLimit * userDownJointAngle );
	}
	return hwDownJointAngle;
}

bool L1L2::setJointVelocity(JointVelocitiesPtr velocity){
	NOT_IMPLEMENTED_STUB(L1L2, setJointVelocity);
}

bool L1L2::setJointTorque(JointTorquesPtr position){
	NOT_IMPLEMENTED_STUB(L1L2, setJointTorque);
}


void L1L2::queueToNodeJointPosition(int16_t magneticEncoderVal, 
                                    int16_t magneticEncoderStatus, 
                                    int16_t motorPosition){
	
	// We do the conversion between the hardware and liboncilla-coordinates here	
	double userUpJointAngle = 0.0;
	double hwUpJointangle = (double) motorPosition;

	/// \todo check status and report if necessary
	
	// conversion
	if(d_isHip){
		userUpJointAngle = (d_isReversed? -1: 1) * (hwUpJointangle - d_motorPositionLimit / 2.0) / 4096.0 * (2.0 * M_PI);
	}
	else{
		userUpJointAngle = d_isReversed? (1.0 - hwUpJointangle / d_motorPositionLimit): (hwUpJointangle / d_motorPositionLimit);
	}

	// set from relative motor encoder values
	_latestJointPosition->setFromRad(MotorAxisIndex,userUpJointAngle);
	
	/// \todo convert magnetic encoder values
	// set from magnetic encoder values
	_latestJointPosition->setFromRad(MagneticEncoderIndex, ((double) magneticEncoderVal) / 4096.0 * 2.0 * M_PI);
	
}

void L1L2::unsafeUpdateJointPosition(unsigned int index, double value){
	_latestJointPosition->setFromRad(index,value);
}

} /* namespace hw */
} /* namespace liboncilla */
