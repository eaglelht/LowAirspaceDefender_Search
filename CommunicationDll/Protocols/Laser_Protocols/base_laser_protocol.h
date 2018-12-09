#ifndef BASE_LASER_PROTOCOL_H_
#define BASE_LASER_PROTOCOL_H_
#include "../i_protocol.h"
#include <boost/date_time/posix_time/ptime.hpp>

class base_laser_protocol_in
	: public i_protocol_in
{
protected:
	double original_distance_;
	boost::posix_time::ptime got_valid_distance_time_;
	double valid_distance_;
	double valid_distance_change_rate_;
public:
	inline virtual double get_original_distance()
	{
		return original_distance_;
	}
};

class base_laser_protocol_out
	: public i_protocol_out
{
public:
	virtual bool from_message(std::string msg_str) override;
protected:
	virtual bool shoot_once() = 0;
	virtual bool power_on() = 0;
	virtual bool power_off() = 0;
};

#endif // BASE_LASER_PROTOCOL_H_
