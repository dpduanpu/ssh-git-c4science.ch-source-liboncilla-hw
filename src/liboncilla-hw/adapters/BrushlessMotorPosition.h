#pragma once

#include <liboncilla-hw/adapters/rx-io.h>
#include <liboncilla/nodes/L1L2.h>


namespace liboncilla{

namespace hw {

	class BrushlessMotorPosition : public rx::Input,
	                               public rx::Output {
	public :

		BrushlessMotorPosition(const rci::oncilla::L1L2::Ptr & node,
		                       unsigned int hardwareRange,
		                       bool isReversed,
		                       bool isNormalized);
		
		virtual ~BrushlessMotorPosition();
		
		virtual void SendToProcess(double value);

		virtual double ReadFromProcess();

	private :
		//\todo replace by liboncilla::hw one
		rci::oncilla::L1L2::Ptr d_node;
		double d_range;
		bool d_reversed;
		bool d_normalized;
	};

} /* namespace liboncilla */

} /* namespace liboncilla */
