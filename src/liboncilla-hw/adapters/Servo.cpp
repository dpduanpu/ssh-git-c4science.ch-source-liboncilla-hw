#include "Servo.h"

using namespace liboncilla::hw;

Servo::Servo(const liboncilla::hw::L0::Ptr & node)
	: d_node(node) {
}


Servo::~Servo(){}

double Servo::ReadFromProcess(){
	return d_node->getLastPositionCommand()->rad(0);
}
