#pragma once

#include <liboncilla/nodes/L0.h>

#include "liboncilla-hw/Common.h"
#include "liboncilla-hw/nodes/DeepCopyable.h"

namespace liboncilla {
namespace hw {

class L0: public rci::oncilla::L0, public DeepCopyable {
public:
    L0(const std::string & name);
    virtual ~L0();

    virtual rci::ForcesPtr getForces() const;

	virtual void deepCopyResources();

	void updateForcesHardwareCoordinates(int force1, int force2, int force3); // I am not sure if type = int should be type = Data or not...

private:
	int d_hwUpForce1, d_hwUpForce2, d_hwUpForce3;
	rci::ForcesPtr d_userUpForces;

};

} /* namespace hw */
} /* namespace liboncilla */
