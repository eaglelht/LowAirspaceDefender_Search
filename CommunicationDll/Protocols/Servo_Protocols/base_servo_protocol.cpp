#include "stdafx.h"
#include "base_servo_protocol.h"
#include <boost/lexical_cast.hpp>
#include "../../Json/reader.h"
#include "../../Json/value.h"

std::string E_ServoWorkPatternString[] =
{
	"Î´¶¨Òå",
	"Í£Ö¹",
	"ÊÖ¿Ø",
	"Ô¤ÖÃÎ»",
	"¸ú×Ù",
	"ÉÈÉ¨",
	"ËÑË÷",
	"ËÑ×ª¸ú",
	"µ¥È¦ËÑË÷",
	"Á½È¦ËÑË÷",
	"ÈýÈ¦ËÑË÷",
	"ËÄÈ¦ËÑË÷"
};

base_servo_protocol_in::base_servo_protocol_in()
{
	has_value_changed_ = true;
	pattern_ = Servo_WP_Stop;
	azimuth_ = 0;
	elevation_ = 0;
	azimuth_speed_ = 0;
	elevation_speed_ = 0;
	azimuth_acceleration_ = 0;
	elevation_acceleration_ = 0;
}


bool base_servo_protocol_in::is_changed()
{
	if (has_value_changed_)
	{
		has_value_changed_ = false;
		return true;
	}
	else
	{
		return false;
	}
}

std::string base_servo_protocol_in::to_message()
{
	Json::Value info;
	info["pattern"] = E_ServoWorkPatternString[pattern_];
	info["azimuth"] = boost::lexical_cast<std::string>
		(get_azimuth());
	info["elevation"] = boost::lexical_cast<std::string>
		(get_elevation());
	info["azimuth_speed"] = boost::lexical_cast<std::string>
		(get_azimuth_speed());
	info["elevation_speed"] = boost::lexical_cast<std::string>
		(get_elevation_speed());
	info["azimuth_acceleration"] = boost::lexical_cast<std::string>
		(get_azimuth_acceleration());
	info["elevation_acceleration"] = boost::lexical_cast<std::string>
		(get_elevation_acceleration());
	return info.toStyledString();
}

base_servo_protocol_out::base_servo_protocol_out()
{
	pattern_ = Servo_WP_Stop;
	manual_azimuth_speed_ = 0;
	manual_elevation_speed_ = 0;

	preset_azimuth_ = 0;
	preset_elevation_ = 0;

	track_img_x_offset_ = 0;
	track_img_y_offset_ = 0;

	search_azimuth_ = 0;
	search_elevation_ = 0;

	search_to_track_azimuth_ = 0;
	search_to_track_elevation_ = 0;

	scan_begin_pt_azimuth_ = 0;
	scan_begin_pt_elevation_ = 0;
	scan_end_pt_azimuth_ = 0;
	scan_end_pt_elevation_ = 0;
}

bool base_servo_protocol_out::from_message(std::string msg_str)
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
	if (str_command.compare("stop") == 0)
	{
		stop();
		return true;
	}
	else if (str_command.compare("manual") == 0)
	{
		double azimuth_speed = 0;
		double elevation_speed = 0;
		azimuth_speed = boost::lexical_cast<double>
			(info["param_1"].asString());
		elevation_speed = boost::lexical_cast<double>
			(info["param_2"].asString());
		manual_move(azimuth_speed, elevation_speed);
		return true;
	}
	else if (str_command.compare("preset") == 0)
	{
		double azimuth_position = 0;
		double elevation_position = 0;
		azimuth_position = boost::lexical_cast<double>
			(info["param_1"].asString());
		elevation_position = boost::lexical_cast<double>
			(info["param_2"].asString());
		preset(azimuth_position, elevation_position);
		return true;
	}
	else if (str_command.compare("track") == 0)
	{
		int x_offset = 0;
		int y_offset = 0;
		x_offset = boost::lexical_cast<int>
			(info["param_1"].asString());
		y_offset = boost::lexical_cast<int>
			(info["param_2"].asString());
		track(x_offset, y_offset);
		return true;
	}
	else if (str_command.compare("search") == 0)
	{
		double azimuth = 0;
		double elevation = 0;
		azimuth = boost::lexical_cast<double>
			(info["param_1"].asString());
		elevation = boost::lexical_cast<double>
			(info["param_2"].asString());
		search(azimuth, elevation);
		return true;
	}
	else if (str_command.compare("search_to_track") == 0)
	{
		double azimuth = 0;
		double elevation = 0;
		azimuth = boost::lexical_cast<double>
			(info["param_1"].asString());
		elevation = boost::lexical_cast<double>
			(info["param_2"].asString());
		search_to_track(azimuth, elevation);
		return true;
	}
	else
	{
		return false;
	}
}

bool base_servo_protocol_out::stop()
{
	pattern_ = Servo_WP_Stop;
	return true;
}

bool base_servo_protocol_out::manual_move(double azimuth_speed, double elevation_speed)
{
	manual_azimuth_speed_ = azimuth_speed;
	manual_elevation_speed_ = elevation_speed;
	pattern_ = Servo_WP_ManualMove;
	return true;
}

bool base_servo_protocol_out::preset(double azimuth, double elevation)
{
	preset_azimuth_ = azimuth;
	preset_elevation_ = elevation;
	pattern_ = Servo_WP_Preset;
	return true;
}

bool base_servo_protocol_out::track(int img_x_offset, int img_y_offset)
{
	track_img_x_offset_ = img_x_offset;
	track_img_y_offset_ = img_y_offset;
	pattern_ = Servo_WP_Track;
	return true;
}

bool base_servo_protocol_out::search(double azimuth, double elevation)
{
	search_azimuth_ = azimuth;
	search_elevation_ = elevation;
	pattern_ = Servo_WP_SearchTypeOne;
	return true;
}

bool base_servo_protocol_out::search_to_track(double azimuth, double elevation)
{
	search_to_track_azimuth_ = azimuth;
	search_to_track_elevation_ = elevation;
	pattern_ = Servo_WP_Search2Track;
	return true;
}

bool base_servo_protocol_out::scan(double begin_x, double begin_y, double end_x, double end_y)
{
	scan_begin_pt_azimuth_ = begin_x;
	scan_begin_pt_elevation_ = begin_y;
	scan_end_pt_azimuth_ = end_x;
	scan_end_pt_elevation_ = end_y;
	pattern_ = Servo_WP_Scan;
	return true;
}
