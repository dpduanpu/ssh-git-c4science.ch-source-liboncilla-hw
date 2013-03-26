#include "Trunk.h"

namespace liboncilla {
namespace hw {

Trunk::Trunk(){
}

Trunk::~Trunk(){
}

TranslationalAccelerationPtr Trunk::getTranslationalAcceleration() const{
	NOT_IMPLEMENTED_STUB(Trunk, getTranslationalAcceleration);
}

OrientationPtr Trunk::getOrientation() const{
	NOT_IMPLEMENTED_STUB(Trunk, getOrientation);
}

void Trunk::deepCopyResources(){
	acctranslational = TranslationalAccelerationPtr(
	        new TranslationalAcceleration(acctranslational->_values));
	orientation = OrientationPtr(new Orientation(orientation->_values));
}

} /* namespace hw */
} /* namespace liboncilla */
