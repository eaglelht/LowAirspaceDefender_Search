#ifndef SEARCH_TYPE_001_IR_LEN_PROTOCOL_H_
#define SEARCH_TYPE_001_IR_LEN_PROTOCOL_H_
#include "base_ir_len_protocol.h"

class search_type_001_ir_len_protocol_out
	: public base_ir_len_protocol_out
{
	bool has_new_message_;
public:
	search_type_001_ir_len_protocol_out();
	virtual ~search_type_001_ir_len_protocol_out() {}
protected:
	virtual bool focus_far() override;
	virtual bool focus_near() override;
	virtual bool focus_stop() override;
	virtual bool mirror_rotating_start() override;
	virtual bool mirror_rotating_stop() override;
public:
	virtual bool need_to_translate() override;
	virtual bool set_value(std::string name, bool value) override;
	virtual bool set_value(std::string name, double value) override;
	virtual bool set_value(std::string name, int value) override;
	virtual int to_bin(unsigned char* buffer, int len) override;
	virtual std::string to_string() override;
};

#endif // SEARCH_TYPE_001_IR_LEN_PROTOCOL_H_
