#pragma once


#include <liboncilla/Synchronizer.h>



namespace liboncilla {

namespace hw {


	class Synchronizer : public rci::oncilla::Synchronizer {
	public :
		typedef boost::shared_ptr< ::liboncilla::hw::Synchronizer > Ptr;

		Synchronizer();
		virtual ~Synchronizer();
	
		
		double lastProcessTimeStep() const;

		void    calibrateIfNeeded();

		static void SigHandler(int c);

	protected:
		void ProcessAsyncPrimpl();
		void WaitForProcessAsyncPrimpl();

	private :
		void Init();

		void InitRT();
		void InitModules();


		const static double TIMESTEP = 0.01;

		unsigned long d_overruns;

		bool d_firstStepped;

	};


} /* namespace hw */

} /* namespace liboncilla */
