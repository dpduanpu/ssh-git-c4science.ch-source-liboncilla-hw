#include "L3.h"

#include "liboncilla-hw/Common.h"

namespace liboncilla {
namespace hw {

L3::L3(const std::string & name)
		: rci::oncilla::L3(name){
}

L3::~L3(){
}

JointAnglesPtr L3::getJointPosition() const{
	NOT_IMPLEMENTED_STUB(L3, getJointPosition);
}

void L3::deepCopyResources(){
	_latestJointPosition = JointAngles::copy(*_latestJointPosition);
}

} /* namespace hw */
} /* namespace liboncilla */
