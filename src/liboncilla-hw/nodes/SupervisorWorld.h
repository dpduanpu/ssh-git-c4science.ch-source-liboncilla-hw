#pragma once

#include <liboncilla/nodes/SupervisorWorld.h>

#include "liboncilla-hw/Common.h"

namespace liboncilla {
namespace hw {

/**
 * World supervisor. This one won't get implemented, because it only makes
 * sense in simualtion (you can`t reset the real worls after all ;).
 */
class SupervisorWorld: public rci::oncilla::SupervisorWorld {
public:
    SupervisorWorld();
    virtual ~SupervisorWorld();

    void reset();

    std::string print();

};

} /* namespace hw */
} /* namespace liboncilla */
