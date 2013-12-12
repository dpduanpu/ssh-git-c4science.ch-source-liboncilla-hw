/*
 * \file NativeHolder.h
 *
 *  \date Jun 27, 2013
 *  \author Alexandre Tuleu
 */

#pragma once

#include <tr1/memory>

#include <native/task.h>
#include <native/mutex.h>
#include <native/event.h>

#include <iostream>
#include <cstring>
#include <cstdlib>

namespace liboncilla {
namespace hw {

template<typename T>
class NativeDestructor {
public:
	typedef int (*DeleterFptr)(T *);

	NativeDestructor(DeleterFptr fptr, const char * name) 
		: d_fptr(fptr)
		, d_name(name) {
	};

	void operator()(T * p) {
		int res = (*d_fptr)(p);
		if (res) {
			std::cerr << "Got error on xenomai " << d_name << " " << (void*) p
			          << " destruction. Code " << -res << " : " << strerror(-res)
			          << std::endl;
#ifndef NDEBUG
			exit(255);
#endif
		}
		delete p;
	};

private:
	DeleterFptr d_fptr;
	const char * d_name;
};

template<typename T>
class NativeHolder : public std::tr1::shared_ptr<T> {
public:
	typedef typename NativeDestructor<T>::DeleterFptr DeleterFptr;
	NativeHolder() : std::tr1::shared_ptr<T>() {
	}

	NativeHolder(T *t, DeleterFptr p, const char * name = "unnammed") 
		: std::tr1::shared_ptr<T>(t, NativeDestructor<T>(p, name)) {
	}

};

#define SPECIALIZE_FOR(rt_object, deleter ) \
	template <> \
	class NativeHolder<rt_object> : public std::tr1::shared_ptr<rt_object> { \
	public : \
		NativeHolder() : std::tr1::shared_ptr<rt_object>(){} \
		NativeHolder(rt_object * t) \
			: std::tr1::shared_ptr<rt_object>(t,NativeDestructor<rt_object>( deleter, #rt_object)) {}\
	}

SPECIALIZE_FOR(RT_TASK, rt_task_delete);
SPECIALIZE_FOR(RT_MUTEX,rt_mutex_delete);
SPECIALIZE_FOR(RT_EVENT, rt_event_delete);

#undef SPECIALIZE_FOR

} /* namespace hw */
} /* namespace liboncilla */
