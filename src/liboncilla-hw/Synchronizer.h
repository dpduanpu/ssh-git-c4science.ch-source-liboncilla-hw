#pragma once


#include <liboncilla/Synchronizer.h>
#include <robo-xeno/rtio/Manager.h>


namespace liboncilla {

namespace hw {


	class Synchronizer : public rci::oncilla::Synchronizer {
	public :
		Synchronizer();
		virtual ~Synchronizer();
	
		::rtio::Input::Ptr GetInput(const std::string & name);
		::rtio::Output::Ptr GetOutput(const std::string & name);
		
		double lastProcessTimeStep() const;

	protected:
		void ProcessAsyncPrimpl();
		void WaitForProcessAsyncPrimpl();

	private :

		rtio::Manager d_manager;
	
	};


} /* namespace hw */

} /* namespace liboncilla */
