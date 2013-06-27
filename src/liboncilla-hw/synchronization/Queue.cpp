/*
 * \file Queue.cpp
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#include "Queue.h"

#include <biorob-cpp/log/Logger.h>

#include <sstream>
#include <stdexcept>

namespace liboncilla {
namespace hw {

unsigned int Queue::s_nbQueues(0);
const unsigned int Queue::MaxNbQueues(8 * sizeof(unsigned long));

Queue::Queue(unsigned int priority,bool preferNonRt)
	: d_id(++s_nbQueues)
	, d_preferNonRt(preferNonRt){
	if(s_nbQueues > MaxNbQueues) {
		std::ostringstream os;
		os << "Cannot alloacte Queue, maximum allowed : " << MaxNbQueues;
		throw std::runtime_error(os.str());
	}

	RT_TASK * t = new RT_TASK();
	xeno_call(rt_task_create,t,NULL,0,priority,0);
	d_task = NativeHolder<RT_TASK>(t);
}

Queue::~Queue(){
	StopTask();
}

void Queue::StartTask() {
	log(debug,"Starting queue",this);
	xeno_call(rt_task_start,
	          d_task.get(),
	          &Queue::TaskEntryPoint,
	          reinterpret_cast<void*>(this));
}

void Queue::TaskEntryPoint(void * itself) {
	reinterpret_cast<Queue*>(itself)->Loop();
}

void Queue::StopTask() {
	log(debug,"Stopping Queue ",this);
	d_task = NativeHolder<RT_TASK>();
}

void Queue::Loop() {
	RT_EVENT e;
	xeno_call(rt_event_bind,&e,EventName,TM_INFINITE);
	unsigned long myMask = 1 << d_id;
	unsigned long mask;

	InitializeIO();

	while(true) {
		int res = rt_event_wait(&e,myMask,&mask,EV_ANY,TM_INFINITE);

		if(res == -EIDRM ) { //the event has been deleted, we should exit
			break;
		}

		if(res) {
			xeno_throw_error_from(rt_event_wait,-res);
		}

		if(d_preferNonRt) {
			xeno_call(rt_task_set_mode,T_PRIMARY,0,NULL);
		}

		PerformIO();

	}
	rt_event_unbind(&e);
}

} /* namespace hw */
} /* namespace liboncilla */
