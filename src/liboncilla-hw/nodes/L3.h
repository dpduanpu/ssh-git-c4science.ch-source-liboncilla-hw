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

	void updateJointPositionHardwareCoordinates(int magneticEncoderVal, int magneticEncoderStatus);

private:
	double d_hwUpJointAngle, d_userUpJointAngle; // upstream joint angles
	int d_magneticEncoderVal, d_magneticEncoderStatus;

};

} /* namespace hw */
} /* namespace liboncilla */
