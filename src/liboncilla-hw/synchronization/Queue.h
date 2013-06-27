/*
 * \file Queue.h
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */


#include <liboncilla-hw/xenomai-utils/Utils.h>


namespace liboncilla {
namespace hw {


/**
 * Represent a thread to perform IO, RAII style.
 */
class Queue {
public:
	Queue(unsigned int priority,bool preferNonRt);
	virtual ~Queue();

	/**
	 * Sends the data from liboncilla-hw to the hardware
	 */
	virtual void DownStreamData() = 0;
	/**
	 * Sends the data from hardware to liboncilla-hw
	 */
	virtual void UpstreamData()   = 0;

	/**
	 * Callback function that should perform the IO.
	 */
	virtual void PerformIO()      = 0;

	/**
	 * Callback to init things
	 */
	virtual void InitializeIO()   = 0;

	void StartTask();
	void StopTask();

	const static char * EventName;

private :
	static void TaskEntryPoint(void * itself);

	void Loop();

	static unsigned int s_nbQueues;
	const static unsigned int MaxNbQueues;

	NativeHolder<RT_TASK> d_task;
	const unsigned int d_id;
	const bool d_preferNonRt;
};

} /* namespace hw */
} /* namespace liboncilla */
