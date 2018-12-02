#ifndef PROTOCOL_SUBJECT_H_
#define PROTOCOL_SUBJECT_H_
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/lock_types.hpp>
#include "mutex_set.h"
#include "protocol_observer.h"

class protocol_subject
	: public mutex_set
	<boost::weak_ptr<protocol_observer>>
{
public:
	protocol_subject() {}
	virtual ~protocol_subject() {}
public:
	virtual void attach_i_protocol_observer(
		boost::weak_ptr<protocol_observer> observer)
	{
		__super::insert(observer);
	}
	virtual void detach_i_protocol_observer(
		boost::weak_ptr<protocol_observer> observer)
	{
		__super::erase(observer);
	}
	virtual void detach_all_i_protocol_observers()
	{
		__super::clear();
	}
	virtual void notify_protocol_out(boost::shared_ptr<i_protocol_out> subject
		, void* ptr_arg = 0)
	{
		boost::shared_lock<boost::shared_mutex>
			read_lock(mutex_t_);
		std::set<boost::weak_ptr<protocol_observer>>::iterator itor =
			set_t_.begin();
		for (; itor != set_t_.end(); ++itor)
		{
			if (itor->expired() == false)
			{
				(itor->lock())->process_protocol_out(subject, ptr_arg);
			}
		}
	}
	virtual void notify_protocol_in(boost::shared_ptr<i_protocol_in> subject
		, void* ptr_arg = 0)
	{
		boost::shared_lock<boost::shared_mutex>
			read_lock(mutex_t_);
		std::set<boost::weak_ptr<protocol_observer>>::iterator itor =
			set_t_.begin();
		for (; itor != set_t_.end(); ++itor)
		{
			if (itor->expired() == false)
			{
				(itor->lock())->process_protocol_in(subject, ptr_arg);
			}
		}
	}

};

#endif // PROTOCOL_SUBJECT_H_
