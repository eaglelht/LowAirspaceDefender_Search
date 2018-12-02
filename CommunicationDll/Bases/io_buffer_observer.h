#ifndef IO_BUFFER_OBSERVER_H_
#define IO_BUFFER_OBSERVER_H_
#include <boost\enable_shared_from_this.hpp>

class io_buffer_observer
	: public boost::enable_shared_from_this
	<io_buffer_observer>
{
public:
	io_buffer_observer() {}
	virtual ~io_buffer_observer() {}
public:
	virtual void process_in_buffer(
		unsigned char* buffer, int size, void* ptr_arg = 0
	) = 0;
	virtual void process_out_buffer(
		unsigned char* buffer, int size, void* ptr_arg = 0
	) = 0;
};

#endif // IO_BUFFER_OBSERVER_H_
