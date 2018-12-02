#ifndef BASE_SERVO_PROTOCOL_H_
#define BASE_SERVO_PROTOCOL_H_
#include <string>
#include "../i_protocol.h"

enum E_ServoWorkPattern
{
	Servo_WP_Undefined = 0,
	Servo_WP_Stop,
	Servo_WP_ManualMove,
	Servo_WP_Preset,
	Servo_WP_Track,
	Servo_WP_Scan,
	Servo_WP_Search,
	Servo_WP_Search2Track,
	Servo_WP_SearchTypeOne,
	Servo_WP_SearchTypeTwo,
	Servo_WP_SearchTypeThree,
	Servo_WP_SearchTypeFour
};

extern std::string E_ServoWorkPatternString[];

class base_servo_protocol_in
	: public i_protocol_in
{
protected:
	bool has_value_changed_;
	E_ServoWorkPattern pattern_;
	double azimuth_;
	double elevation_;
	double azimuth_speed_;
	double elevation_speed_;
	double azimuth_acceleration_;
	double elevation_acceleration_;
public:
	base_servo_protocol_in();
	virtual ~base_servo_protocol_in() {}
public:
	inline virtual E_ServoWorkPattern get_work_pattern()
	{
		return pattern_;
	}
	inline virtual double get_azimuth()
	{
		return azimuth_;
	}
	inline virtual double get_elevation()
	{
		return elevation_;
	}
	inline virtual double get_azimuth_speed()
	{
		return azimuth_speed_;
	}
	inline virtual double get_elevation_speed()
	{
		return elevation_speed_;
	}
	inline virtual double get_azimuth_acceleration()
	{
		return azimuth_acceleration_;
	}
	inline virtual double get_elevation_acceleration()
	{
		return elevation_acceleration_;
	}
	virtual bool is_changed() override;
	virtual std::string to_message() override;
};

class base_servo_protocol_out
	: public i_protocol_out
{
protected:
	E_ServoWorkPattern pattern_;
	double manual_azimuth_speed_;
	double manual_elevation_speed_;
	double preset_azimuth_;
	double preset_elevation_;
	int track_img_x_offset_;
	int track_img_y_offset_;
	double search_azimuth_;
	double search_elevation_;
	double search_to_track_azimuth_;
	double search_to_track_elevation_;
	double scan_begin_pt_azimuth_;
	double scan_end_pt_azimuth_;
	double scan_begin_pt_elevation_;
	double scan_end_pt_elevation_;
public:
	base_servo_protocol_out();
	virtual ~base_servo_protocol_out() {}

	virtual bool from_message(std::string msg_str) override;
public:
	virtual bool stop();
	virtual bool manual_move(double azimuth_speed, double elevation_speed);
	virtual bool preset(double azimuth, double elevation);
	virtual bool track(int img_x_offset, int img_y_offset);
	virtual bool search(double azimuth, double elevation);
	virtual bool search_to_track(double azimuth, double elevation);
	virtual bool scan(double begin_x, double begin_y,
		double end_x, double end_y);
};

#endif // BASE_SERVO_PROTOCOL_H_
