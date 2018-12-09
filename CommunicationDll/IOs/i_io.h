#ifndef I_IO_H_
#define I_IO_H_
#include <string>
#include <boost\function.hpp>

class i_io
{
public:
	virtual ~i_io() {}
public:
	virtual bool init(std::string info) = 0;
	virtual bool open() = 0;
	virtual bool close() = 0;
	virtual bool is_connected() = 0;

	virtual bool do_read(unsigned char* buffer, int size) = 0;
	virtual bool set_on_read_callback(boost::function<bool(size_t)> fn) = 0;

	virtual bool do_write(unsigned char* buffer, int size) = 0;
	virtual bool set_on_write_callback(boost::function<bool(void)> fn) = 0;
};

#endif // I_IO_H_
