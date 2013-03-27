#include "HWOncillaBackend.h"

#include <liboncilla/nodes/Nodes.h>
#include <liboncilla/nodes/SupervisorNodes.h>

#include "liboncilla-hw/nodes/Nodes.h"
#include "liboncilla-hw/adapters/Adapters.h"

//export the plugin
BIOROB_CPP_EXPORT_PLUGIN(OncillaBackend, HWOncillaBackend);

namespace ro = rci::oncilla;
namespace loh = liboncilla::hw;

HWOncillaBackend::HWOncillaBackend()
		: synchronizer(){
}

HWOncillaBackend::~HWOncillaBackend(){
}

boost::shared_ptr<rci::oncilla::Synchronizer> HWOncillaBackend::CreateSynchronizer(){
	// TODO: Create synchronizer when it`s no longer pure virtual
	synchronizer = ro::Synchronizer::Ptr();

	return synchronizer;
}

boost::shared_ptr<rci::oncilla::L0> HWOncillaBackend::CreateL0(
        rci::oncilla::Leg leg , const std::string& name){

	ro::L0::Ptr n(new loh::L0(name));

	// TODO: Force sensing adapter

	loh::MagneticEncoder *encoder = new loh::MagneticEncoder(
	        boost::dynamic_pointer_cast<rci::PositionSensing>(n),
	        ENCODER_RATIO_L1, ro::L1L2::MotorAxisIndex);

	// TODO: Glue inputs and outputs to the adapter

	return n;
}

boost::shared_ptr<rci::oncilla::L1L2> HWOncillaBackend::CreateL1(
        rci::oncilla::Leg leg , const std::string& name){

	ro::L1L2::Ptr n(new loh::L1L2(*synchronizer, name));

	bool isReversed = NOT_REVERSED; // TODO: Set depending on left/right/whatever
	unsigned int range; // Hardware range of the brushless motors - TODO: Which unit?
	if (isForeLeg(leg)) {
		range = HW_RANGE_L1_FORE;
	} else {
		range = HW_RANGE_L1_HIND;
	}
	loh::BrushlessMotorPosition *motor = new loh::BrushlessMotorPosition(
	        n, range, isReversed, NOT_NORMALIZED);

	loh::MagneticEncoder *encoder = new loh::MagneticEncoder(
	        boost::dynamic_pointer_cast<rci::PositionSensing>(n),
	        ENCODER_RATIO_L1, ro::L1L2::MagneticEncoderIndex);

	// TODO: Glue inputs and outputs to the adapters

	return n;
}

boost::shared_ptr<rci::oncilla::L1L2> HWOncillaBackend::CreateL2(
        rci::oncilla::Leg leg , const std::string& name){

	ro::L1L2::Ptr n(new loh::L1L2(*synchronizer, name));

	bool isReversed = NOT_REVERSED; // TODO: Set depending on left/right/whatever
	unsigned int range; // Hardware range of the brushless motors - TODO: Which unit?
	if (isForeLeg(leg)) {
		range = HW_RANGE_L1_FORE;
	} else {
		range = HW_RANGE_L1_HIND;
	}
	loh::BrushlessMotorPosition *motor = new loh::BrushlessMotorPosition(
	        n, range, isReversed, NORMALIZED);

	loh::MagneticEncoder *encoder = new loh::MagneticEncoder(
	        boost::dynamic_pointer_cast<rci::PositionSensing>(n),
	        ENCODER_RATIO_L2, ro::L1L2::MagneticEncoderIndex);

	// TODO: Glue inputs and outputs to the adapters

	return n;
}

boost::shared_ptr<rci::oncilla::L3> HWOncillaBackend::CreateL3(
        rci::oncilla::Leg leg , const std::string& name){

	ro::L3::Ptr n(new loh::L3(name));

	loh::MagneticEncoder *encoder = new loh::MagneticEncoder(
	        boost::static_pointer_cast<rci::PositionSensing>(n),
	        ENCODER_RATIO_L3, ro::L1L2::MagneticEncoderIndex);

	// TODO: Glue inputs and outputs to the adapter

	return n;
}

boost::shared_ptr<rci::oncilla::Trunk> HWOncillaBackend::CreateTrunk(){
	return ro::Trunk::Ptr(new loh::Trunk());
}

boost::shared_ptr<rci::oncilla::SupervisorTrunk> HWOncillaBackend::CreateSupervisorTrunk(){
	return ro::SupervisorTrunk::Ptr();
}

boost::shared_ptr<rci::oncilla::SupervisorWorld> HWOncillaBackend::CreateSupervisorWorld(){
	return ro::SupervisorWorld::Ptr();
}

boost::shared_ptr<rci::oncilla::SupervisorL4> HWOncillaBackend::CreateSupervisorL4(
        rci::oncilla::Leg , const std::string&){
	return ro::SupervisorL4::Ptr();
}

bool HWOncillaBackend::isLeftLeg(rci::oncilla::Leg l){
	return (l == ro::LEFT_FORE || l == ro::LEFT_HIND);
}

bool HWOncillaBackend::isForeLeg(rci::oncilla::Leg l){
	return (l == ro::LEFT_FORE || l == ro::RIGHT_FORE);
}
