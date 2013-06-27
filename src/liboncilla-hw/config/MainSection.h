/*
 * \file MainSection.h
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#pragma once

#include <biorob-cpp/options/Config.h>


namespace liboncilla {
namespace hw {

class MainSection : public options::ConfigSection {
public:
	MainSection(const options::Section::Ptr & base);
	virtual ~MainSection();

	BCPP_ADD_OPTION(Priority,
	                "priority",
	                unsigned int,
	                50,
	                "xenomai priority of the task, between 0 (lowest) and 99 (highest) = is the same priority than a standard linux thread.");
	BCPP_ADD_OPTION(Timestep,
	                "timestep",
	                unsigned int,
	                10,
	                "desired timestep desired to run the task in millisecond. Notice that oncilla will behave strangly if you do not meet this deadline.");
};



} /* namespace hw */
} /* namespace liboncilla */
