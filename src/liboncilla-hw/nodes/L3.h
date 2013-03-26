#pragma once

#include <liboncilla/nodes/L3.h>

#include <liboncilla-hw/Common.h>

namespace liboncilla {
namespace hw {

class L3: public rci::oncilla::L3 {
public:
    L3(const std::string & name);
    virtual ~L3();

};

} /* namespace hw */
} /* namespace liboncilla */
