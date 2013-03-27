#include "Synchronizer.h"
#include "Common.h"
#include <sys/mman.h>
#include <signal.h>
#include <robo-xeno/NativeError.h>
#include <liboncilla-hw/nodes/DeepCopyable.h>

using namespace liboncilla::hw;

std::tr1::shared_ptr<rtio::Manager> Synchronizer::s_manager;
std::tr1::shared_ptr<rx::ModuleLoader> Synchronizer::s_loader;

Synchronizer::Synchronizer()
	: rci::oncilla::Synchronizer("Oncilla HW Synchronizer")
	, d_firstStepped(false){	
	Init();
}

void Synchronizer::Init(){
	InitRT();
	InitModules();
}

void Synchronizer::InitRT() {
	int res;
	if (  ( res = mlockall(MCL_CURRENT | MCL_FUTURE)) != 0 ){
		throw rx::NativeError("Could not lock memory : ",-res);
	}

	signal(SIGINT,&Synchronizer::SigHandler);
	
	if ( ( res = rt_task_shadow(NULL,NULL,50, T_FPU) ) != 0 ) {
		throw rx::NativeError("Could not go into Realtime context : ", -res);
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

void Synchronizer::calibrateIfNeeded() {

}

double Synchronizer::lastProcessTimeStep() const {
	if (!d_firstStepped) {
		return 0.0;
	}
	return TIMESTEP * (d_overruns + 1);
}


void Synchronizer::ProcessAsyncPrimpl() {
	if(!d_firstStepped){
		int res = rt_task_set_periodic(NULL,TM_NOW,1e9 * TIMESTEP);

		if ( res != 0 ) {
			throw rx::NativeError("Could not set real time timer : ", -res);
		}

		s_manager->InitializeQueues();
	}

	s_manager->StartIdleQueues();
}

void Synchronizer::WaitForProcessAsyncPrimpl(){
	int res;

	res = rt_task_wait_period(&d_overruns);
	if (res != 0 && res != -ETIMEDOUT ) {
		throw rx::NativeError("Could not rt_task_wait_period : " ,-res);
	}
   
	//now some time have been spent, we just update the needed nodes

	const rtio::Manager::ListOfQueue & idles = s_manager->IdleQueues();
	for(rtio::Manager::ListOfQueue::const_iterator q = idles.begin();
	    q != idles.end();
	    ++q){
		const rtio::Queue::OutputByName & outputs = (*q)->Outputs();
		for(rtio::Queue::OutputByName::const_iterator o = outputs.begin();
		    o != outputs.end();
		    ++o){
			if(o->second->IsMapped()){
				o->second->ReadFromProcess();
			}
		}
		const rtio::Queue::InputByName & inputs = (*q)->Inputs();
		for(rtio::Queue::InputByName::const_iterator i = inputs.begin();
		    i != inputs.end();
		    ++i){
			if(i->second->IsMapped()){
				i->second->SendToProcess();
			}
		}
	}


	//now we make deep copies

	DeepCopyable::makeDeepCopies();
	DeepCopyable::clearMarks();
	

}
