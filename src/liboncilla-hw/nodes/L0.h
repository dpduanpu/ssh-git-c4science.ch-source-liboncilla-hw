#pragma once

#include <liboncilla/nodes/L0.h>

#include "liboncilla-hw/Common.h"
#include "liboncilla-hw/nodes/DeepCopyable.h"

namespace liboncilla {
namespace hw {

class L0: public rci::oncilla::L0 {
public:
    L0(const std::string & name);
    virtual ~L0();

	void updateForcesHardwareCoordinates(int force1, int force2, int force3); // I am not sure if type = int should be type = Data or not...

};

} /* namespace hw */
} /* namespace liboncilla */
