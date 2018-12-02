#include "stdafx.h"
#include "search_servo_type_001_session.h"
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/microsec_time_clock.hpp>
#include "../Bases/message.h"
#include "../IOs/io_buffer.h"
#include "../IOs/i_io.h"
#include "../Json/value.h"
#include "../Protocols/Servo_Protocols/base_servo_protocol.h"

search_servo_type_001_session::search_servo_type_001_session(boost::asio::io_service& service)
	: write_timer_(service)
{
	running_ = false;
	write_interval_ = 20;
}

search_servo_type_001_session::~search_servo_type_001_session()
{
	
}

void search_servo_type_001_session::process_message(boost::shared_ptr<message> subject, void* ptr_arg)
{
	if (subject == 0)
	{
		return;
	}
	if (subject->main_type_ == message::E_MMT_Command
		&& subject->sub_type_ == message::E_MST_Servo)
	{
		// 对伺服的命令
		messages_.push(subject);		
	}
}

void search_servo_type_001_session::monitor_process()
{
	if (running_ == false || io_ == 0)
	{
		return;
	}
	if (io_->is_connected() == false)
	{
		if (reopen() == true)
		{
			start_read();
			start_write();
		}
	} 
	else
	{
		// 做一些其他的处理工作
		boost::posix_time::ptime now
			= boost::posix_time::microsec_clock::local_time();
		compute_bps(now);
		compute_pps(now);
	}
}

bool search_servo_type_001_session::fn_on_read_process(int size)
{
	if (in_buffer_ == 0 || running_ == false)
	{
		return false;
	}
	got_bytes_count_ += size;
	//notify_in_buffer(in_buffer_->get_read_pos(), size);
	in_buffer_->add_size(size);
	while (translate_protocol_from_buffer())
	{
		++got_pack_count_;
		notify_protocol_in(protocol_in_, 0);
		process_protocol();
	}
	do_read();	
	return true;
}

bool search_servo_type_001_session::fn_on_write_process()
{
	if (running_ == false)
	{
		return false;
	}
	return do_wait_write_timer(write_interval_);	
}

bool search_servo_type_001_session::start_write()
{
	return do_wait_write_timer(0);
}

bool search_servo_type_001_session::do_wait_write_timer(int ms)
{
#if _MSC_VER <= 1600
	write_timer_.expires_from_now(boost::chrono::milliseconds(ms));
#else
	write_timer_.expires_from_now(std::chrono::milliseconds(ms));
#endif	
	write_timer_.async_wait(
		boost::bind(&search_servo_type_001_session::on_wait_write_timer,
			this, boost::asio::placeholders::error));
	return true;
}

void search_servo_type_001_session::over_wait_write_timer()
{
	try
	{
		write_timer_.cancel();
	}
	catch (const boost::system::system_error&)
	{
		;
	}
}

void search_servo_type_001_session::on_wait_write_timer(const boost::system::error_code& error)
{
	if (running_ == false)
	{
		return;
	}
	if (error)
	{
		return;
	}
	notify_protocol_out(protocol_out_, 0);
	if (translate_protocol_to_buffer() == false)
	{
		//return;
	}
	//notify_out_buffer(out_buffer_->buffer_, out_buffer_->size_);
	do_write();
}

bool search_servo_type_001_session::translate_protocol_to_buffer()
{
	if (out_buffer_ == 0 || protocol_out_ == 0)
	{
		return false;
	}
	while (messages_.is_empty() == false)
	{
		boost::shared_ptr<message> subject = messages_.pop();
		protocol_out_->from_message(subject->information_);
	}	
	out_buffer_->size_ =
		protocol_out_->to_bin(out_buffer_->buffer_, out_buffer_->max_size_);	
	if (out_buffer_->size_ > 0)
	{
		return true;
	}
	return false;
}

bool search_servo_type_001_session::translate_protocol_from_buffer()
{
	if (in_buffer_ == 0 || protocol_in_ == 0)
	{
		return false;
	}
	return protocol_in_->from_bin(
		in_buffer_->buffer_, in_buffer_->size_);
}

void search_servo_type_001_session::process_protocol()
{
	if (protocol_in_->is_changed())
	{
		
	}
}

void search_servo_type_001_session::compute_pps(boost::posix_time::ptime& now)
{
	__int64 ms = (now - pre_got_pack_time_).total_milliseconds();
	double pps = got_pack_count_ * 1000 / static_cast<double>(ms);
	pre_got_pack_time_ = now;
	got_pack_count_ = 0;
	boost::shared_ptr<message> sptr_msg
		= boost::make_shared<message>();
	//sptr_msg->time_ms_ = now;
	Json::Value info;
	info["pps"] = pps;
	sptr_msg->information_ = info.toStyledString();
}

void search_servo_type_001_session::compute_bps(boost::posix_time::ptime& now)
{
	__int64 ms = (now - pre_got_buffer_time_).total_milliseconds();
	double bps = got_bytes_count_ * 1000 / static_cast<double>(ms);
	pre_got_buffer_time_ = now;
	got_bytes_count_ = 0;
	boost::shared_ptr<message> sptr_msg
		= boost::make_shared<message>();
	//sptr_msg->time_ms_ = now;
	Json::Value info;
	info["bps"] = bps;
	sptr_msg->information_ = info.toStyledString();
}
