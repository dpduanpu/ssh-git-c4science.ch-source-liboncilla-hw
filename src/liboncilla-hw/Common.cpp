/*
 * \file Common.cpp
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#include "Common.h"
#include <vector>
#include <sstream>

const std::string & LegPrefix(rci::oncilla::Leg leg) {
	static std::vector<std::string> s_legNames;

	if (s_legNames.empty()) {
		s_legNames.resize(4, "");
		s_legNames[rci::oncilla::LEFT_FORE] = "left_fore";
		s_legNames[rci::oncilla::RIGHT_FORE] = "right_fore";
		s_legNames[rci::oncilla::LEFT_HIND] = "left_hind";
		s_legNames[rci::oncilla::RIGHT_HIND] = "right_hind";
	}

	if (leg >= s_legNames.size()) {
		std::ostringstream os;
		os << "Leg (" << leg << ") is out of range";
		throw std::out_of_range(os.str());
	}

	return s_legNames[leg];
}
