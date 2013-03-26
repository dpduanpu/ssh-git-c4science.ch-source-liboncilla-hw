#include "MagneticEncoder.h"




using namespace liboncilla::hw;


MagneticEncoder::MagneticEncoder(const PositionSensingPtr & node,
                                 double ratio,
                                 unsigned int index)
	: d_node(node)
	, d_ratio(ratio)
	, d_index(index){
	///\todo add check for dimension
}

MagneticEncoder::~MagneticEncoder(){
}

void MagneticEncoder::SendToProcess(double value){
	value = value * d_ratio  / 4096.0 * 2 * M_PI;
	///\todo update and marks
}
