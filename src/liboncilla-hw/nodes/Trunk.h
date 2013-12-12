#pragma once

#include <liboncilla/nodes/Trunk.h>

#include <liboncilla-hw/Common.h>

namespace liboncilla {
namespace hw {

using namespace rci;

class Trunk : public rci::oncilla::Trunk {
public:
	typedef boost::shared_ptr<Trunk> Ptr;
	Trunk();
	virtual ~Trunk();
	
	virtual TranslationalAccelerationPtr getTranslationalAcceleration() const;
	virtual OrientationPtr getOrientation() const;

};

} /* namespace hw */
} /* namespace liboncilla */

