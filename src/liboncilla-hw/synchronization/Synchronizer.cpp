#include "Synchronizer.h"


#include <sys/mman.h>
#include <signal.h>
#include <errno.h>


#include <liboncilla-hw/nodes/DeepCopyable.h>
#include <liboncilla-hw/xenomai-utils/Utils.h>

#include <liboncilla-hw/config/Config.h>

namespace liboncilla{
namespace hw {

Synchronizer::Synchronizer(const MainSection & config)
	: rci::oncilla::Synchronizer("Oncilla HW Synchronizer")
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

void Synchronizer::CheckConfig(const MainSection& config){
	std::ostringstream os;
	if(config.Timestep() < 2){
		os << "Unsupported timestep of " << config.Timestep()
		   << " ms. It should be >= 2ms";
		throw std::runtime_error(os.str());
	}

	if(config.Priority() > 99) {
		os << "Unsupported priorityof " << config.Priority() << " only value between 0 and 99 are supported";
		throw std::runtime_error(os.str());
	}
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


bool Synchronizer::start() {
	d_firstStepped = false;

	RT_EVENT * e = new RT_EVENT();
	xeno_call(rt_event_create,e,Queue::EventName,0,EV_PRIO);
	d_event = NativeHolder<RT_EVENT>(e);

	for(ListOfQueue::const_iterator q = d_queues.begin();
	    q != d_queues.end();
	    ++q){
		(*q)->StartTask();
	}
	d_idleQueueMask = Queue::AllQueueMask();


	bool res = rci::oncilla::Synchronizer::start();

	xeno_call(rt_task_set_periodic,NULL,TM_NOW,d_timestep * 1e9);

	return res;
}


bool Synchronizer::stop() {
	for(ListOfQueue::const_iterator q = d_queues.begin();
	    q != d_queues.end();
	    ++q){
		(*q)->StopTask();
	}
	d_event = NativeHolder<RT_EVENT>();

	return rci::Synchronizer::stop();
}

double Synchronizer::lastProcessTimeStep() const {
	if (d_firstStepped) {
		return 0.0;
	}
	return d_timestep * (d_overruns + 1);
}


void Synchronizer::ProcessAsyncPrimpl() {
	for(ListOfQueue::const_iterator q = d_queues.begin();
	    q != d_queues.end();
	    ++q){
		if(IsFinished(**q)){
			(*q)->DownstreamData();
		}
	}
	WakeIdleQueues();
	d_firstStepped = true;
}

void Synchronizer::WaitForProcessAsyncPrimpl(){
	int res;

	unsigned int maxTrials(20);

	while(true) {
		res = rt_task_wait_period(&d_overruns);
		if (res != 0 && res != -ETIMEDOUT ) {
			xeno_throw_error_from(rt_task_wait_period,res);
		}

		FetchIdleQueues();
		//we ensure that all sbcp communication are done !
		if(IsFinished(d_sbcpQueue)) {
			break;
		}

		--maxTrials;
		if(maxTrials == 0) {
			std::ostringstream os;
			os << "SBCP communication did not finished after " << 20
			   << " timesteps. We abort everything";
			throw std::runtime_error(os.str());
		}
	}

	for(ListOfQueue::const_iterator q = d_queues.begin();
	    q != d_queues.end();
	    ++q){
		if(IsFinished(**q)) {
			(*q)->UpstreamData();
		}
	}

}

void Synchronizer::RegisterTrunk(const Trunk::Ptr& node){
	/// \todo implement
}

void Synchronizer::RegisterL0(rci::oncilla::Leg l , const L0::Ptr& node){
	d_rbioQueue.RegisterL0(l,node);
	d_sbcpQueue.RegisterL0(l,node);
}

void Synchronizer::RegisterL1(rci::oncilla::Leg l , const L1L2::Ptr& node){
	d_sbcpQueue.RegisterL1(l,node);
}

void Synchronizer::RegisterL2(rci::oncilla::Leg l , const L1L2::Ptr& node){
	d_sbcpQueue.RegisterL2(l,node);
}

void Synchronizer::RegisterL3(rci::oncilla::Leg l , const L3::Ptr& node){
	d_sbcpQueue.RegisterL3(l,node);
}

void Synchronizer::WakeIdleQueues(){
	xeno_call(rt_event_signal,d_event.get(),d_idleQueueMask);
}

void Synchronizer::FetchIdleQueues(){
	xeno_call(rt_event_wait,d_event.get(),0,&d_idleQueueMask,EV_ANY,TM_NONBLOCK);
	d_idleQueueMask ^= Queue::AllQueueMask();
}

bool Synchronizer::IsFinished(const Queue& q){
	return q.Mask() & d_idleQueueMask;
}

} //namespace hw
} //namespace liboncilla
