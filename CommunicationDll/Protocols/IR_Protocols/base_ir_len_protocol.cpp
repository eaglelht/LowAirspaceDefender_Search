#include "stdafx.h"
#include "base_ir_len_protocol.h"
#include <boost/lexical_cast.hpp>
#include "../../Json/reader.h"
#include "../../Json/value.h"

base_ir_len_protocol_out::base_ir_len_protocol_out()
{
	focus_ = E_IR_LFS_Stop;
	mirror_ = E_IR_Len_MS_Stop;
}

bool base_ir_len_protocol_out::from_message(std::string msg_str)
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
	if (str_command.compare("focus_near"))
	{
		focus_near();
		return true;
	}
	else if (str_command.compare("focus_far"))
	{
		focus_far();
		return true;
	}
	else if (str_command.compare("focus_stop"))
	{
		focus_stop();
		return true;
	}
	else if (str_command.compare("mirror_start"))
	{
		mirror_rotating_start();
		return true;
	}
	else if (str_command.compare("mirror_stop"))
	{
		mirror_rotating_stop();
		return true;
	}
	else
	{
		return false;
	}
}
