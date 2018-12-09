#ifndef IR_LEN_TYPE_001_SESSION_H_
#define IR_LEN_TYPE_001_SESSION_H_
#include "base_session.h"

class ir_len_type_001_session
	: public base_session
{
public:
	ir_len_type_001_session();
	virtual ~ir_len_type_001_session();


	virtual void monitor_process() override;


	virtual void process_message(boost::shared_ptr<message> subject, void* ptr_arg = 0) override;

	

protected:
	virtual bool fn_on_read_process(int size) override;


	virtual bool fn_on_write_process() override;

	bool translate_message_to_protocol(boost::shared_ptr<message> subject);
	bool translate_protocol_to_buffer();

};

#endif // IR_LEN_TYPE_001_SESSION_H_
