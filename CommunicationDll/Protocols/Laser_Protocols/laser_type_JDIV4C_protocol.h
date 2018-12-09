#ifndef LASER_TYPE_JDIV4C_PROTOCOL_H_
#define LASER_TYPE_JDIV4C_PROTOCOL_H_
#include "base_laser_protocol.h"
#include <array>

class laser_type_JDIV4C_protocol_in
	: public base_laser_protocol_in
{
protected:
	int protocol_size_;
	bool linked_pack_check_;

	bool dp_1_error_; // 信号处理电路1故障
	bool dp_2_error_; // 信号处理电路2故障
	bool apd_error_; // APD电压故障
	bool mw_error_; // 主波故障
	bool power_error_; // 激光电源故障
	bool during_timeout_; // 一次连续工作超时
public:
	laser_type_JDIV4C_protocol_in();
public:
	virtual bool from_bin(unsigned char* buffer, int& len) override;
	virtual bool is_changed() override;
	virtual std::string to_message() override;
	virtual std::string to_string() override;
protected:
	bool find_protocol_index(const unsigned char* buffer,
		const int len, int& protocol_index);
	bool translate_buffer(const unsigned char* buffer,
		int& translate_len);
	void post_process_buffer(unsigned char* buffer,
		int& len, const int translate_len);
};

class laser_type_JDIV4C_protocol_out
	: public base_laser_protocol_out
{
	enum { max_buffer_size = 256 };
	std::array<unsigned char, max_buffer_size> command_buffer_;
	int command_buffer_size_;
	bool has_new_message_;
protected:
	virtual bool power_off() override;
	virtual bool power_on() override;
	virtual bool shoot_once() override;
public:
	virtual bool need_to_translate() override;
	virtual bool set_value(std::string name, bool value) override;
	virtual bool set_value(std::string name, double value) override;
	virtual bool set_value(std::string name, int value) override;
	virtual int to_bin(unsigned char* buffer, int len) override;
	virtual std::string to_string() override;
};

#endif // LASER_TYPE_JDIV4C_PROTOCOL_H_
