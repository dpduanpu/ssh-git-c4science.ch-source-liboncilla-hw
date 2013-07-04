#pragma once

#include <liboncilla/nodes/L3.h>

#include "liboncilla-hw/Common.h"
#include "liboncilla-hw/nodes/DeepCopyable.h"

namespace liboncilla {
namespace hw {

class L3 : public rci::oncilla::L3{
public:
	typedef boost::shared_ptr< ::liboncilla::hw::L3> Ptr;
	
	L3(const std::string & name);
	virtual ~L3();

	void queueToNodeJointPosition(int magneticEncoderVal, int magneticEncoderStatus);

};

} /* namespace hw */
} /* namespace liboncilla */
