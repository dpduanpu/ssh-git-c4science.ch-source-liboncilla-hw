#include "SupervisorL4.h"

namespace liboncilla {
namespace hw {

SupervisorL4::SupervisorL4(const std::string & name) :
        rci::oncilla::SupervisorL4(name) {
}

SupervisorL4::~SupervisorL4() {
}

PosePtr SupervisorL4::getPose() const {
    NOT_IMPLEMENTED_STUB(SupervisorL4, getPose);
}

ForcesPtr SupervisorL4::getForces() const {
    NOT_IMPLEMENTED_STUB(SupervisorL4, getForces);
}

} /* namespace hw */
} /* namespace liboncilla */
