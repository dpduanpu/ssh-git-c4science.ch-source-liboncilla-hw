/*
 * \file Utils.h
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#pragma once

#include "NativeError.h"
#include "NativeHolder.h"

#define xeno_throw_error_from(fct_name,errorCode) do{\
	throw ::liboncilla::hw::NativeError(std::string("Xenomai Error when executing '") + #fct_name +"'()",-(errorCode)); \
}while(0)

#define xeno_call(fct_name,...) do{\
	int res = fct_name(__VA_ARGS__);\
	if(res != 0) { \
		xeno_throw_error_from(fct_name,res);\
	} \
}while(0)

