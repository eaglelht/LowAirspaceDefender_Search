#ifndef PROTOCOL_OBSERVER_H_
#define PROTOCOL_OBSERVER_H_
#include <boost\smart_ptr\shared_ptr.hpp>
#include <boost\enable_shared_from_this.hpp>

class i_protocol_in;
class i_protocol_out;

class protocol_observer
	: public boost::enable_shared_from_this
	<protocol_observer>
{
public:
	protocol_observer() {}
	virtual ~protocol_observer() {}
public:
	virtual void process_protocol_in(
		boost::shared_ptr<i_protocol_in> protocol, void* ptr_arg) = 0;
	virtual void process_protocol_out(
		boost::shared_ptr<i_protocol_out> protocol, void* ptr_arg) = 0;

};

#endif // PROTOCOL_OBSERVER_H_