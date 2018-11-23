#pragma once
#include <string>
#include <boost/thread.hpp>
#include "Bases/message_subject.h"

class test_message_subject
	: public message_subject
{
	std::string info_;
	bool running_;
	boost::thread thread_;
public:
	test_message_subject(std::string info);
	~test_message_subject();
protected:
	void fn_create_message();
};

