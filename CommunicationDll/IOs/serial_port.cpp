#include "stdafx.h"
#include "serial_port.h"
#include <boost\bind\bind.hpp>
#include <boost\asio\placeholders.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\format.hpp>
#include "..\Json\value.h"
#include "..\Json\reader.h"

serial_port::serial_port(boost::asio::io_service& service)
	: sp_(service)
{
	fn_on_read_callback = 0;
	fn_on_write_callback = 0;
	//
	port_ = "COM0";
	baud_rate_ = 9600;
	flow_control_ = boost::asio::serial_port::flow_control::type::none;
	parity_ = boost::asio::serial_port::parity::type::none;
	stop_bits_ = boost::asio::serial_port::stop_bits::type::one;
	character_size_ = 8;
}

serial_port::~serial_port()
{
	close();
}

bool serial_port::close()
{
	try
	{
		sp_.close();
	}
	catch (boost::system::system_error&)
	{
		return false;
	}
	return true;
}

bool serial_port::init(std::string info)
{
	Json::Reader j_reader;
	Json::Value j_info;
	if (j_reader.parse(info, j_info) == false)
	{
		return false;
	}
	if (j_info["port"].empty() == false)
	{
		int port;
		try
		{
			port = boost::lexical_cast<int>(j_info["port"].asString());
			boost::format fmt("COM%d");
			fmt % port;
			port_ = fmt.str();
		}
		catch (const boost::bad_lexical_cast&)
		{
			return false;
		}
	}
	if (j_info["baud_rate"].empty() == false)
	{
		try
		{
			baud_rate_ = boost::lexical_cast<int>(j_info["baud_rate"].asString());
		}
		catch (const boost::bad_lexical_cast&)
		{
			return false;
		}
	}
	if (j_info["flow_control"].empty() == false)
	{
		try
		{
			std::string type_str;
			type_str = boost::lexical_cast<std::string>(j_info["flow_control"].asString());
			if (type_str.compare("none") == 0)
			{
				flow_control_ = boost::asio::serial_port::flow_control::type::none;
			}
			else if (type_str.compare("software") == 0)
			{
				flow_control_ = boost::asio::serial_port::flow_control::type::software;
			}
			else if (type_str.compare("hardware") == 0)
			{
				flow_control_ = boost::asio::serial_port::flow_control::type::hardware;
			}
			else
			{
				return false;
			}
		}
		catch (const boost::bad_lexical_cast&)
		{
			return false;
		}
	}
	if (j_info["parity"].empty() == false)
	{
		try
		{
			std::string type_str;
			type_str = boost::lexical_cast<std::string>(j_info["parity"].asString());
			if (type_str.compare("none") == 0)
			{
				parity_ = boost::asio::serial_port::parity::type::none;
			}
			else if (type_str.compare("odd") == 0)
			{
				parity_ = boost::asio::serial_port::parity::type::odd;
			}
			else if (type_str.compare("even") == 0)
			{
				parity_ = boost::asio::serial_port::parity::type::even;
			}
			else
			{
				return false;
			}
		}
		catch (const boost::bad_lexical_cast&)
		{
			return false;
		}
	}
	if (j_info["stop_bits"].empty() == false)
	{
		try
		{
			std::string type_str;
			type_str = boost::lexical_cast<std::string>(j_info["stop_bits"].asString());
			if (type_str.compare("one") == 0)
			{
				stop_bits_ = boost::asio::serial_port::stop_bits::type::one;
			}
			else if (type_str.compare("onepointfive") == 0)
			{
				stop_bits_ = boost::asio::serial_port::stop_bits::type::onepointfive;
			}
			else if (type_str.compare("two") == 0)
			{
				stop_bits_ = boost::asio::serial_port::stop_bits::type::two;
			}
			else
			{
				return false;
			}
		}
		catch (const boost::bad_lexical_cast&)
		{
			return false;
		}
	}
	if (j_info["character_size"].empty() == false)
	{
		try
		{
			character_size_ = boost::lexical_cast<int>(j_info["character_size"].asString());
		}
		catch (const boost::bad_lexical_cast&)
		{
			return false;
		}
	}
	return true;
}

bool serial_port::is_connected()
{
	return sp_.is_open();
}

bool serial_port::open()
{
	if (sp_.is_open() == true)
	{
		return true;
	}
	try
	{
		sp_.open(port_);
		sp_.set_option(boost::asio::serial_port::baud_rate(baud_rate_));
		sp_.set_option(boost::asio::serial_port::flow_control(flow_control_));
		sp_.set_option(boost::asio::serial_port::parity(parity_));
		sp_.set_option(boost::asio::serial_port::stop_bits(stop_bits_));
		sp_.set_option(boost::asio::serial_port::character_size(character_size_));
	}
	catch (boost::system::system_error&)
	{
		return false;
	}
	return true;
}

bool serial_port::do_read(unsigned char* buffer, int size)
{
	sp_.async_read_some(
		boost::asio::buffer(buffer, size),
		boost::bind(&serial_port::on_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	return true;
}

bool serial_port::set_on_read_callback(boost::function<bool(size_t)> fn)
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

bool serial_port::do_write(unsigned char* buffer, int size)
{
	sp_.async_write_some(
		boost::asio::buffer(buffer, size),
		boost::bind(&serial_port::on_write, this,
			boost::asio::placeholders::error));
	return true;
}

bool serial_port::set_on_write_callback(boost::function<bool(void)> fn)
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

void serial_port::on_read(const boost::system::error_code& err, size_t size)
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

void serial_port::on_write(const boost::system::error_code& err)
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
