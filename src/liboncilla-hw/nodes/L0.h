#pragma once

#include <liboncilla/nodes/L0.h>

#include "liboncilla-hw/Common.h"

namespace liboncilla {
namespace hw {

class L0: public rci::oncilla::L0 {
public:
    L0(const std::string & name);
    virtual ~L0();

    virtual rci::ForcesPtr getForces() const {
        NOT_IMPLEMENTED_STUB(L0, getForces);
    }

};

} /* namespace hw */
} /* namespace liboncilla */
