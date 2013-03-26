#include "L1L2.h"

#include "liboncilla-hw/Common.h"

namespace liboncilla {
namespace hw {

using namespace rci;

L1L2::L1L2(rci::oncilla::Synchronizer & s, const std::string & name) :
        rci::oncilla::L1L2(s, name) {
}

L1L2::~L1L2() {
}

void L1L2::setControlMode(ControlModePtr controlMode) {
    NOT_IMPLEMENTED_STUB(L1L2, setControlMode);
}

bool L1L2::setJointImpedance(JointImpedancePtr) {
    NOT_IMPLEMENTED_STUB(L1L2, setJointImpedance);
}

bool L1L2::setJointPosition(JointAnglesPtr position) {
    NOT_IMPLEMENTED_STUB(L1L2, setJointPosition);
}

bool L1L2::setJointVelocity(JointVelocitiesPtr velocity) {
    NOT_IMPLEMENTED_STUB(L1L2, setJointVelocity);
}

bool L1L2::setJointTorque(JointTorquesPtr position) {
    NOT_IMPLEMENTED_STUB(L1L2, setJointTorque);
}

JointAnglesPtr L1L2::getJointPosition() const {
    NOT_IMPLEMENTED_STUB(L1L2, getJointPosition);
}

} /* namespace hw */
} /* namespace liboncilla */
