#pragma once

#include <liboncilla/nodes/Trunk.h>

#include "liboncilla-hw/Common.h"

namespace liboncilla {
namespace hw {

class Trunk: public rci::oncilla::Trunk {
public:
    Trunk();
    virtual ~Trunk();

};

} /* namespace hw */
} /* namespace liboncilla */

