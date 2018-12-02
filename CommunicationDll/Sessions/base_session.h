#ifndef BASE_SESSION_H_
#define BASE_SESSION_H_
#include <set>
#include <list>
#include <boost/thread/shared_mutex.hpp>
#include <boost/atomic/atomic.hpp>
#include "../Bases/io_buffer_subject.h"
#include "../Bases/protocol_subject.h"
#include "../Bases/message_subject.h"
#include "../Bases/message_observer_.h"

class i_io;
class i_protocol_in;
class i_protocol_out;

template<class Y> class boost::weak_ptr;
template<class Y> class boost::shared_ptr;
template<class Y> struct io_buffer;

class base_session
	: public message_subject
	, public message_observer
	, public io_buffer_subject
	, public protocol_subject
{

	enum { default_buffer_size = 1024 };
protected:
	boost::atomic<bool> running_;

	boost::shared_ptr<i_io> io_;
	boost::shared_ptr<i_protocol_in> protocol_in_;
	boost::shared_ptr<i_protocol_out> protocol_out_;

	boost::shared_ptr<io_buffer<unsigned char>> in_buffer_;
	boost::shared_ptr<io_buffer<unsigned char>> out_buffer_;
public:
	base_session();
	virtual ~base_session();
public:
	virtual bool start();
	virtual bool stop();
	virtual void monitor_process() = 0;

	virtual bool set_io(boost::shared_ptr<i_io> sptr_io);
	virtual bool set_protocol_in(boost::shared_ptr<i_protocol_in> protocol);
	virtual bool set_protocol_out(boost::shared_ptr<i_protocol_out> protocol);

protected:
	virtual bool fn_on_read_process(int size) = 0;
	virtual bool fn_on_write_process() = 0;

	virtual bool open();
	virtual bool close();
	virtual bool reopen();

	virtual bool start_read();
	virtual bool start_write();

	virtual bool do_write();
	virtual bool do_read();
};

#endif // BASE_SESSION_H_
