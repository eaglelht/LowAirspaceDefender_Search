#ifndef BASE_IR_SENSOR_PROTOCOL_H_
#define BASE_IR_SENSOR_PROTOCOL_H_
#include "../i_protocol.h"

enum E_IR_Sensor_NUC_Type
{
	IR_Sensor_NT_Default = 1
};
enum E_IR_Sensor_Sync_Model
{
	IR_Sensor_SM_External = 1,
	IR_Sensor_SM_Internal
};

class base_ir_sensor_protocol_in
	: public i_protocol_in
{
protected:
	E_IR_Sensor_Sync_Model sync_model_;
public:
	base_ir_sensor_protocol_in() {}
	virtual ~base_ir_sensor_protocol_in() {}
public:
	virtual inline E_IR_Sensor_Sync_Model get_sync_model()
	{
		return sync_model_;
	}
};

class base_ir_sensor_protocol_out
	: public i_protocol_out
{
protected:
	E_IR_Sensor_Sync_Model sync_model_;
public:
	base_ir_sensor_protocol_out();
	virtual ~base_ir_sensor_protocol_out() {}

	virtual bool from_message(std::string msg_str) override;
public:
	virtual bool activate_nuc(E_IR_Sensor_NUC_Type type = IR_Sensor_NT_Default) = 0;
	virtual bool set_synchronization_model(E_IR_Sensor_Sync_Model model, bool save = false) = 0;
};

#endif // BASE_IR_SENSOR_PROTOCOL_H_
