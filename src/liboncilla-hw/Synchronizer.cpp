#include "Synchronizer.h"

#include "Common.h"

using namespace liboncilla::hw;

Synchronizer::Synchronizer()
	: rci::oncilla::Synchronizer("Oncilla HW Synchronizer"){
}

Synchronizer::~Synchronizer(){
}

::rtio::Input::Ptr Synchronizer::GetInput(const std::string & name) {
	return d_manager.GetInput(name);
}

::rtio::Output::Ptr Synchronizer::GetOutput(const std::string & name) {
	return d_manager.GetOutput(name);
}

void Synchronizer::calibrateIfNeeded(){
	NOT_IMPLEMENTED_STUB(calibrateIfNeeded,Synchronizer);
}

double Synchronizer::lastProcessTimeStep() const {
	NOT_IMPLEMENTED_STUB(lastProcessTimeStep,Synchronizer);
}


void Synchronizer::ProcessAsyncPrimpl() {
	NOT_IMPLEMENTED_STUB(processAsync,Synchronizer);
}

void Synchronizer::WaitForProcessAsyncPrimpl(){
	NOT_IMPLEMENTED_STUB(waitForProcessAsync,Synchronizer);
}
