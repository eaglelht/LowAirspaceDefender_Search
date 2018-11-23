#include "stdafx.h"
#include "test_message_observer.h"
#include "Bases/message.h"


test_message_observer::test_message_observer()
{
}


test_message_observer::~test_message_observer()
{
}

void test_message_observer::process_message(boost::shared_ptr<message> subject, void* ptr_arg)
{
	if (subject == 0)
	{
		return;
	}
	//OutputDebugString(subject->information_.c_str());
}
