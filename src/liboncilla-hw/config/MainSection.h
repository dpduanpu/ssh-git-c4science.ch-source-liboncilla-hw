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

	BCPP_ADD_OPTION(MainPriority,
	                "priority",
	                unsigned int,
	                70,
	                "xenomai priority of the main task, between 0 (lowest) and 99 (highest) = is the same priority than a standard linux thread.");

	BCPP_ADD_OPTION(SBCPPriority,
	                "sbcp-priority",
	                unsigned int,
	                60,
	                "xenomai priority of the sbcp I/O task, between 0 (lowest) and 99 (highest) = is the same priority than a standard linux thread.");

	BCPP_ADD_OPTION(RBIOPriority,
	                "rbio-priority",
	                unsigned int,
	                60,
	                "xenomai priority of the rbio I/O task, between 0 (lowest) and 99 (highest) = is the same priority than a standard linux thread.");


	BCPP_ADD_OPTION(Timestep,
	                "timestep",
	                unsigned int,
	                10,
	                "desired timestep desired to run the task in millisecond. Notice that oncilla will behave strangly if you do not meet this deadline.");
};



} /* namespace hw */
} /* namespace liboncilla */
