#ifndef MESSAGE_H_
#define MESSAGE_H_
#include <string>
#include <list>
#include <boost/enable_shared_from_this.hpp>
#include <boost/date_time/posix_time/ptime.hpp>

struct message
	: public boost::enable_shared_from_this
	<message>
{
	enum E_Message_Main_Type
	{
		E_MMT_Undefine = 0,
		E_MMT_Command,
		E_MMT_Update
	};

	enum E_Message_Sub_Type
	{
		E_MST_Undefine = 0,
		E_MST_Servo,
		E_MST_IR,
		E_MST_TV,
		E_MST_Laser
	};

	E_Message_Main_Type main_type_;
	E_Message_Sub_Type sub_type_;

	__int16 source_id_;
	__int16 destination_id_;
	boost::posix_time::ptime create_time_;
	boost::posix_time::time_duration delay_time_;
	std::string information_;
};


#endif // MESSAGE_H_
