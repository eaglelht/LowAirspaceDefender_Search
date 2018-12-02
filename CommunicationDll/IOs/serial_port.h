#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_
#include <string>
#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/function.hpp>
#include "i_io.h"
class serial_port
	: public i_io
{
	boost::asio::serial_port sp_;
	std::string port_;
	int baud_rate_;
	boost::asio::serial_port::flow_control::type flow_control_;
	boost::asio::serial_port::parity::type parity_;
	boost::asio::serial_port::stop_bits::type stop_bits_;
	int character_size_;

	boost::function<bool(size_t)> fn_on_read_callback;
	boost::function<bool(void)> fn_on_write_callback;
public:
	serial_port(boost::asio::io_service& service);
	virtual ~serial_port();

	virtual bool close() override;
	virtual bool do_read(unsigned char* buffer, int size) override;
	virtual bool do_write(unsigned char* buffer, int size) override;
	virtual bool init(std::string info) override;
	virtual bool is_connected() override;
	virtual bool open() override;
	virtual bool set_on_read_callback(boost::function<bool(size_t) > fn) override;
	virtual bool set_on_write_callback(boost::function<bool(void) > fn) override;
protected:
	virtual void on_read(const boost::system::error_code& err, size_t size);
	virtual void on_write(const boost::system::error_code& err);
};

#endif
