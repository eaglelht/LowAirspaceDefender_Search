#ifndef SEARCH_TYPE_001_IR_SENSOR_PROTOCOL_H_
#define SEARCH_TYPE_001_IR_SENSOR_PROTOCOL_H_
#include <string>
#include "base_sensor_protocol.h"
#include "../../Bases/mutex_list.h"

class search_type_001_ir_sensor_protocol_in
	: public base_ir_sensor_protocol_in
{
	enum { C_STX = 2, C_ETX = 3 };
	int min_protocol_size_;
	bool linked_pack_check_;
	int data_len_;
public:
	search_type_001_ir_sensor_protocol_in();
	virtual ~search_type_001_ir_sensor_protocol_in();
	virtual bool from_bin(unsigned char* buffer, int& len) override;
	virtual bool is_changed() override;
	virtual std::string to_string() override;
protected:
	bool find_protocol_index(const unsigned char* buffer,
		const int len, int& protocol_index);
	bool translate_buffer(const unsigned char* buffer,
		int& translate_len);
	void post_process_buffer(unsigned char* buffer,
		int& len, const int translate_len);
};

class search_type_001_ir_sensor_protocol_out
	: public base_ir_sensor_protocol_out
{
	enum { C_STX = 2, C_ETX = 3 };
	mutex_list<std::string> list_strings_;
public:
	virtual bool activate_nuc(E_IR_Sensor_NUC_Type type = IR_Sensor_NT_Default) override;
	virtual bool set_synchronization_model(E_IR_Sensor_Sync_Model model, bool save = false) override;
	virtual bool need_to_translate() override;
	virtual int to_bin(unsigned char* buffer, int len) override;
	virtual std::string to_string() override;
protected:
	int format_command(std::string str_cmd,
		unsigned char* buffer, const int buffer_len);
};

#endif // SEARCH_TYPE_001_IR_SENSOR_PROTOCOL_H_
