#include "L1.h"

#include "liboncilla-hw/Common.h"

namespace liboncilla {
namespace hw {

using namespace rci;

L1::L1(rci::oncilla::Synchronizer & s, const std::string & name) :
        rci::oncilla::L1(s, name) {
}

L1::~L1() {
}

void L1::setControlMode(ControlModePtr controlMode) {
    NOT_IMPLEMENTED_STUB(L1, setControlMode);
}

bool L1::setJointImpedance(JointImpedancePtr) {
    NOT_IMPLEMENTED_STUB(L1, setJointImpedance);
}

bool L1::setJointPosition(JointAnglesPtr position) {
    NOT_IMPLEMENTED_STUB(L1, setJointPosition);
}

bool L1::setJointVelocity(JointVelocitiesPtr velocity) {
    NOT_IMPLEMENTED_STUB(L1, setJointVelocity);
}

bool L1::setJointTorque(JointTorquesPtr position) {
    NOT_IMPLEMENTED_STUB(L1, setJointTorque);
}

JointAnglesPtr L1::getJointPosition() const {
    NOT_IMPLEMENTED_STUB(L1, getJointPosition);
}

} /* namespace hw */
} /* namespace liboncilla */
