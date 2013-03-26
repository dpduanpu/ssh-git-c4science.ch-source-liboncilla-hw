#pragma once

#include <liboncilla/nodes/L1L2L2.h>

namespace liboncilla {
namespace hw {

using namespace rci;

class L1L2: public rci::oncilla::L1L2 {
public:
    L1L2(rci::oncilla::Synchronizer & s, const std::string & name);
    virtual ~L1L2();

    virtual void setControlMode(ControlModePtr controlMode);

    virtual bool setJointImpedance(JointImpedancePtr);
    virtual bool setJointPosition(JointAnglesPtr position);
    virtual bool setJointVelocity(JointVelocitiesPtr velocity);
    virtual bool setJointTorque(JointTorquesPtr position);

    virtual JointAnglesPtr getJointPosition() const;
};

} /* namespace hw */
} /* namespace liboncilla */