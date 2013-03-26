#include "DeepCopyable.h"

namespace liboncilla {
namespace hw {

DeepCopyable::DeepCopyable() {
}

DeepCopyable::~DeepCopyable() {
    // Remove myself from the set before destruction
    ChangedDeepCopyables.erase(this);
}

void DeepCopyable::mark() {
    // Add myself to the set
    ChangedDeepCopyables.insert(this);
}

void makeDeepCopies() {
    for (std::set<DeepCopyable*>::iterator i = ChangedDeepCopyables.begin();
            i != ChangedDeepCopyables.end(); ++i) {
        (*i)->deepCopyResources();
    }
}

void clearMark() {
    ChangedDeepCopyables.clear();
}

}
}
