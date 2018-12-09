#ifndef IO_FACTORY_H_
#define IO_FACTORY_H_
#include <string>
#include <boost/asio/io_service.hpp>
#include "../Bases/loki/Factory.h"
#include "../Bases/loki/Sequence.h"
#include "i_io.h"
#include "serial_port.h"
#include "udp.h"

//#pragma comment(lib, "Bases/loki/loki_D.lib")

typedef Loki::Factory<
	i_io,
	std::string,
	Loki::Seq<boost::asio::io_service&>
> io_factory;

 template<class T> struct io_create_t
 {
 	T* operator() (boost::asio::io_service& service) const
 	{
 		return new T(service);
 	}
 };

typedef Loki::SingletonHolder
<
	io_factory, 
	Loki::CreateUsingNew, 
	Loki::LongevityLifetime::DieAsSmallObjectChild
> singleton_io_factory;

inline bool register_io_factory()
{
	io_factory& factory = singleton_io_factory::Instance();

	bool res = true;
	res = res && factory.Register("udp", io_create_t<udp>());
	res = res && factory.Register("serial_port", io_create_t<serial_port>());

	return res;
}

#endif // IO_FACTORY_H_
