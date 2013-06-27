#include "Synchronizer.h"


#include <sys/mman.h>
#include <signal.h>
#include <errno.h>

#include "Common.h"
#include <liboncilla-hw/nodes/DeepCopyable.h>
#include <liboncilla-hw/xenomai-utils/Utils.h>

#include <liboncilla-hw/config/Config.h>

using namespace liboncilla::hw;

Synchronizer::Synchronizer(const MainSection & config)
	: rci::oncilla::Synchronizer("Oncilla HW Synchronizer")
	, d_firstStepped(false)
	, d_timestep(config.Timestep() * 1.0e-3)
	, d_priority(config.Priority()){
	CheckConfig(config);
	Init();
}

void Synchronizer::Init(){

	InitRT();
	InitModules();
}


void Synchronizer::InitRT() {


	if (signal(SIGINT,&Synchronizer::SigHandler) == SIG_ERR ) {
		throw NativeError("Could not install handler", errno);
	}
	
	
	xeno_call(rt_task_shadow,NULL,NULL,d_priority, T_FPU);
}


void Synchronizer::InitModules() {

}


Synchronizer::~Synchronizer(){
	SigHandler(0);
}

void Synchronizer::SigHandler(int ){

}

void Synchronizer::calibrateIfNeeded() {

}

double Synchronizer::lastProcessTimeStep() const {
	if (!d_firstStepped) {
		return 0.0;
	}
	return d_timestep * (d_overruns + 1);
}


void Synchronizer::ProcessAsyncPrimpl() {
	if(!d_firstStepped){
		xeno_call(rt_task_set_periodic,NULL,TM_NOW,1e9 * d_timestep);
	}

}

void Synchronizer::WaitForProcessAsyncPrimpl(){
	int res;

	res = rt_task_wait_period(&d_overruns);
	if (res != 0 && res != -ETIMEDOUT ) {
		xeno_throw_error_from(rt_task_wait_period,res);
	}
}
