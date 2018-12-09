#ifndef LASER_TYPE_001_SESSION_H_
#define LASER_TYPE_001_SESSION_H_
#include "base_session.h"

class laser_type_001_session
	: public base_session
{
public:
	laser_type_001_session();
	virtual ~laser_type_001_session();

	virtual void monitor_process() override;
	virtual void process_message(boost::shared_ptr<message> subject, void* ptr_arg = 0) override;
protected:
	virtual bool fn_on_read_process(int size) override;
	virtual bool fn_on_write_process() override;

	bool translate_protocol_from_buffer();
	void process_protocol();
};

#endif // LASER_TYPE_001_SESSION_H_
