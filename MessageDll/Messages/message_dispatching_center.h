#ifndef MESSAGE_DISPATCHING_CENTER_H_
#define MESSAGE_DISPATCHING_CENTER_H_
#include <list>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>
#include <boost/thread.hpp>
#include "../Bases/message.h"
#include "../Bases/semaphore.h"
#include "../Bases/message_subject.h"
#include "../Bases/message_observer_.h"
#include "../Bases/mutex_list.h"


class message_dispatching_center
	: public message_observer
{
	mutex_list<boost::shared_ptr<message>> messages_;
	semaphore messages_semaphore_;
	boost::thread_group threads_;
	bool running_;
	
	std::map<message::E_Message_Main_Type, message_subject> message_observers_;
public:
	message_dispatching_center();
	~message_dispatching_center();
	void register_observer(message::E_Message_Main_Type type, 
		boost::weak_ptr<message_observer> wptr_observer);
	void cancel_observer(message::E_Message_Main_Type type,
		boost::weak_ptr<message_observer> wptr_observer);

	virtual void process_message(boost::shared_ptr<message> subject, void* ptr_arg) override;
protected:
	void fn_dispatch_message();
	void dispatch_message(boost::shared_ptr<message> sptr_msg);
	void dispatch_command_message(boost::shared_ptr<message> sptr_msg);
	void dispatch_update_message(boost::shared_ptr<message> sptr_msg);
};

#endif // MESSAGE_DISPATCHING_CENTER_H_
