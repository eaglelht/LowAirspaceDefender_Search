#ifndef SESSION_FACTORY_H_
#define SESSION_FACTORY_H_
#include <string>
#include <boost/asio/io_service.hpp>
#include "../Bases/loki/Factory.h"
#include "../Bases/loki/Sequence.h"
#include "base_session.h"
#include "search_servo_type_001_session.h"

typedef Loki::Factory
<
	base_session,
	std::string,
	Loki::Seq<boost::asio::io_service&>
> session_factory;

typedef Loki::SingletonHolder
<
	session_factory,
	Loki::CreateUsingNew,
	Loki::LongevityLifetime::DieAsSmallObjectChild
> singleton_session_factory;

template<class T> struct session_create_t
{
	T* operator() (boost::asio::io_service& service) const
	{
		return new T(service);
	}
};

inline bool regster_session_factory()
{
	session_factory& factory =
		singleton_session_factory::Instance();

	bool res = true;

	res = res & factory.Register("servo",
		session_create_t<search_servo_type_001_session>());

	return res;
}

#endif // SESSION_FACTORY_H_
