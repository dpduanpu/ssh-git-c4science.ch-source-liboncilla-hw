#pragma once

#include <liboncilla/common.h>

#include <liboncilla/exceptions/NotImplementedError.h>

#define NOT_IMPLEMENTED_STUB(className,methodName) do{\
	throw rci::oncilla::NotImplementedError("liboncilla-hw",#methodName,#className); \
}while(0)

#define UNAVAILABLE_INTERFACE(reason,className,methodName) do{\
	throw rci::oncilla::UnavailableInterfaceError(reason,"liboncilla-hw",#methodName,#className); \
}while(0)

const std::string & LegPrefix(rci::oncilla::Leg leg);
