#include "L0.h"

#include "liboncilla-hw/Common.h"

namespace liboncilla {
namespace hw {

using namespace rci;

L0::L0(const std::string & name)
		: rci::oncilla::L0(name){
}

L0::~L0(){
}

void L0::deepCopyResources(){
	// JointAnglesPtr _lastCommandedPosition;
	_lastCommandedPosition = JointAngles::copy(*_lastCommandedPosition);

	// ForcesPtr forces;
	forces = ForcesPtr(new Forces(forces->_values));
}

} /* namespace hw */
} /* namespace liboncilla */
