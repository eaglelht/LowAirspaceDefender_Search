#include "stdafx.h"
#include "ir_sensor_type_001_session.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind/bind.hpp>
#include "../Bases/message.h"
#include "../IOs/i_io.h"
#include "../IOs/io_buffer.h"
#include "../Protocols/i_protocol.h"

ir_sensor_type_001_session::ir_sensor_type_001_session(boost::asio::io_service& service)
	: write_timer_(service)
{
	write_interval_ = 20;
}

ir_sensor_type_001_session::~ir_sensor_type_001_session()
{

}

void ir_sensor_type_001_session::monitor_process()
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
		}
	}
	else
	{
		// 做一些其他的处理工作
	}
}

void ir_sensor_type_001_session::process_message(boost::shared_ptr<message> subject, void* ptr_arg)
{
	if (subject == 0 || running_ == false)
	{
		return;
	}
	if (subject->main_type_ == message::E_MMT_Command
		&& subject->sub_type_ == message::E_MST_IR)
	{
		messages_.push(subject);
		start_write_timer();
	}
}

bool ir_sensor_type_001_session::fn_on_read_process(int size)
{
	if (in_buffer_ == 0 || running_ == false)
	{
		return false;
	}
	//got_bytes_count_ += size;
	//notify_in_buffer(in_buffer_->get_read_pos(), size);
	in_buffer_->add_size(size);
	while (translate_protocol_from_buffer())
	{
		//++got_pack_count_;
		notify_protocol_in(protocol_in_, 0);
		process_protocol();
	}
	do_read();
	return true;
}

bool ir_sensor_type_001_session::fn_on_write_process()
{
	if (running_ == false)
	{
		return false;
	}
	return do_wait_write_timer(write_interval_);
}

bool ir_sensor_type_001_session::start_write_timer()
{
	if (running_ == false)
	{
		return false;
	}
	if (has_to_translate() == true)
	{
		return do_wait_write_timer(0);		
	}
	return true;
}

bool ir_sensor_type_001_session::do_wait_write_timer(int ms)
{
#if _MSC_VER <= 1600
	write_timer_.expires_from_now(boost::chrono::milliseconds(ms));
#else
	write_timer_.expires_from_now(std::chrono::milliseconds(ms));
#endif	
	write_timer_.async_wait(
		boost::bind(&ir_sensor_type_001_session::on_wait_write_timer,
			this, boost::asio::placeholders::error));
	return true;
}

void ir_sensor_type_001_session::over_wait_write_timer()
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

void ir_sensor_type_001_session::on_wait_write_timer(const boost::system::error_code& error)
{
	if (running_ == false)
	{
		return;
	}
	if (error)
	{
		return;
	}
	if (has_to_translate() == false)
	{
		return;
	}
	if (translate_message_to_protocol())
	{
		// 将message翻译成protocol
		notify_protocol_out(protocol_out_, 0);
	}
	if (translate_protocol_to_buffer() == false)
	{
		// 将protocol翻译成具体的buffer
		do_wait_write_timer(write_interval_);
	}
	else
	{
		//notify_out_buffer(out_buffer_->buffer_, out_buffer_->size_);
		do_write();
	}
}

bool ir_sensor_type_001_session::has_to_translate()
{
	// 判断message有没有需要翻译的
	if (messages_.is_empty() == true)
	{
		return true;
	}
	// 判断protocol有没有需要翻译的
	if (protocol_out_->need_to_translate() == true)
	{
		return true;
	}
	return false;
}

bool ir_sensor_type_001_session::translate_message_to_protocol()
{
	if (protocol_out_ == 0)
	{
		return false;
	}
	if (messages_.is_empty() == false)
	{
		boost::shared_ptr<message> subject = messages_.pop();
		// 判断当前指令是否到达可以执行的时间
		boost::posix_time::ptime now =
			boost::posix_time::microsec_clock::universal_time();
		if ((now - subject->create_time_)
			>= subject->delay_time_)
		{
			protocol_out_->from_message(subject->information_);
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool ir_sensor_type_001_session::translate_protocol_to_buffer()
{
	if (out_buffer_ == 0 || protocol_out_ == 0)
	{
		return false;
	}
	if (protocol_out_->need_to_translate() == false)
	{
		return false;
	}
	out_buffer_->size_ =
		protocol_out_->to_bin(out_buffer_->buffer_, out_buffer_->max_size_);
	if (out_buffer_->size_ > 0)
	{
		return true;
	}
	return false;
}

bool ir_sensor_type_001_session::translate_protocol_from_buffer()
{
	if (in_buffer_ == 0 || protocol_in_ == 0)
	{
		return false;
	}
	return protocol_in_->from_bin(
		in_buffer_->buffer_, in_buffer_->size_);
}

void ir_sensor_type_001_session::process_protocol()
{

}
