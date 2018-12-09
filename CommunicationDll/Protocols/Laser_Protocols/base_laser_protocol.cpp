#include "stdafx.h"
#include "base_laser_protocol.h"
#include <boost/lexical_cast.hpp>
#include "../../Json/reader.h"
#include "../../Json/value.h"

bool base_laser_protocol_out::from_message(std::string msg_str)
{
	Json::Reader reader;
	Json::Value info;
	if (reader.parse(msg_str, info) == false)
	{
		return false;
	}
	if (info["command"].empty() == true)
	{
		return false;
	}
	std::string str_command = boost::lexical_cast<std::string>
		(info["command"].asString());
	if (str_command.compare("shoot_once") == 0)
	{
		shoot_once();
		return true;
	}
	else if (str_command.compare("power_on") == 0)
	{
		power_on();
		return true;
	}
	else if (str_command.compare("power_off") == 0)
	{
		power_off();
		return true;
	}
	else
	{
		return false;
	}
}
