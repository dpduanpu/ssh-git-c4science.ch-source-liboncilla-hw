/*
 * \file NativeError.h
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#pragma once
#include <exception>
#include <string>

namespace liboncilla {
namespace hw {

class NativeError {
public:
	NativeError(const std::string & whatStr, int error);
	virtual ~NativeError() throw ();

	virtual const char * what() const throw ();

private:
	std::string d_what;
};

} /* namespace hw */
} /* namespace liboncilla */
