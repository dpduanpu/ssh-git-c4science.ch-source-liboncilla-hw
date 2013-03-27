#pragma once

#include <liboncilla/nodes/L3.h>

#include "liboncilla-hw/Common.h"
#include "liboncilla-hw/nodes/DeepCopyable.h"

namespace liboncilla {
namespace hw {

class L3 : public rci::oncilla::L3 ,
           public DeepCopyable {
public:
	typedef boost::shared_ptr< ::liboncilla::hw::L3> Ptr;
	
	L3(const std::string & name);
	virtual ~L3();

	virtual rci::JointAnglesPtr getJointPosition() const;

	virtual void deepCopyResources();

};

} /* namespace hw */
} /* namespace liboncilla */
