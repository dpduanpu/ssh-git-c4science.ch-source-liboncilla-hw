#pragma once

#include <robo-xeno/rtio/IO.h>
#include <liboncilla-hw/nodes/L0.h>


namespace liboncilla  {

namespace hw {

	class Servo : public rx::Output {
	public :
		Servo(const liboncilla::hw::L0::Ptr & node);
		virtual ~Servo();

		double ReadFromProcess();

	private :
		liboncilla::hw::L0::Ptr d_node;
	};


} /* namespace hw */

} /* namespace liboncilla */
