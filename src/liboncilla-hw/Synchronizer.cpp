#include "Synchronizer.h"
#include "Common.h"
#include <sys/mman.h>
#include <signal.h>


using namespace liboncilla::hw;


std::tr1::shared_ptr<rtio::Manager> Synchronizer::s_manager;
std::tr1::shared_ptr<rx::ModuleLoader> Synchronizer::s_loader;

Synchronizer::Synchronizer()
	: rci::oncilla::Synchronizer("Oncilla HW Synchronizer"){	
	Init();
}

void Synchronizer::Init(){
	InitRT();
	InitModules();
}

void Synchronizer::InitRT() {
	int res;
	if (  ( res = mlockall(MCL_CURRENT | MCL_FUTURE)) != 0 ){
		throw std::runtime_error(std::string("Could not lock memory : ") + strerror(errno));
	}

	signal(SIGINT,&Synchronizer::SigHandler);
	
	if ( ( res = rt_task_shadow(NULL,NULL,50, T_FPU) ) != 0 ) {
		throw std::runtime_error(std::string("Could not go into Realtime context : ") +strerror(errno));
	}
	
}


void Synchronizer::InitModules() {
	s_manager = ManagerPtr(new rtio::Manager());
	s_loader = ModuleLoaderPtr(new rx::ModuleLoader());

	options::Section::Ptr root(new options::Section("",""));	
	rx::ModulesSection config(root);
	config.CriticalOption()->SetValue("rtio-sbcp;rtio-rbio");
	config.OptionnalOption()->SetValue("rtio-phidgets");

	s_loader->LoadModuleFromConfig(config);
	s_loader->StartQueuesOfModules(*s_manager);
}


Synchronizer::~Synchronizer(){
	SigHandler(0);
}

void Synchronizer::SigHandler(int ){
	s_manager = ManagerPtr();
	s_loader  = ModuleLoaderPtr();
}

::rtio::Input::Ptr Synchronizer::GetInput(const std::string & name) {
	return s_manager->GetInput(name);
}

::rtio::Output::Ptr Synchronizer::GetOutput(const std::string & name) {
	return s_manager->GetOutput(name);
}

void Synchronizer::calibrateIfNeeded(){
}

double Synchronizer::lastProcessTimeStep() const {
	NOT_IMPLEMENTED_STUB(lastProcessTimeStep,Synchronizer);
}


void Synchronizer::ProcessAsyncPrimpl() {
	NOT_IMPLEMENTED_STUB(processAsync,Synchronizer);
}

void Synchronizer::WaitForProcessAsyncPrimpl(){
	NOT_IMPLEMENTED_STUB(waitForProcessAsync,Synchronizer);
}
