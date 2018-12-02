#ifndef SEARCH_TYPE_001_SERVO_PROTOCOL_H_
#define SEARCH_TYPE_001_SERVO_PROTOCOL_H_
#include "base_servo_protocol.h"

class search_type_001_servo_protocol_in
	: public base_servo_protocol_in
{
	int protocol_size_;
	bool linked_pack_check_;

	unsigned int tick_count_;
	unsigned int tick_count_error_times_;
public:
	search_type_001_servo_protocol_in();
	virtual ~search_type_001_servo_protocol_in();

	virtual bool from_bin(unsigned char* buffer, int& len) override;
	virtual std::string to_string() override;
protected:
	bool find_protocol_index(const unsigned char* buffer,
		const int len, int& protocol_index);
	bool translate_buffer(const unsigned char* buffer,
		int& translate_len);
	void post_process_buffer(unsigned char* buffer,
		int& len, const int translate_len);
};


class search_type_001_servo_protocol_out
	: public base_servo_protocol_out
{
public:
	virtual int to_bin(unsigned char* buffer, int len) override;
	virtual bool need_to_translate() override;
	virtual std::string to_string() override;
	virtual bool set_value(std::string name, double value) override;
	virtual bool set_value(std::string name, int value) override;
	virtual bool set_value(std::string name, bool value) override;
};

#endif // SEARCH_TYPE_001_SERVO_PROTOCOL_H_
