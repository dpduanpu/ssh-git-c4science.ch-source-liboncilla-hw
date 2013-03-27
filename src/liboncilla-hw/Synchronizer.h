#pragma once

#include <robo-xeno/module/ModuleLoader.h>

#include <liboncilla/Synchronizer.h>
#include <robo-xeno/rtio/Manager.h>


namespace liboncilla {

namespace hw {


	class Synchronizer : public rci::oncilla::Synchronizer {
	public :
		typedef boost::shared_ptr< ::liboncilla::hw::Synchronizer > Ptr;

		Synchronizer();
		virtual ~Synchronizer();
	
		::rtio::Input::Ptr GetInput(const std::string & name);
		::rtio::Output::Ptr GetOutput(const std::string & name);
		
		double lastProcessTimeStep() const;

		void    calibrateIfNeeded();

		static void SigHandler(int c);

	protected:
		void ProcessAsyncPrimpl();
		void WaitForProcessAsyncPrimpl();

	private :
		typedef std::tr1::shared_ptr<rtio::Manager> ManagerPtr;
		typedef std::tr1::shared_ptr<rx::ModuleLoader> ModuleLoaderPtr;
		void Init();

		void InitRT();
		void InitModules();

		static ManagerPtr s_manager;
		//member because holds the module, so we should avoid dlclose
		//our object.
		static ModuleLoaderPtr s_loader;

		const static double TIMESTEP = 0.01;

		unsigned long d_overruns;

		bool d_firstStepped;

	};


} /* namespace hw */

} /* namespace liboncilla */
