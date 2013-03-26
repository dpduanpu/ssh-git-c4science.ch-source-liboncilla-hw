#pragma once

#include <liboncilla/nodes/SupervisorL4.h>

#include "liboncilla-hw/Common.h"

namespace liboncilla {
namespace hw {

using namespace rci;

class SupervisorL4: public rci::oncilla::SupervisorL4 {
public:
    SupervisorL4(const std::string & name);
    virtual ~SupervisorL4();

    virtual PosePtr getPose() const;
    virtual ForcesPtr getForces() const;

};

} /* namespace hw */
} /* namespace liboncilla */
