#include "stdafx.h"
#include "test_message_subject.h"
#include <boost/bind/bind.hpp>
#include "Bases/message.h"


test_message_subject::test_message_subject(std::string info)
	: running_(true)
	, info_(info)
{
	boost::thread thr(
		boost::bind(&test_message_subject::fn_create_message,
			this)
	);
	thread_.swap(thr);
	thr.detach();
}


test_message_subject::~test_message_subject()
{
	running_ = false;
	thread_.join();
}

void test_message_subject::fn_create_message()
{
	while (running_)
	{
		boost::shared_ptr<message> sptr_msg
			= boost::make_shared<message>();
		sptr_msg->main_type_ = message::E_MMT_Command;
		sptr_msg->sub_type_ = message::E_MST_Servo;
		sptr_msg->information_ = info_;
	
		notify(sptr_msg, 0);

		boost::this_thread::sleep(boost::posix_time::millisec(20));
	}
}
