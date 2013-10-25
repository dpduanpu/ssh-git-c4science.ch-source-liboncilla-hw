/*
 * \file Queue.h
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#pragma once

#include <liboncilla-hw/xenomai-utils/Utils.h>

namespace liboncilla {
namespace hw {

/**
 * Represent a thread to perform IO, RAII style.
 */
class Queue {
public:
	const static char * EventName;
	static unsigned long AllQueueMask();
	/**
	 * /param preferNonRt: use callbacks which are not supposed to run on the realtime Kernel (default: true)
	 */
	Queue(unsigned int priority, bool preferNonRt);
	virtual ~Queue();

	/**
	 * Sends the data from liboncilla-hw to the hardware
	 */
	virtual void DownstreamData() = 0;

	/**
	 * Sends the data from hardware to liboncilla-hw
	 */
	virtual void UpstreamData() = 0;

	/**
	 * Callback function that should perform the IO.
	 */
	virtual void PerformIO() = 0;

	/**
	 * Callback to initialize things
	 */
	virtual void InitializeIO() = 0;

	/**
	 * Callback to de-initialize things
	 */
	virtual void DeinitializeIO() {};

	void StartTask();

	void StopTask();

	unsigned long Mask() const;

private:
	static void TaskEntryPoint(void * itself);

	void Loop();

	static unsigned int s_nbQueues;
	const static unsigned int MaxNbQueues;

	NativeHolder<RT_TASK> d_task;
	const unsigned int d_id;
	const bool d_preferNonRt;
	const unsigned int d_priority;
};

inline unsigned long Queue::Mask() const {
	return 1 << d_id;
}

inline unsigned long Queue::AllQueueMask() {
	if (s_nbQueues == MaxNbQueues) {
		return -1;
	}
	return (1 << s_nbQueues) - 1;
}
} /* namespace hw */
} /* namespace liboncilla */
