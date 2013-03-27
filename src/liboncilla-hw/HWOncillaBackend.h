#pragma once

#include <liboncilla/utils/OncillaBackend.h>
#include <liboncilla/Synchronizer.h>

/**
 * Liboncilla backend for the Oncilla hardware, providing all resource node
 * implementations connected to the hardware drivers.
 */
class HWOncillaBackend : public OncillaBackend {
public:
	HWOncillaBackend();
	virtual ~HWOncillaBackend();

	virtual boost::shared_ptr<rci::oncilla::Synchronizer> CreateSynchronizer();
	virtual boost::shared_ptr<rci::oncilla::L0> CreateL0(rci::oncilla::Leg ,
	                                                     const std::string&);
	virtual boost::shared_ptr<rci::oncilla::L1L2> CreateL1(rci::oncilla::Leg ,
	                                                       const std::string&);
	virtual boost::shared_ptr<rci::oncilla::L1L2> CreateL2(rci::oncilla::Leg ,
	                                                       const std::string &);
	virtual boost::shared_ptr<rci::oncilla::L3> CreateL3(rci::oncilla::Leg ,
	                                                     const std::string &);
	virtual boost::shared_ptr<rci::oncilla::Trunk> CreateTrunk();

	virtual boost::shared_ptr<rci::oncilla::SupervisorTrunk> CreateSupervisorTrunk();

	virtual boost::shared_ptr<rci::oncilla::SupervisorWorld> CreateSupervisorWorld();

	virtual boost::shared_ptr<rci::oncilla::SupervisorL4> CreateSupervisorL4(
	        rci::oncilla::Leg , const std::string&);

protected:
	::rci::oncilla::Synchronizer::Ptr synchronizer;

private:
	static const bool NORMALIZED = true;
	static const bool NOT_NORMALIZED = false;

	static const bool REVERSED = true;
	static const bool NOT_REVERSED = false;

	static const unsigned int HW_RANGE_L1_FORE = 1850; // TODO: CHECK
	static const unsigned int HW_RANGE_L1_HIND = 2200; // TODO: CHECK

	static const double ENCODER_RATIO_L1 = 1.0 / 4096.0; // TODO: CHECK
	static const double ENCODER_RATIO_L2 = 1.0 / 4096.0; // TODO: CHECK
	static const double ENCODER_RATIO_L3 = 1.0 / 4096.0; // TODO: CHECK

	bool isLeftLeg(rci::oncilla::Leg);

	bool isForeLeg(rci::oncilla::Leg);
};
