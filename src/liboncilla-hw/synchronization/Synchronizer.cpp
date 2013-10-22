#include "Synchronizer.h"
#include <sys/mman.h>
#include <signal.h>
#include <errno.h>

#include <liboncilla-hw/xenomai-utils/Utils.h>

#include <liboncilla-hw/config/Config.h>

namespace liboncilla {
namespace hw {

Synchronizer::Synchronizer(const Config & config) :
		rci::oncilla::Synchronizer("Oncilla HW Synchronizer"), d_timestep(
				config.Main().Timestep() * 1.0e-3), d_priority(
				config.Main().MainPriority()), d_sbcpQueue(config), d_rbioQueue(
				config.Main().RBIOPriority()) {
	CheckMainConfig(config.Main());
	Init();
	//TODO bad bad pratice new queue should be added automatically to this list.
	d_queues.push_back(&d_sbcpQueue);
	d_queues.push_back(&d_rbioQueue);
}

void Synchronizer::Init() {

	InitRT();
	InitModules();
}

void Synchronizer::InitRT() {

	xeno_call(rt_task_shadow, NULL, NULL, d_priority, T_FPU);
}

void Synchronizer::CheckPriority(unsigned int p, const std::string & name) {
	if (p > 99) {
		std::ostringstream os;
		os << "Unsupported " << name << " of " << p
				<< " only value between 0 and 99 are supported";
		throw std::runtime_error(os.str());
	}
}

void Synchronizer::CheckMainConfig(const MainSection& config) {
	std::ostringstream os;
	if (config.Timestep() < 2) {
		os << "Unsupported timestep of " << config.Timestep()
				<< " ms. It should be >= 2ms";
		throw std::runtime_error(os.str());
	}

	CheckPriority(config.MainPriority(), "main priority");
	CheckPriority(config.SBCPPriority(), "sbcp priority");
	CheckPriority(config.RBIOPriority(), "rbio priority");
}

void Synchronizer::InitModules() {

}

Synchronizer::~Synchronizer() {
}

void Synchronizer::calibrateIfNeeded() {

}

bool Synchronizer::start() {
	d_firstStepped = false;

	RT_EVENT * e = new RT_EVENT();
	xeno_call(rt_event_create, e, Queue::EventName, 0, EV_PRIO);
	d_event = NativeHolder<RT_EVENT>(e);

	for (ListOfQueue::const_iterator q = d_queues.begin(); q != d_queues.end();
			++q) {
		(*q)->StartTask();
	}
	d_idleQueueMask = Queue::AllQueueMask();

	bool res = rci::oncilla::Synchronizer::start();

	xeno_call(rt_task_set_periodic, NULL, TM_NOW, d_timestep * 1e9);

	return res;
}

bool Synchronizer::stop() {
	for (ListOfQueue::const_iterator q = d_queues.begin(); q != d_queues.end();
			++q) {
		(*q)->StopTask();
	}
	d_event = NativeHolder<RT_EVENT>();

	return rci::Synchronizer::stop();
}

double Synchronizer::lastProcessTimeStep() const {
	if (!d_firstStepped) {
		return 0.0;
	}
	return d_timestep * (d_overruns + 1);
}

void Synchronizer::ProcessAsyncPrimpl() {
	for (ListOfQueue::const_iterator q = d_queues.begin(); q != d_queues.end();
			++q) {
		if (IsFinished(**q)) {
			(*q)->DownstreamData();
		}
	}
	WakeIdleQueues();
	d_firstStepped = true;
}

void Synchronizer::WaitForProcessAsyncPrimpl() {
	int res;

	unsigned int maxTrials(20);

	while (true) {
		res = rt_task_wait_period(&d_overruns);
		if (res != 0 && res != -ETIMEDOUT) {
			xeno_throw_error_from(rt_task_wait_period, res);
		}

		FetchIdleQueues();
		//we ensure that all sbcp communication are done !
		if (IsFinished(d_sbcpQueue)) {
			break;
		}

		--maxTrials;
		if (maxTrials == 0) {
			std::ostringstream os;
			os << "SBCP communication did not finished after " << maxTrials
					<< " timesteps. We abort everything";
			throw std::runtime_error(os.str());
		}
	}

	for (ListOfQueue::const_iterator q = d_queues.begin(); q != d_queues.end();
			++q) {
		if (IsFinished(**q)) {
			(*q)->UpstreamData();
		}
	}

}

void Synchronizer::RegisterTrunk(const Trunk::Ptr& node) {
	/// \todo implement
}

void Synchronizer::RegisterL0(rci::oncilla::Leg l, const L0::Ptr& node) {
	d_rbioQueue.RegisterL0(l, node);
	d_sbcpQueue.RegisterL0(l, node);
}

void Synchronizer::RegisterL1(rci::oncilla::Leg l, const L1L2::Ptr& node) {
	if (node == NULL) {
		// TODO: Logging/Warning
		std::cout << "Ignoring L1 node of leg " << l << ". Is it connected?"
				<< std::endl;
		return; // Node is missing - board might be disconnected
	}
	d_sbcpQueue.RegisterL1(l, node);
}

void Synchronizer::RegisterL2(rci::oncilla::Leg l, const L1L2::Ptr& node) {
	if (node == NULL) {
		// TODO: Logging/Warning
		std::cout << "Ignoring L2 node of leg " << l << ". Is it connected?"
				<< std::endl;
		return; // Node is missing - board might be disconnected
	}
	d_sbcpQueue.RegisterL2(l, node);
}

void Synchronizer::RegisterL3(rci::oncilla::Leg l, const L3::Ptr& node) {
	if (node == NULL) {
		// TODO: Logging/Warning
		std::cout << "Ignoring L3 node of leg " << l << ". Is it connected?"
				<< std::endl;
		return; // Node is missing - board might be disconnected
	}
	d_sbcpQueue.RegisterL3(l, node);
}

void Synchronizer::WakeIdleQueues() {
	xeno_call(rt_event_signal, d_event.get(), d_idleQueueMask);
}

void Synchronizer::FetchIdleQueues() {
	xeno_call(rt_event_wait,
			d_event.get(), 0, &d_idleQueueMask, EV_ANY, TM_NONBLOCK);
	d_idleQueueMask ^= Queue::AllQueueMask();
	std::cout << "d_idleQueueMask: " << d_idleQueueMask << std::endl;
}

bool Synchronizer::IsFinished(const Queue& q) {
	return q.Mask() & d_idleQueueMask;
}

} //namespace hw
} //namespace liboncilla

