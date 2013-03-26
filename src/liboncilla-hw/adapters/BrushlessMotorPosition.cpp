#include "BrushlessMotorPosition.h"

using namespace liboncilla::hw;

BrushlessMotorPosition::BrushlessMotorPosition(const rci::oncilla::L1L2::Ptr & node,
                                               unsigned int hardwareRange,
                                               bool isReversed,
                                               bool isNormalized)
	: d_node(node)
	, d_range(hardwareRange)
	, d_reversed(isReversed)
	, d_normalized(isNormalized){
}

BrushlessMotorPosition::~BrushlessMotorPosition(){
}

void BrushlessMotorPosition::SendToProcess(double value){

	double actualValue = (d_reversed ? d_range - value : value ) / (d_range);
	if (!d_normalized){
		actualValue *= 2 * M_PI;
	}

	//\todo do the actual update and marking
	
}

double BrushlessMotorPosition::ReadFromProcess(){
	double nodeValue = d_node->getLastPositionCommand()->rad(0);

	if(!d_normalized) {
		nodeValue /= 2 * M_PI;
	}

	return d_range * (d_reversed ? 1.0 - nodeValue : nodeValue);
}
