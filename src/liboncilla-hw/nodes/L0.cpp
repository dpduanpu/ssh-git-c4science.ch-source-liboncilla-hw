#include "L0.h"

#include "liboncilla-hw/Common.h"

namespace liboncilla {
namespace hw {

using namespace rci;

L0::L0(const std::string & name)
		: rci::oncilla::L0(name){
	d_userUpForces = ForcesPtr(new rci::Forces(0, 0, 0));
}

L0::~L0(){
}

void L0::updateForcesHardwareCoordinates(int force1, int force2, int force3){
	d_hwUpForce1 = force1;
	d_hwUpForce2 = force2;
	d_hwUpForce3 = force3;
}

rci::ForcesPtr L0::getForces(){
	d_userUpForces->setValues({d_hwUpForce1, d_hwUpForce2, d_hwUpForce3});
	return d_userUpForces;
}

void L0::deepCopyResources(){
	// JointAnglesPtr _lastCommandedPosition;
	_lastCommandedPosition = JointAngles::copy(*_lastCommandedPosition);

	// ForcesPtr forces;
	forces = ForcesPtr(new Forces(forces->_values));
}

} /* namespace hw */
} /* namespace liboncilla */
