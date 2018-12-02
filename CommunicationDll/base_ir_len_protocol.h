#ifndef BASE_IR_LEN_PROTOCOL_H_
#define BASE_IR_LEN_PROTOCOL_H_
#include "Protocols/i_protocol.h"

class base_ir_len_protocol_out
	: public i_protocol_out
{
	virtual bool focus_near() = 0;
	virtual bool focus_far() = 0;
	virtual bool focus_stop() = 0;
};

#endif // BASE_IR_LEN_PROTOCOL_H_
