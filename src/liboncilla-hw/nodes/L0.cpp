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

void L0::updateForcesHardwareCoordinates(int force1, int force2, int force3){
	// TODO: Do conversion
	updateForces({force1, force2, force3});
}

} /* namespace hw */
} /* namespace liboncilla */
