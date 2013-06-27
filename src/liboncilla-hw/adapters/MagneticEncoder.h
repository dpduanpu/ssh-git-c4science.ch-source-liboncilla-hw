#pragma once




#include <rci/Sensing.h>



namespace liboncilla  {

namespace hw {
	
	template <class Node>
	class MagneticEncoder : public rx::Input {
	public :
		typedef typename Node::Ptr NodePtr;
		MagneticEncoder(const NodePtr & node,
		                double ratio,
		                unsigned int index)
			: d_node(node)
			, d_ratio(ratio)
			, d_index(index){
			///\todo adds check for index and dimension. Not possible
			/// with rci now [upstream needed]
		}
		virtual ~MagneticEncoder(){}

		void SendToProcess(double value){
			value = value * d_ratio / 4096.0 * 2.0 * M_PI;
			d_node->getJointPosition()->setFromRad(d_index,value);
			d_node->mark();
		}

	private :
		NodePtr d_node;
		double d_ratio;
		unsigned int d_index;
	};


} /* namespace hw */

} /* namespace liboncilla */
