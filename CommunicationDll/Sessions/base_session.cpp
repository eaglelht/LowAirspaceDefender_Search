#include "stdafx.h"
#include "base_session.h"
#include <boost/thread/lock_types.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/bind.hpp>
#include "../Bases/io_buffer_observer.h"
#include "../Bases/message_observer_.h"
#include "../Bases/protocol_observer.h"
#include "../IOs/io_buffer.h"
#include "../IOs/i_io.h"

base_session::base_session()
{
	io_ = 0;
	protocol_in_ = 0;
	protocol_out_ = 0;

	in_buffer_ = boost::make_shared<io_buffer<unsigned char>>(default_buffer_size);
	out_buffer_ = boost::make_shared<io_buffer<unsigned char>>(default_buffer_size);
}

base_session::~base_session()
{
	stop();
}

bool base_session::start()
{
	if (io_ == false)
	{
		return false;
	}
	running_ = true;
	return true;
}

bool base_session::stop()
{
	running_ = false;
	close();
	return true;
}

bool base_session::set_io(boost::shared_ptr<i_io> sptr_io)
{
	if (sptr_io == 0)
	{
		return false;
	}
	io_ = sptr_io;
	io_->set_on_read_callback(
		boost::bind(&base_session::fn_on_read_process, this,
			::_1));
	io_->set_on_write_callback(
		boost::bind(&base_session::fn_on_write_process, this));
	return true;
}

bool base_session::set_protocol_in(boost::shared_ptr<i_protocol_in> protocol)
{
	if (protocol == 0)
	{
		return false;
	}
	protocol_in_ = protocol;
	return true;
}

bool base_session::set_protocol_out(boost::shared_ptr<i_protocol_out> protocol)
{
	if (protocol == 0)
	{
		return false;
	}
	protocol_out_ = protocol;
	return true;
}

bool base_session::open()
{
	if (io_)
	{
		return io_->open();
	}
	return false;
}

bool base_session::close()
{
	if (io_)
	{
		return io_->close();
	}
	return false;
}

bool base_session::reopen()
{
	bool res = true;
	res = close() && res;
	res = open() && res;
	return res;
}

bool base_session::start_read()
{
	return do_read();
}

bool base_session::start_write()
{
	return do_write();
}

bool base_session::do_write()
{
	if (io_ && out_buffer_)
	{
		return io_->do_write(
			out_buffer_->get_write_pos(),
			out_buffer_->get_write_size());
	}
	return false;
}

bool base_session::do_read()
{
	if (io_ && in_buffer_)
	{
		return io_->do_read(
			in_buffer_->get_read_pos(),
			in_buffer_->get_read_size());
	}
	return false;
}

