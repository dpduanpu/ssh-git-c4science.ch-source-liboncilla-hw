#pragma once


#include <liboncilla/Synchronizer.h>

namespace liboncilla {

namespace hw {
	class MainSection;

	class Synchronizer : public rci::oncilla::Synchronizer {
	public :
		typedef boost::shared_ptr< ::liboncilla::hw::Synchronizer > Ptr;

		Synchronizer(const MainSection & config);
		virtual ~Synchronizer();
	
		
		double lastProcessTimeStep() const;

		void    calibrateIfNeeded();

		static void SigHandler(int c);

	protected:
		void ProcessAsyncPrimpl();
		void WaitForProcessAsyncPrimpl();

	private :
		void Init();
		void CheckConfig(const MainSection & config);
		void InitRT();
		void InitModules();


		unsigned long d_overruns;

		bool d_firstStepped;

		double d_timestep;
		unsigned int d_priority;
	};


} /* namespace hw */

} /* namespace liboncilla */
