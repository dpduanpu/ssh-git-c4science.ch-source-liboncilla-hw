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

void L1L2::setControlMode(ControlModePtr controlMode){
	NOT_IMPLEMENTED_STUB(L1L2, setControlMode);
}

bool L1L2::setJointImpedance(JointImpedancePtr){
	NOT_IMPLEMENTED_STUB(L1L2, setJointImpedance);
}

bool L1L2::setJointPosition(JointAnglesPtr position){
	NOT_IMPLEMENTED_STUB(L1L2, setJointPosition);
}

bool L1L2::setJointVelocity(JointVelocitiesPtr velocity){
	NOT_IMPLEMENTED_STUB(L1L2, setJointVelocity);
}

bool L1L2::setJointTorque(JointTorquesPtr position){
	NOT_IMPLEMENTED_STUB(L1L2, setJointTorque);
}

JointAnglesPtr L1L2::getJointPosition() const{
	NOT_IMPLEMENTED_STUB(L1L2, getJointPosition);
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

} /* namespace hw */
} /* namespace liboncilla */
