#pragma once

#include <liboncilla/nodes/Trunk.h>

#include "liboncilla-hw/Common.h"
#include "liboncilla-hw/nodes/DeepCopyable.h"

namespace liboncilla {
namespace hw {

using namespace rci;

class Trunk : public rci::oncilla::Trunk ,
              public DeepCopyable {
public:
	typedef boost::shared_ptr<Trunk> Ptr;
	Trunk();
	virtual ~Trunk();

	virtual TranslationalAccelerationPtr getTranslationalAcceleration() const;
	virtual OrientationPtr getOrientation() const;

	virtual void deepCopyResources();
};

} /* namespace hw */
} /* namespace liboncilla */

