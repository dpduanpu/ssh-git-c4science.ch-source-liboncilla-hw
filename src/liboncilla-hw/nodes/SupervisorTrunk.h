#pragma once

#include <liboncilla/nodes/SupervisorTrunk.h>

namespace liboncilla {
namespace hw {

using namespace rci;

class SupervisorTrunk: public rci::oncilla::SupervisorTrunk {
public:
    SupervisorTrunk();
    virtual ~SupervisorTrunk();

    virtual PosePtr getPose() const;
};

} /* namespace hw */
} /* namespace liboncilla */
