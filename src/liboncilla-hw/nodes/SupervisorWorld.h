#pragma once

#include <liboncilla/nodes/SupervisorWorld.h>

#include "liboncilla-hw/Common.h"

namespace liboncilla {
namespace hw {

class SupervisorWorld: public rci::oncilla::SupervisorWorld {
public:
    SupervisorWorld();
    virtual ~SupervisorWorld();

    void reset();

    std::string print();

};

} /* namespace hw */
} /* namespace liboncilla */
