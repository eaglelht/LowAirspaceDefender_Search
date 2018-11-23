#include "stdafx.h"
#include <stdio.h>
#include <list>
#include <boost/smart_ptr/make_shared_object.hpp>
#include "../Messages/message_dispatching_center.h"
#include "../test_message_observer.h"
#include "../test_message_subject.h"

int main()
{
	boost::shared_ptr<message_dispatching_center> sptr_mdc
		= boost::make_shared<message_dispatching_center>();
	std::list<boost::shared_ptr<message_observer>> observers;

	for (int i = 0; i < 100; ++i)
	{
		boost::shared_ptr<test_message_observer> sptr_tmo
			= boost::make_shared<test_message_observer>();
		sptr_mdc->register_observer(message::E_MMT_Command,
			sptr_tmo->weak_from_this());
		observers.push_back(sptr_tmo);
	}

	std::list<boost::shared_ptr<message_subject>> subjects;

	for (int i = 0; i < 50; ++i)
	{
		boost::shared_ptr<test_message_subject> sptr_tms
			= boost::make_shared<test_message_subject>
			("this is test1\n");	
		sptr_tms->attach_i_message_observer(
			sptr_mdc->weak_from_this());
		subjects.push_back(sptr_tms);
	}
	getchar();
	return 0;
}