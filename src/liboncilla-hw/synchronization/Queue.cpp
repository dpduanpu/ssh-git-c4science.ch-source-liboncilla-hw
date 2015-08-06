/*
 * \file Queue.cpp
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#include "Queue.h"

#include <glog/logging.h>

#include <sstream>
#include <stdexcept>

namespace liboncilla {
namespace hw {

unsigned int Queue::s_nbQueues(0);
const unsigned int Queue::MaxNbQueues(8 * sizeof(unsigned long));
const char * Queue::EventName = "liboncilla-hw synchronization event";

Queue::Queue(unsigned int priority, bool preferNonRt) 
	: d_id(s_nbQueues++)
	, d_preferNonRt(preferNonRt)
	, d_priority(priority) {
	if (s_nbQueues > MaxNbQueues) {
		std::ostringstream os;
		os << "Cannot allocate Queue, maximum allowed : " << MaxNbQueues;
		throw std::runtime_error(os.str());
	}

}

Queue::~Queue() {
	StopTask();
}

void Queue::StartTask() {
	DLOG(INFO) << "Creating task for Queue " << this;

	RT_TASK * t = new RT_TASK();
	xeno_call(rt_task_create, t, NULL, 0, d_priority, 0);
	d_task = NativeHolder<RT_TASK>(t);

	InitializeIO();
   
	DLOG(INFO) <<  "Starting task for Queue " <<this;

	xeno_call(rt_task_start, 
	          d_task.get(), 
	          &Queue::TaskEntryPoint,
	          reinterpret_cast<void*>(this));
}

void Queue::TaskEntryPoint(void * itself) {
	reinterpret_cast<Queue*>(itself)->Loop();
}

void Queue::StopTask() {
	DLOG(INFO) << "Stopping Queue " << this;
	d_task = NativeHolder<RT_TASK>();
	DeinitializeIO();
}

void Queue::Loop() {
	RT_EVENT e;
	xeno_call(rt_event_bind, &e, EventName, TM_INFINITE);
	unsigned long mask;

	//InitializeIO();

	while (true) {
		int res = rt_event_wait(&e, Mask(), &mask, EV_ANY, TM_INFINITE);

		if (res == -EIDRM) { //the event has been deleted, we should exit
			break;
		}

		if (res) {
			xeno_throw_error_from(rt_event_wait, -res);
		}

		if (d_preferNonRt) {
			xeno_call(rt_task_set_mode, T_PRIMARY, 0, NULL);
		}

		PerformIO();
		xeno_call(rt_event_clear, &e, Mask(), &mask);
	}

	//DeinitializeIO();

	rt_event_unbind(&e);
}

} /* namespace hw */
} /* namespace liboncilla */
