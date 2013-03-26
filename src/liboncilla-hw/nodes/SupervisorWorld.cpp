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
    throw std::runtime_error("You are not god - you can`t reset the world!"); // ;)
}

} /* namespace hw */
} /* namespace liboncilla */
