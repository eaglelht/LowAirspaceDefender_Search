#include "stdafx.h"
#include "base_sensor_protocol.h"
#include "../../Json/reader.h"
#include "../../Json/value.h"
#include <../../../packages/boost.1.68.0.0/lib/native/include/boost/lexical_cast.hpp>

base_ir_sensor_protocol_out::base_ir_sensor_protocol_out()
{

}

bool base_ir_sensor_protocol_out::from_message(std::string msg_str)
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
	if (str_command.compare("nuc") == 0)
	{
		activate_nuc();
		return true;
	}
	else if (str_command.compare("synchronization") == 0)
	{
		sync_model_ = static_cast<E_IR_Sensor_Sync_Model>(boost::lexical_cast<int>
			(info["param_1"].asString()));
		set_synchronization_model(sync_model_);
		return true;
	}
	else
	{
		return false;
	}
}
