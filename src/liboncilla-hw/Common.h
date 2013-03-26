#ifndef LIBONCILLA_HW_COMMON_H_
#define LIBONCILLA_HW_COMMON_H_


#include <liboncilla/exceptions/NotImplementedError.h>



#define NOT_IMPLEMENTED_STUB(className,methodName) do{\
	throw rci::oncilla::NotImplementedError("liboncilla-hw",#methodName,#className);\
}while(0)




#endif //LIBONCILLA_HW_COMMON_H_
