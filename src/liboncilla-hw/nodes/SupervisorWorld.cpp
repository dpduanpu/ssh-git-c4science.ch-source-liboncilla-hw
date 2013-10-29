#include "SupervisorWorld.h"

namespace liboncilla {
namespace hw {

SupervisorWorld::SupervisorWorld() {
}

SupervisorWorld::~SupervisorWorld() {
}

std::string SupervisorWorld::print() {
	return std::string("Non-existing World Supervisor");
}

void SupervisorWorld::reset() {
	UNAVAILABLE_INTERFACE("You are not god, You cannot reset this real world !",
			SupervisorWorld, reset);
}

} /* namespace hw */
} /* namespace liboncilla */
