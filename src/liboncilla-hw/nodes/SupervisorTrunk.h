#pragma once

#include <liboncilla/nodes/SupervisorTrunk.h>

namespace liboncilla {
namespace hw {

using namespace rci;

/**
 * Supervising the trunk in terms of it`s cartesian world coordinates has to
 * be provided by an external motion capturing system.
 */
class SupervisorTrunk: public rci::oncilla::SupervisorTrunk {
public:
    SupervisorTrunk();
    virtual ~SupervisorTrunk();

    virtual PosePtr getPose() const;
};

} /* namespace hw */
} /* namespace liboncilla */
