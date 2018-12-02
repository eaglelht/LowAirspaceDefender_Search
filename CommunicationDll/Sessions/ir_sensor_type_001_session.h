#ifndef IR_SENSOR_TYPE_001_SESSION_H_
#define IR_SENSOR_TYPE_001_SESSION_H_
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/atomic/atomic.hpp>
#include "base_session.h"
#include "../Bases/mutex_list.h"

class ir_sensor_type_001_session
	: public base_session
{
	boost::asio::steady_timer write_timer_;
	int write_interval_;

	mutex_list<boost::shared_ptr<message>> messages_;
public:
	ir_sensor_type_001_session(boost::asio::io_service& service);
	virtual ~ir_sensor_type_001_session();
public:
	virtual void monitor_process() override;
	virtual void process_message(boost::shared_ptr<message> subject, void* ptr_arg) override;
protected:
	virtual bool fn_on_read_process(int size) override;
	virtual bool fn_on_write_process() override;
protected:
	bool start_write_timer();
	bool do_wait_write_timer(int ms);
	void over_wait_write_timer();
	void on_wait_write_timer(const boost::system::error_code& error);

	bool has_to_translate();
	bool translate_message_to_protocol();
	bool translate_protocol_to_buffer();
	bool translate_protocol_from_buffer();
	void process_protocol();
};

#endif //IR_SENSOR_TYPE_001_SESSION_H_
