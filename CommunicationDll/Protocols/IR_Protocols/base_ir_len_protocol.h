#ifndef BASE_IR_LEN_PROTOCOL_H_
#define BASE_IR_LEN_PROTOCOL_H_
#include "../i_protocol.h"

enum E_IR_Len_Focus_State
{
	E_IR_LFS_Stop = 0,
	E_IR_LFS_Near,
	E_IR_LFS_Far
};

enum E_IR_Len_Mirror_State
{
	E_IR_Len_MS_Stop = 0,
	E_IR_Len_MS_Start
};

class base_ir_len_protocol_out
	: public i_protocol_out
{
protected:
	E_IR_Len_Focus_State focus_;
	E_IR_Len_Mirror_State mirror_;
public:
	base_ir_len_protocol_out();
	virtual ~base_ir_len_protocol_out() {}
public:
	virtual bool from_message(std::string msg_str) override;
protected:
	virtual bool focus_near() = 0;
	virtual bool focus_far() = 0;
	virtual bool focus_stop() = 0;

	virtual bool mirror_rotating_start() = 0;
	virtual bool mirror_rotating_stop() = 0;
};

#endif // BASE_IR_LEN_PROTOCOL_H_
