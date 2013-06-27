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

namespace liboncilla {
namespace hw {


template <typename T>
class NativeDestructor {
public :
	typedef int (*DeleterFptr)(T *);

	NativeDestructor(DeleterFptr fptr) : d_fptr(fptr){};

	void operator()(T * p){
		(*d_fptr)(p);
		delete p;
	}

private :
	DeleterFptr d_fptr;
};

template <typename T>
class NativeHolder : public std::tr1::shared_ptr<T> {
public :
	typedef typename NativeDestructor<T>::DeleterFptr DeleterFptr;
	NativeHolder() : std::tr1::shared_ptr<T>(){}

	NativeHolder(T *t, DeleterFptr p)
		: std::tr1::shared_ptr<T>(t,NativeDestructor<T>(p)) {}

};


template <>
class NativeHolder<RT_TASK> : public std::tr1::shared_ptr<RT_TASK> {
public :
	NativeHolder() : std::tr1::shared_ptr<RT_TASK>(){}

	NativeHolder(RT_TASK *t)
		: std::tr1::shared_ptr<RT_TASK>(t,NativeDestructor<RT_TASK>(rt_task_delete)) {}

};

template <>
class NativeHolder<RT_MUTEX> : public std::tr1::shared_ptr<RT_MUTEX> {
public :
	NativeHolder() : std::tr1::shared_ptr<RT_MUTEX>(){}

	NativeHolder(RT_MUTEX *t)
		: std::tr1::shared_ptr<RT_MUTEX>(t,NativeDestructor<RT_MUTEX>(rt_mutex_delete)) {}

};

template <>
class NativeHolder<RT_EVENT> : public std::tr1::shared_ptr<RT_EVENT> {
public :
	NativeHolder() : std::tr1::shared_ptr<RT_EVENT>(){}

	NativeHolder(RT_EVENT *t)
		: std::tr1::shared_ptr<RT_EVENT>(t,NativeDestructor<RT_EVENT>(rt_event_delete)) {}

};
} /* namespace hw */
} /* namespace liboncilla */
