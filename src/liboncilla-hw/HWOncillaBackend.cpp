#include "HWOncillaBackend.h"

#include <liboncilla/nodes/Nodes.h>
#include <liboncilla/nodes/SupervisorNodes.h>

#include "liboncilla-hw/nodes/Nodes.h"
#include "liboncilla-hw/adapters/Adapters.h"

//export the plugin
BIOROB_CPP_EXPORT_PLUGIN(OncillaBackend, HWOncillaBackend);

namespace ro = rci::oncilla;
namespace loh = liboncilla::hw;

const char * HWOncillaBackend::L1_POSITION_SUFFIX = "_L1";
const char * HWOncillaBackend::L2_POSITION_SUFFIX = "_L2";

const char * HWOncillaBackend::ME1_SUFFIX = "_ME1";
const char * HWOncillaBackend::ME2_SUFFIX = "_ME2";
const char * HWOncillaBackend::ME3_SUFFIX = "_ME3";


HWOncillaBackend::HWOncillaBackend()
	: d_synchronizer(new loh::Synchronizer()){
}

HWOncillaBackend::~HWOncillaBackend(){
}

boost::shared_ptr<rci::oncilla::Synchronizer> HWOncillaBackend::CreateSynchronizer(){
	return boost::static_pointer_cast<ro::Synchronizer,loh::Synchronizer>(d_synchronizer);
}

boost::shared_ptr<rci::oncilla::L0> 
HWOncillaBackend::CreateL0(rci::oncilla::Leg leg ,
                           const std::string& name){

	ro::L0::Ptr node(new loh::L0(name));

	// TODO: Force sensing adapter

	rx::Output::Ptr servo(new loh::Servo(node));
	
	rtio::Output::Ptr o = d_synchronizer->GetOutput(legPrefix(leg));
	if(!o) {
		UNAVAILABLE_INTERFACE("Could not find suitable servo device for " + legPrefix(leg) + " node",
		                      HWOncillaBackend,
		                      CreateL0);
	}
   
	o->SetProcessOutput(servo);

	return node;
}

boost::shared_ptr<rci::oncilla::L1L2> 
HWOncillaBackend::CreateL1(rci::oncilla::Leg leg , 
                           const std::string& name){

	loh::L1L2::Ptr node(new loh::L1L2(*d_synchronizer, name));

	bool isReversed = NOT_REVERSED; // TODO: Set depending on left/right/whatever
	unsigned int range; // Hardware range of the brushless motors - TODO: Which unit?
	if (isForeLeg(leg)) {
		range = HW_RANGE_L1_FORE;
	} else {
		range = HW_RANGE_L1_HIND;
	}

	std::tr1::shared_ptr<loh::BrushlessMotorPosition> 
		l1Adapter(new loh::BrushlessMotorPosition(node,
		                                          range, 
		                                          isReversed, 
		                                          NOT_NORMALIZED));

	rx::Input::Ptr inMeAdapter(new loh::MagneticEncoder<loh::L1L2>(node,
	                                                               ENCODER_RATIO_L1, 
	                                                               ro::L1L2::MagneticEncoderIndex));
	
	rtio::Input::Ptr l1In = d_synchronizer->GetInput(legPrefix(leg) + L1_POSITION_SUFFIX);
	rtio::Output::Ptr l1Out = d_synchronizer->GetOutput(legPrefix(leg) + L1_POSITION_SUFFIX);
	
	rtio::Input::Ptr me1In = d_synchronizer->GetInput(legPrefix(leg) + ME1_SUFFIX);

	if( ! (l1In && l1Out) ){
		UNAVAILABLE_INTERFACE("Could not find suitable brushless motor device for L1 " + legPrefix(leg) + " node",
		                      HWONcillaBackend,
		                      CreateL1);
	}

	if( !me1In) {
		UNAVAILABLE_INTERFACE("Could not find suitable Magnetic Encoder device for L1 " + legPrefix(leg) + " node",
		                      HWOncillaBackend,
		                      CreateL1);
	}
	
	l1In->SetProcessInput(std::tr1::static_pointer_cast<rx::Input,loh::BrushlessMotorPosition>(l1Adapter));
	l1Out->SetProcessOutput(std::tr1::static_pointer_cast<rx::Output,loh::BrushlessMotorPosition>(l1Adapter));

	me1In->SetProcessInput(inMeAdapter);

	return boost::static_pointer_cast<ro::L1L2,loh::L1L2>(node);
}

boost::shared_ptr<rci::oncilla::L1L2> 
HWOncillaBackend::CreateL2(rci::oncilla::Leg leg ,
                           const std::string& name){

	loh::L1L2::Ptr node(new loh::L1L2(*d_synchronizer, name));

	bool isReversed = NOT_REVERSED; // TODO: Set depending on left/right/whatever
	unsigned int range; // Hardware range of the brushless motors - TODO: Which unit?
	if (isForeLeg(leg)) {
		range = HW_RANGE_L2_FORE;
	} else {
		range = HW_RANGE_L2_HIND;
	}

	std::tr1::shared_ptr<loh::BrushlessMotorPosition> 
		l2Adapter(new loh::BrushlessMotorPosition(node,
		                                          range, 
		                                          isReversed, 
		                                          NOT_NORMALIZED));
	
	rx::Input::Ptr inMeAdapter(new loh::MagneticEncoder<loh::L1L2>(node,
	                                                               ENCODER_RATIO_L2, 
	                                                               ro::L1L2::MagneticEncoderIndex));
	
	rtio::Input::Ptr l2In = d_synchronizer->GetInput(legPrefix(leg) + L2_POSITION_SUFFIX);
	rtio::Output::Ptr l2Out = d_synchronizer->GetOutput(legPrefix(leg) + L2_POSITION_SUFFIX);
	
	rtio::Input::Ptr me2In = d_synchronizer->GetInput(legPrefix(leg) + ME2_SUFFIX);

	if( ! (l2In && l2Out) ){
		UNAVAILABLE_INTERFACE("Could not find suitable brushless motor device for L2 " + legPrefix(leg) + " node",
		                      HWONcillaBackend,
		                      CreateL2);
	}

	if( ! me2In) {
		UNAVAILABLE_INTERFACE("Could not find suitable Magnetic Encoder device for L2 " + legPrefix(leg) + " node",
		                      HWOncillaBackend,
		                      CreateL2);
	}
	
	l2In->SetProcessInput(std::tr1::static_pointer_cast<rx::Input,loh::BrushlessMotorPosition>(l2Adapter));
	l2Out->SetProcessOutput(std::tr1::static_pointer_cast<rx::Output,loh::BrushlessMotorPosition>(l2Adapter));

	me2In->SetProcessInput(inMeAdapter);

	return boost::static_pointer_cast<ro::L1L2,loh::L1L2>(node);
}

boost::shared_ptr<rci::oncilla::L3> 
HWOncillaBackend::CreateL3(rci::oncilla::Leg leg ,
                           const std::string& name){

	loh::L3::Ptr node(new loh::L3(name));

	rx::Input::Ptr adapter(new loh::MagneticEncoder<loh::L3>(node,
	                                                         ENCODER_RATIO_L3, 
	                                                         0));
	rtio::Input::Ptr me = d_synchronizer->GetInput(legPrefix(leg) + ME3_SUFFIX);

	if (! me ) {
		UNAVAILABLE_INTERFACE("Could not find suitable magnetic encoder device for L3 " + legPrefix(leg) + " node",
		                      HWOncillaBackend,
		                      CreateL3);
	}

	me->SetProcessInput(adapter);
return boost::static_pointer_cast<ro::L3,loh::L3>(node);
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
