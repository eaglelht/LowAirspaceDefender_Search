#pragma once
#include "Bases/message_observer_.h"

class test_message_observer
	: public message_observer
{
public:
	test_message_observer();
	~test_message_observer();

	virtual void process_message(boost::shared_ptr<message> subject, 
		void* ptr_arg) override;

};

