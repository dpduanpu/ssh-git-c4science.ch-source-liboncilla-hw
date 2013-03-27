#pragma once

#include <liboncilla/nodes/SupervisorL4.h>

#include "liboncilla-hw/Common.h"

namespace liboncilla {
namespace hw {

using namespace rci;

/**
 * Supervising the L4 (foot) nodes has to be done by external sensory, e. g.:
 * - motion capturing system for L4 pose in cartesian world coordinates
 * - pressure plate for contact forces
 */
class SupervisorL4: public rci::oncilla::SupervisorL4 {
public:
    SupervisorL4(const std::string & name);
    virtual ~SupervisorL4();

    virtual PosePtr getPose() const;
    virtual ForcesPtr getForces() const;

};

} /* namespace hw */
} /* namespace liboncilla */
