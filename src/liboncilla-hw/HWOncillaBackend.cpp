#include "HWOncillaBackend.h"

#include <sys/mman.h>

#include <liboncilla/nodes/Nodes.h>
#include <liboncilla/nodes/SupervisorNodes.h>
#include <liboncilla-hw/xenomai-utils/Utils.h>

#include "liboncilla-hw/nodes/Nodes.h"


//export the plugin
BIOROB_CPP_EXPORT_PLUGIN(OncillaBackend, HWOncillaBackend);

namespace ro = rci::oncilla;
namespace loh = liboncilla::hw;

const char * HWOncillaBackend::L1_POSITION_SUFFIX = "_L1";
const char * HWOncillaBackend::L2_POSITION_SUFFIX = "_L2";

const char * HWOncillaBackend::ME1_SUFFIX = "_ME1";
const char * HWOncillaBackend::ME2_SUFFIX = "_ME2";
const char * HWOncillaBackend::ME3_SUFFIX = "_ME3";


HWOncillaBackend::HWOncillaBackend(){
	// first thing is to lock memory as soon as possible, so In the entry point
	// of the module
	LockMemory();
	d_config.LoadAllFiles();
	d_synchronizer = loh::Synchronizer::Ptr(new loh::Synchronizer(d_config.Main()));
}


void HWOncillaBackend::LockMemory() {
	int res;
	if (  ( res = mlockall(MCL_CURRENT | MCL_FUTURE)) != 0 ){
		throw liboncilla::hw::NativeError("Error while locking memory",res);
	}
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

	return node;
}

boost::shared_ptr<rci::oncilla::L1L2> 
HWOncillaBackend::CreateL1(rci::oncilla::Leg leg , 
                           const std::string& name){

	loh::L1L2::Ptr node(new loh::L1L2(*d_synchronizer, name));

	bool isReversed = NOT_REVERSED; // TODO: Set depending on left/right/whatever
	unsigned int range; // Hardware range of the brushless motors - TODO: Which unit?
	if (IsForeLeg(leg)) {
		range = HW_RANGE_L1_FORE;
	} else {
		range = HW_RANGE_L1_HIND;
	}

	return boost::static_pointer_cast<ro::L1L2,loh::L1L2>(node);
}

boost::shared_ptr<rci::oncilla::L1L2> 
HWOncillaBackend::CreateL2(rci::oncilla::Leg leg ,
                           const std::string& name){

	loh::L1L2::Ptr node(new loh::L1L2(*d_synchronizer, name));

	bool isReversed = NOT_REVERSED; // TODO: Set depending on left/right/whatever
	unsigned int range; // Hardware range of the brushless motors - TODO: Which unit?
	if (IsForeLeg(leg)) {
		range = HW_RANGE_L2_FORE;
	} else {
		range = HW_RANGE_L2_HIND;
	}



	return boost::static_pointer_cast<ro::L1L2,loh::L1L2>(node);
}

boost::shared_ptr<rci::oncilla::L3> 
HWOncillaBackend::CreateL3(rci::oncilla::Leg leg ,
                           const std::string& name){

	loh::L3::Ptr node(new loh::L3(name));

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

bool HWOncillaBackend::IsLeftLeg(rci::oncilla::Leg l){
	return (l == ro::LEFT_FORE || l == ro::LEFT_HIND);
}

bool HWOncillaBackend::IsForeLeg(rci::oncilla::Leg l){
	return (l == ro::LEFT_FORE || l == ro::RIGHT_FORE);
}
