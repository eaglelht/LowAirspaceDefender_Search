#include "stdafx.h"
#include "ir_len_type_001_session.h"
#include "../IOs/i_io.h"
#include "../Bases/message.h"
#include "../Protocols/i_protocol.h"
#include "base_session.h"
#include "../IOs/io_buffer.h"

void ir_len_type_001_session::monitor_process()
{
	if (running_ == false || io_ == 0)
	{
		return;
	}
	if (io_->is_connected() == false)
	{
		if (reopen() == true)
		{
			
		}
	}
	else
	{
		// 做一些其他的处理工作
	}
}

void ir_len_type_001_session::process_message(boost::shared_ptr<message> subject, void* ptr_arg /*= 0*/)
{
	if (subject == 0 || running_ == false)
	{
		return;
	}
	if (subject->main_type_ == message::E_MMT_Command
		&& subject->sub_type_ == message::E_MST_IR)
	{
		if (translate_message_to_protocol(subject) == true)
		{
			// 将message翻译成protocol
			notify_protocol_out(protocol_out_, 0);
		}
		if (translate_protocol_to_buffer() == true)
		{
			//notify_out_buffer(out_buffer_->buffer_, out_buffer_->size_);
			do_write();
		}
	}
}

bool ir_len_type_001_session::translate_message_to_protocol(boost::shared_ptr<message> subject)
{
	if (protocol_out_ == 0 || subject == 0)
	{
		return false;
	}
	if (protocol_out_->from_message(subject->information_) == false
		|| protocol_out_->need_to_translate() == false)
	{
		return false;
	}
	return true;
}

bool ir_len_type_001_session::translate_protocol_to_buffer()
{
	if (out_buffer_ == 0 || protocol_out_ == 0)
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

bool ir_len_type_001_session::fn_on_read_process(int size)
{
	return false;
}

bool ir_len_type_001_session::fn_on_write_process()
{
	return false;
}

