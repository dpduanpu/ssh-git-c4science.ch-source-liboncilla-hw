#include "Synchronizer.h"


#include <sys/mman.h>
#include <signal.h>
#include <errno.h>

#include "Common.h"
#include <liboncilla-hw/nodes/DeepCopyable.h>
#include <liboncilla-hw/xenomai-utils/Utils.h>




using namespace liboncilla::hw;

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
		throw NativeError("Error while locking memory",res);
	}

	if (signal(SIGINT,&Synchronizer::SigHandler) == SIG_ERR ) {
		throw NativeError("Could not install handler", errno);
	}
	
	
	xeno_call(rt_task_shadow,NULL,NULL,50, T_FPU);
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
	return TIMESTEP * (d_overruns + 1);
}


void Synchronizer::ProcessAsyncPrimpl() {
	if(!d_firstStepped){
		xeno_call(rt_task_set_periodic,NULL,TM_NOW,1e9 * TIMESTEP);
	}

}

void Synchronizer::WaitForProcessAsyncPrimpl(){
	int res;

	res = rt_task_wait_period(&d_overruns);
	if (res != 0 && res != -ETIMEDOUT ) {
		xeno_throw_error_from(rt_task_wait_period,res);
	}
}
