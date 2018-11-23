#ifndef MESSAGE_OBSERVER_H_
#define MESSAGE_OBSERVER_H_
#include <boost\smart_ptr\shared_ptr.hpp>
#include <boost\enable_shared_from_this.hpp>

struct message;

class message_observer
	: public boost::enable_shared_from_this
	<message_observer>
{
public:
	message_observer() {}
	virtual ~message_observer() {}
public:
	virtual void process_message(
		boost::shared_ptr<message> subject,
		void* ptr_arg
	) = 0;
};
#endif // MESSAGE_OBSERVER_H_
