#include "stdafx.h"
#include "laser_type_001_session.h"
#include "../IOs/i_io.h"
#include "../IOs/io_buffer.h"
#include "../Protocols/i_protocol.h"

laser_type_001_session::laser_type_001_session()
{

}

laser_type_001_session::~laser_type_001_session()
{

}

void laser_type_001_session::monitor_process()
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

void laser_type_001_session::process_message(boost::shared_ptr<message> subject, void* ptr_arg /*= 0*/)
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool laser_type_001_session::fn_on_read_process(int size)
{
	if (in_buffer_ == 0 || running_ == false)
	{
		return false;
	}
	//notify_in_buffer(in_buffer_->get_read_pos(), size);
	in_buffer_->add_size(size);
	while (translate_protocol_from_buffer())
	{
		notify_protocol_in(protocol_in_, 0);
		process_protocol();
	}
	do_read();
	return true;
}

bool laser_type_001_session::fn_on_write_process()
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool laser_type_001_session::translate_protocol_from_buffer()
{
	if (in_buffer_ == 0 || protocol_in_ == 0)
	{
		return false;
	}
	return protocol_in_->from_bin(
		in_buffer_->buffer_, in_buffer_->size_);
}

void laser_type_001_session::process_protocol()
{

}
