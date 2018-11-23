#include "stdafx.h"
#include "message_dispatching_center.h"


message_dispatching_center::message_dispatching_center()
	:messages_semaphore_(0)
{
	running_ = true;
	threads_.create_thread(
		boost::bind(&message_dispatching_center::fn_dispatch_message,
			this));
	threads_.create_thread(
		boost::bind(&message_dispatching_center::fn_dispatch_message,
			this));
}

message_dispatching_center::~message_dispatching_center()
{
	running_ = false;
	for (size_t i = 0; i < threads_.size(); ++i)
	{
		messages_semaphore_.signal();
	}	
	threads_.join_all();
}

void message_dispatching_center::register_observer(message::E_Message_Main_Type type, boost::weak_ptr<message_observer> wptr_observer)
{
	message_observers_[type].attach_i_message_observer(wptr_observer);
}

void message_dispatching_center::cancel_observer(message::E_Message_Main_Type type, boost::weak_ptr<message_observer> wptr_observer)
{
	message_observers_[type].detach_i_message_observer(wptr_observer);
}

void message_dispatching_center::process_message(boost::shared_ptr<message> subject, void* ptr_arg)
{
	if (subject == 0)
	{
		return;
	}
	messages_.push(subject);
	messages_semaphore_.signal();
}

void message_dispatching_center::dispatch_message(boost::shared_ptr<message> sptr_msg)
{
	if (sptr_msg == 0)
	{
		return;
	}
	switch (sptr_msg->main_type_)
	{
	case message::E_MMT_Command:
	{
		dispatch_command_message(sptr_msg);
		break;
	}
	case message::E_MMT_Update:
	{
		dispatch_update_message(sptr_msg);
		break;
	}
	default:
	{
		break;
	}
	}
}

void message_dispatching_center::dispatch_command_message(boost::shared_ptr<message> sptr_msg)
{
	message_observers_[message::E_MMT_Command].notify(sptr_msg, 0);
}

void message_dispatching_center::dispatch_update_message(boost::shared_ptr<message> sptr_msg)
{
	message_observers_[message::E_MMT_Update].notify(sptr_msg, 0);
}

void message_dispatching_center::fn_dispatch_message()
{
	while (running_)
	{
		messages_semaphore_.wait();
		boost::shared_ptr<message> sptr_msg
			= messages_.pop();
		if (sptr_msg)
		{
			dispatch_message(sptr_msg);
		}
	}
}
