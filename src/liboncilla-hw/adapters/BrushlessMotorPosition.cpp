#include "BrushlessMotorPosition.h"

using namespace liboncilla::hw;

BrushlessMotorPosition::BrushlessMotorPosition(const ::liboncilla::hw::L1L2::Ptr & node,
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

	double actualValue = (d_reversed ? d_range - value : value ) / 4096.0;
	if (!d_normalized){
		actualValue *= 2 * M_PI;
	}

	d_node->unsafeUpdateJointPosition(L1L2::MotorAxisIndex,actualValue);

	d_node->mark();
	
}

double BrushlessMotorPosition::ReadFromProcess(){
	double nodeValue = d_node->getLastPositionCommand()->rad(0);

	if(!d_normalized) {
		nodeValue /= 2 * M_PI;
	}

	return (d_reversed ? d_range - 4096.0 * nodeValue : 4096.0 * nodeValue);
}
