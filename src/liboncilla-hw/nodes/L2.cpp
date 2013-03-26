#include "L2.h"

#include "liboncilla-hw/Common.h"

namespace liboncilla {
namespace hw {

using namespace rci;

L2::L2(rci::oncilla::Synchronizer & s, const std::string & name) :
        rci::oncilla::L2(s, name) {
}

L2::~L2() {
}

void L2::setControlMode(ControlModePtr controlMode) {
    NOT_IMPLEMENTED_STUB(L2, setControlMode);
}

bool L2::setJointImpedance(JointImpedancePtr) {
    NOT_IMPLEMENTED_STUB(L2, setJointImpedance);
}

bool L2::setJointPosition(JointAnglesPtr position) {
    NOT_IMPLEMENTED_STUB(L2, setJointPosition);
}

bool L2::setJointVelocity(JointVelocitiesPtr velocity) {
    NOT_IMPLEMENTED_STUB(L2, setJointVelocity);
}

bool L2::setJointTorque(JointTorquesPtr position) {
    NOT_IMPLEMENTED_STUB(L2, setJointTorque);
}

JointAnglesPtr L2::getJointPosition() const {
    NOT_IMPLEMENTED_STUB(L2, getJointPosition);
}

} /* namespace hw */
} /* namespace liboncilla */
