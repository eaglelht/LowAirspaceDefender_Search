#ifndef SEARCH_SERVO_TYPE_001_SESSION_H_
#define SEARCH_SERVO_TYPE_001_SESSION_H_
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include "base_session.h"
#include "../Bases/mutex_list.h"

class search_servo_type_001_session
	: public base_session
{	
	boost::asio::steady_timer write_timer_;
	int write_interval_;

	mutex_list<boost::shared_ptr<message>> messages_;

	boost::posix_time::ptime pre_got_pack_time_;
	boost::posix_time::ptime pre_got_buffer_time_;
	unsigned int got_pack_count_;
	unsigned int got_bytes_count_;
public:
	search_servo_type_001_session(boost::asio::io_service& service);
	virtual ~search_servo_type_001_session();

	virtual void monitor_process() override;
	virtual void process_message(boost::shared_ptr<message> subject, void* ptr_arg = 0) override;
protected:
	virtual bool fn_on_read_process(int size) override;
	virtual bool fn_on_write_process() override;

	virtual bool start_write() override;

	bool do_wait_write_timer(int ms);
	void over_wait_write_timer();
	void on_wait_write_timer(const boost::system::error_code& error);

	bool translate_protocol_to_buffer();
	bool translate_protocol_from_buffer();
	void process_protocol();

	void compute_pps(boost::posix_time::ptime& now);
	void compute_bps(boost::posix_time::ptime& now);
};

#endif // SEARCH_SERVO_TYPE_001_SESSION_H_
