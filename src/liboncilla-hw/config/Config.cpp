/*
 * \file Config.cpp
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#include "Config.h"

namespace liboncilla {
namespace hw {

Config::Config() 
	: options::Config("liboncilla-hw")
	, d_shMain(Section())
	, d_shMotors(Section()) {
}

Config::~Config() {
}

} /* namespace hw */
} /* namespace liboncilla */
