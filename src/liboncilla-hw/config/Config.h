/*
 * \file Config.h
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#pragma once

#include "MainSection.h"

namespace liboncilla {
namespace hw {

class Config : public options::Config {
public:
	Config();
	virtual ~Config();

	BCPP_ADD_SUBSECTION(MainSection,Main,"main","Main settings for liboncilla-hw")
};

} /* namespace hw */
} /* namespace liboncilla */
