#include "stdafx.h"
#include "udp.h"
#include <boost/bind/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/lexical_cast.hpp>
#include "../Json/value.h"
#include "../Json/reader.h"

udp::udp(boost::asio::io_service& service)
	: socket_(service)
{
	read_ep_ = 0;
	write_ep_ = 0;

	fn_on_read_callback = 0;
	fn_on_write_callback = 0;
}

udp::~udp()
{
	close();
}

bool udp::close()
{
	try
	{
		socket_.close();
	}
	catch (const boost::system::system_error& error)
	{
		if (error.code() == boost::asio::error::operation_aborted)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool udp::do_read(unsigned char* buffer, int size)
{
	if (read_ep_ == 0)
	{
		return false;
	}
	socket_.async_receive_from(
		boost::asio::buffer(buffer, size), remote_ep_,
		boost::bind(&udp::on_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	return true;
}

bool udp::do_write(unsigned char* buffer, int size)
{
	if (write_ep_ == 0)
	{
		return false;
	}
	socket_.async_send_to(
		boost::asio::buffer(buffer, size), *write_ep_,
		boost::bind(&udp::on_write, this,
			boost::asio::placeholders::error));
	return true;
}

bool udp::init(std::string info)
{
	Json::Reader j_reader;
	Json::Value j_info;
	if (j_reader.parse(info, j_info) == false)
	{
		return false;
	}
	if (j_info["read_ip"].empty() == false)
	{
		std::string ip;
		int port;
		try
		{
			ip = boost::lexical_cast<std::string>(j_info["read_ip"].asString());
			port = boost::lexical_cast<int>(j_info["read_port"].asString());
		}
		catch (const boost::bad_lexical_cast&)
		{			
			return false;
		}
		read_ep_ = boost::make_shared<
			boost::asio::ip::udp::endpoint>
			(boost::asio::ip::address::from_string(ip)
				, port);
	}
	if (j_info["write_ip"].empty() == false)
	{
		std::string ip;
		int port;
		try
		{
			ip = boost::lexical_cast<std::string>(j_info["write_ip"].asString());
			port = boost::lexical_cast<int>(j_info["write_port"].asString());
		}
		catch (const boost::bad_lexical_cast&)
		{
			return false;
		}
		write_ep_ = boost::make_shared<
			boost::asio::ip::udp::endpoint>
			(boost::asio::ip::address::from_string(ip)
				, port);
	}
	return true;
}

bool udp::is_connected()
{
	return socket_.is_open();
}

bool udp::open()
{
	if (read_ep_ == 0)
	{
		return false;
	}
	if (socket_.is_open() == true)
	{
		return true;
	}
	try
	{
		socket_.open(read_ep_->protocol());
		socket_.bind(*read_ep_);
	}
	catch (boost::system::system_error&)
	{
		return false;
	}
	return true;
}

bool udp::set_on_read_callback(boost::function<bool(size_t) > fn)
{
	if (fn.empty())
	{
		return false;
	}
	else
	{
		fn_on_read_callback = fn;
		return true;
	}
}

bool udp::set_on_write_callback(boost::function<bool(void) > fn)
{
	if (fn.empty())
	{
		return false;
	}
	else
	{
		fn_on_write_callback = fn;
		return true;
	}
}

void udp::on_read(const boost::system::error_code& err, size_t size)
{
	if (err)
	{
		return;
	}
	if (fn_on_read_callback.empty())
	{
		return;
	}
	fn_on_read_callback(size);
}

void udp::on_write(const boost::system::error_code& err)
{
	if (err)
	{
		return;
	}
	if (fn_on_write_callback.empty())
	{
		return;
	}
	fn_on_write_callback();
}

