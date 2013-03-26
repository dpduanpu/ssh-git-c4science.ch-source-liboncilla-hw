#include "DeepCopyable.h"

namespace liboncilla {
namespace hw {

std::set<DeepCopyable*> DeepCopyable::s_changedDeepCopyables;

DeepCopyable::DeepCopyable(){
}

DeepCopyable::~DeepCopyable(){
	// Remove myself from the set before destruction
	s_changedDeepCopyables.erase(this);
}

void DeepCopyable::mark(){
	// Add myself to the set
	s_changedDeepCopyables.insert(this);
}

void DeepCopyable::makeDeepCopies(){
	for (std::set<DeepCopyable*>::iterator i = s_changedDeepCopyables.begin();
	        i != s_changedDeepCopyables.end(); ++i) {
		(*i)->deepCopyResources();
	}
}

void DeepCopyable::clearMarks(){
	s_changedDeepCopyables.clear();
}

}
}
