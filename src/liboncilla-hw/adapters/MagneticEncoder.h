#pragma once



#include <liboncilla-hw/adapters/rx-io.h>
#include <rci/Sensing.h>



namespace liboncilla  {

namespace hw {

	class MagneticEncoder : public rx::Input {
	public :
		typedef boost::shared_ptr<rci::PositionSensing> PositionSensingPtr;
		MagneticEncoder(const PositionSensingPtr & node,
		                double ratio,
		                unsigned int index);
		virtual ~MagneticEncoder();

		void SendToProcess(double value);

	private :
		PositionSensingPtr d_node;
		double d_ratio;
		unsigned int d_index;
	};


} /* namespace hw */

} /* namespace liboncilla */
