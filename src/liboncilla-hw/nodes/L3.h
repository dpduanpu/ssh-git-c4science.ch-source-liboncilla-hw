#pragma once

#include <liboncilla/nodes/L3.h>

#include <liboncilla-hw/Common.h>

namespace liboncilla {
namespace hw {

class L3 : public rci::oncilla::L3 {
public:
	typedef boost::shared_ptr< ::liboncilla::hw::L3> Ptr;

	L3(const std::string & name);
	virtual ~L3();
	
	void queueToNodeJointPosition(int16_t magneticEncoderVal,
	                              int16_t magneticEncoderStatus);

};

} /* namespace hw */
} /* namespace liboncilla */
