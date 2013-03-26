/*
 * HWOncillaBackend.h
 */

#ifndef LIBONCILLA_HW_HWONCILLABACKEND_H_
#define LIBONCILLA_HW_HWONCILLABACKEND_H_


#include <liboncilla/utils/OncillaBackend.h>


class HWOncillaBackend : public OncillaBackend {
public :
	HWOncillaBackend();
	virtual ~HWOncillaBackend();

	virtual boost::shared_ptr<rci::oncilla::Synchronizer> CreateSynchronizer();
	virtual boost::shared_ptr<rci::oncilla::L0> CreateL0(rci::oncilla::Leg,
	                                                     const std::string&);
	virtual boost::shared_ptr<rci::oncilla::L1L2> CreateL1(rci::oncilla::Leg,
	                                                       const std::string&);
	virtual boost::shared_ptr<rci::oncilla::L1L2> CreateL2(rci::oncilla::Leg,
	                                                       const std::string &);
	virtual boost::shared_ptr<rci::oncilla::L3> CreateL3(rci::oncilla::Leg,
	                                                     const std::string &);
	virtual boost::shared_ptr<rci::oncilla::Trunk> CreateTrunk();

	virtual boost::shared_ptr<rci::oncilla::SupervisorTrunk> CreateSupervisorTrunk();

	virtual boost::shared_ptr<rci::oncilla::SupervisorWorld> CreateSupervisorWorld();

	virtual boost::shared_ptr<rci::oncilla::SupervisorL4> CreateSupervisorL4(rci::oncilla::Leg,
	                                                                         const std::string&);

};


#endif //LIBONCILLA_HW_HWONCILLABACKEND_H_
