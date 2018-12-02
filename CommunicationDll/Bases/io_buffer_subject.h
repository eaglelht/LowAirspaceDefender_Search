#ifndef IO_BUFFER_SUBJECT_H_
#define IO_BUFFER_SUBJECT_H_
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/lock_types.hpp>
#include "mutex_set.h"
#include "io_buffer_observer.h"

class io_buffer_subject
	: public mutex_set
	<boost::weak_ptr<io_buffer_observer>>
{
	virtual void attach_i_io_buffer_observer(
		boost::weak_ptr<io_buffer_observer> observer)
	{
		__super::insert(observer);
	}
	virtual void detach_i_io_buffer_observer(
		boost::weak_ptr<io_buffer_observer> observer)
	{
		__super::erase(observer);
	}
	virtual void detach_all_i_io_buffer_observers()
	{
		__super::clear();
	}
	virtual void notify_in_buffer(unsigned char* buffer, int size, void* ptr_arg = 0)
	{
		boost::shared_lock<boost::shared_mutex>
			read_lock(mutex_t_);
		std::set<boost::weak_ptr<io_buffer_observer>>::iterator itor =
			set_t_.begin();
		for (; itor != set_t_.end(); ++itor)
		{
			if (itor->expired() == false)
			{
				(itor->lock())->process_in_buffer(buffer, size, ptr_arg);
			}
		}
	}
	virtual void notify_out_buffer(unsigned char* buffer, int size, void* ptr_arg = 0)
	{
		boost::shared_lock<boost::shared_mutex>
			read_lock(mutex_t_);
		std::set<boost::weak_ptr<io_buffer_observer>>::iterator itor =
			set_t_.begin();
		for (; itor != set_t_.end(); ++itor)
		{
			if (itor->expired() == false)
			{
				(itor->lock())->process_out_buffer(buffer, size, ptr_arg);
			}
		}
	}

};

#endif // IO_BUFFER_SUBJECT_H_
