/*
 * \file NativeError.cpp
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#include "NativeError.h"

#include <sstream>
#include <cstring>

namespace liboncilla {
namespace hw {

NativeError::NativeError(const std::string& whatStr, int error) {
	std::ostringstream os;
	os << whatStr << ". Code " << error << " : " << strerror(error);
	d_what = os.str();
}

NativeError::~NativeError() throw () {
}

const char* NativeError::what() const throw () {
	return d_what.c_str();
}

} /* namespace hw */
} /* namespace liboncilla */
