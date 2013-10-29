#pragma once

#include <liboncilla/nodes/L0.h>

#include <liboncilla-hw/Common.h>

namespace liboncilla {
namespace hw {

class L0: public rci::oncilla::L0 {
public:
	typedef boost::shared_ptr<liboncilla::hw::L0> Ptr;
	L0(const std::string & name);
	virtual ~L0();

	void queueToNodeForces(int force1, int force2, int force3);

};

} /* namespace hw */
} /* namespace liboncilla */
