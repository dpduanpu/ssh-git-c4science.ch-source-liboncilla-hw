/*
 * \file Config.h
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#pragma once

#include <liboncilla-hw/config/MainSection.h>
#include <liboncilla-hw/config/MotorSection.h>


namespace liboncilla {
namespace hw {

class Config : public options::Config {
public:
	Config();
	virtual ~Config();

	BCPP_ADD_SUBSECTION(MainSection,Main,"main","Main settings for liboncilla-hw")
	BCPP_ADD_SUBSECTION(MotorSection,Motor,"motor","Section for motor configuration")


};

} /* namespace hw */
} /* namespace liboncilla */
