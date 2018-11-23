#ifndef BASE_MESSAGE_SUBJECT_H_
#define BASE_MESSAGE_SUBJECT_H_
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/lock_types.hpp>
#include "message_observer_.h"
#include "mutex_set.h"

struct message;

class message_subject
	: public mutex_set<boost::weak_ptr<message_observer>>
{
public:
	message_subject() {}
	virtual ~message_subject() {}
public:
	virtual void attach_i_message_observer(
		boost::weak_ptr<message_observer> wptr_observer)
	{
		__super::insert(wptr_observer);
	}
	virtual void detach_i_message_observer(
		boost::weak_ptr<message_observer> wptr_observer)
	{
		__super::erase(wptr_observer);
	}
	virtual void detach_all_i_message_observers()
	{
		__super::clear();
	}
	virtual void notify(boost::shared_ptr<message> sptr_subject, void* ptr_arg)
	{
		boost::shared_lock<boost::shared_mutex>
			read_lock(mutex_t_);
		std::set<boost::weak_ptr<message_observer>>::iterator itor =
			set_t_.begin();
		for (; itor != set_t_.end(); ++itor)
		{
			if (itor->expired() == false)
			{
				(itor->lock())->process_message(sptr_subject, ptr_arg);
			}
		}
	}
};

#endif // I_MESSAGE_SUBJECT_H_
