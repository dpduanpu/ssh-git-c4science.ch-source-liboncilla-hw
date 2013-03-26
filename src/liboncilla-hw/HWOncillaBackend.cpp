/*
 * HWOncillaBackend.cpp
 */

#include "HWOncillaBackend.h"

#include <liboncilla/nodes/Nodes.h>
#include <liboncilla/nodes/SupervisorNodes.h>
#include <liboncilla/Synchronizer.h>

#include <liboncilla/nodes/Nodes.h>

//export the plugin
BIOROB_CPP_EXPORT_PLUGIN(OncillaBackend,HWOncillaBackend)

namespace ro = rci::oncilla;

HWOncillaBackend::HWOncillaBackend(){
}

HWOncillaBackend::~HWOncillaBackend(){
}

boost::shared_ptr<rci::oncilla::Synchronizer>
HWOncillaBackend::CreateSynchronizer(){
	return ro::Synchronizer::Ptr();
}

boost::shared_ptr<rci::oncilla::L0>
HWOncillaBackend::CreateL0(rci::oncilla::Leg,
                           const std::string&){
	return ro::L0::Ptr();
}

boost::shared_ptr<rci::oncilla::L1L2>
HWOncillaBackend::CreateL1(rci::oncilla::Leg,
                           const std::string&){
	return ro::L1::Ptr();
}

boost::shared_ptr<rci::oncilla::L1L2>
HWOncillaBackend::CreateL2(rci::oncilla::Leg,
                           const std::string &){
	return ro::L2::Ptr();
}


boost::shared_ptr<rci::oncilla::L3>
HWOncillaBackend::CreateL3(rci::oncilla::Leg,
                           const std::string &){
	return ro::L3::Ptr();
}


boost::shared_ptr<rci::oncilla::Trunk> HWOncillaBackend::CreateTrunk(){
	return ro::Trunk::Ptr();
}

boost::shared_ptr<rci::oncilla::SupervisorTrunk>
HWOncillaBackend::CreateSupervisorTrunk(){
	return ro::SupervisorTrunk::Ptr();
}

boost::shared_ptr<rci::oncilla::SupervisorWorld>
HWOncillaBackend::CreateSupervisorWorld(){
	return ro::SupervisorWorld::Ptr();
}

boost::shared_ptr<rci::oncilla::SupervisorL4>
HWOncillaBackend::CreateSupervisorL4(rci::oncilla::Leg,
                                     const std::string&){
	return ro::SupervisorL4::Ptr();
}
