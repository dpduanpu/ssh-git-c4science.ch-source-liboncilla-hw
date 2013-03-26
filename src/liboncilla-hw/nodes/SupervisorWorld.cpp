#include "SupervisorWorld.h"

namespace liboncilla {
namespace hw {

using namespace rsc;
using namespace rsc::logging;


SupervisorWorld::SupervisorWorld() :
        logger(Logger::getLogger("low.supervisor")) {
}

SupervisorWorld::~SupervisorWorld() {
}

std::string SupervisorWorld::print() {
    return std::string("Non-existing World Supervisor");
}

void SupervisorWorld::reset() {
    // You are not - you cannot reset the world! ;)
    UNAVAILABLE_INTERFACE(SupervisorWorld, reset);
}

} /* namespace hw */
} /* namespace liboncilla */
