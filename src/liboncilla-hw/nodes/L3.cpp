#include "L3.h"

#include "liboncilla-hw/Common.h"

using namespace liboncilla::hw;
using namespace rci;

L3::L3(const std::string & name) 
	: rci::oncilla::L3(name) {
}

L3::~L3() {
}

void L3::queueToNodeJointPosition(int16_t magneticEncoderVal,
                                  int16_t magneticEncoderStatus) {
	/// \todo check status and report if necessary, if interface available
	/// \todo do conversion, following parameters could easily be extracted at
	/// 	calibration time.
	
	double sign = 1, offset = 0; /// \todo BUT THIS SHOULD BE CHANGED
	
	double userUpJointAngle = sign * (magneticEncoderVal + offset) / 4096.0
	                               * (2.0 * M_PI);
	
	_latestJointPosition->setFromRad(0, userUpJointAngle);
}
