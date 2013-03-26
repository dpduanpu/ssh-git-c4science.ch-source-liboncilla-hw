#pragma once

#include <liboncilla/exceptions/NotImplementedError.h>

#define NOT_IMPLEMENTED_STUB(className,methodName) do{\
    throw rci::oncilla::NotImplementedError("liboncilla-hw",#methodName,#className);\
}while(0)

#define UNAVAILABLE_INTERFACE(className,methodName) do{\
    throw rci::oncilla::UnavailableInterfaceError("liboncilla-hw",#methodName,#className);\
}while(0)
