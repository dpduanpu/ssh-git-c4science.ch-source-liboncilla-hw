#pragma once


#include <liboncilla/Synchronizer.h>
#include <liboncilla-hw/xenomai-utils/Utils.h>
#include <liboncilla-hw/nodes/Nodes.h>

#include "SBCPQueue.h"
#include "RBIOQueue.h"


class HWOncillaBackend;

namespace liboncilla {

namespace hw {
	class MainSection;
	class Queue;

	class Synchronizer : public rci::oncilla::Synchronizer {
	public :
		typedef boost::shared_ptr< ::liboncilla::hw::Synchronizer > Ptr;

		Synchronizer(const MainSection & config);
		virtual ~Synchronizer();
	
		
		double lastProcessTimeStep() const;

		void    calibrateIfNeeded();

		virtual bool start();
		virtual bool stop();

		static void SigHandler(int c);



		class RegistrationAccessor {
			static void RegisterTrunk(liboncilla::hw::Synchronizer & itself,
			                          const Trunk::Ptr & node);
			static void RegisterL0(liboncilla::hw::Synchronizer & itself,
			                       rci::oncilla::Leg l,
			                       const L0::Ptr & node);
			static void RegisterL1(liboncilla::hw::Synchronizer & itself,
			                       rci::oncilla::Leg l,
			                       const L1L2::Ptr & node);
			static void RegisterL2(liboncilla::hw::Synchronizer & itself,
			                       rci::oncilla::Leg l,
			                       const L1L2::Ptr & node);
			static void RegisterL3(liboncilla::hw::Synchronizer & itself,
			                       rci::oncilla::Leg l,
			                       const L3::Ptr & node);
			friend class ::HWOncillaBackend;
		};
	protected:
		void ProcessAsyncPrimpl();
		void WaitForProcessAsyncPrimpl();

	private :
		typedef std::vector<Queue*> ListOfQueue;
		void Init();
		void CheckConfig(const MainSection & config);
		void InitRT();
		void InitModules();

		void RegisterTrunk(const Trunk::Ptr & node);
		void RegisterL0(rci::oncilla::Leg l,const L0::Ptr & node);
		void RegisterL1(rci::oncilla::Leg l,const L1L2::Ptr & node);
		void RegisterL2(rci::oncilla::Leg l,const L1L2::Ptr & node);
		void RegisterL3(rci::oncilla::Leg l,const L3::Ptr & node);

		void WakeIdleQueues();
		void FetchIdleQueues();
		bool IsFinished(const Queue & q);


		unsigned long d_overruns;

		bool d_firstStepped;

		double d_timestep;
		unsigned int d_priority;

		NativeHolder<RT_EVENT> d_event;
		unsigned long d_idleQueueMask;
		SBCPQueue  d_sbcpQueue;
		RBIOQueue  d_rbioQueue;
		ListOfQueue d_queues;

	};


	inline void
	Synchronizer::RegistrationAccessor::RegisterTrunk(Synchronizer & itself,
	                                                  const Trunk::Ptr & node){
		itself.RegisterTrunk(node);
	}

	inline void
	Synchronizer::RegistrationAccessor::RegisterL0(Synchronizer & itself,
		                                               rci::oncilla::Leg l,
		                                               const L0::Ptr & node){
		itself.RegisterL0(l,node);
	}

	inline void
	Synchronizer::RegistrationAccessor::RegisterL1(Synchronizer & itself,
	                                               rci::oncilla::Leg l,
	                                               const L1L2::Ptr & node){
		itself.RegisterL1(l,node);
	}

	inline void
	Synchronizer::RegistrationAccessor::RegisterL2(Synchronizer & itself,
	                                               rci::oncilla::Leg l,
	                                               const L1L2::Ptr & node) {
		itself.RegisterL2(l,node);
	}

	inline void
	Synchronizer::RegistrationAccessor::RegisterL3(Synchronizer & itself,
	                                               rci::oncilla::Leg l,
	                                               const L3::Ptr & node) {
		itself.RegisterL3(l,node);
	}



} /* namespace hw */

} /* namespace liboncilla */
