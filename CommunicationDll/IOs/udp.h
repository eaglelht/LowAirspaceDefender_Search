#ifndef UDP_H_
#define UDP_H_
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/function.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include "i_io.h"

class udp
	: public i_io
{
	boost::asio::ip::udp::socket socket_;
	boost::shared_ptr<boost::asio::ip::udp::endpoint> read_ep_;
	boost::shared_ptr<boost::asio::ip::udp::endpoint> write_ep_;
	boost::asio::ip::udp::endpoint remote_ep_;

	boost::function<bool(size_t)> fn_on_read_callback;
	boost::function<bool(void)> fn_on_write_callback;
public:
	udp(boost::asio::io_service& service);
	virtual ~udp();

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

#endif // UDP_H_
