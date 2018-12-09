#include "stdafx.h"
#include "search_type_001_ir_len_protocol.h"

search_type_001_ir_len_protocol_out::search_type_001_ir_len_protocol_out()
{
	has_new_message_ = false;
}

bool search_type_001_ir_len_protocol_out::focus_far()
{
	focus_ = E_IR_LFS_Far;
	has_new_message_ = true;
	return true;
}

bool search_type_001_ir_len_protocol_out::focus_near()
{
	focus_ = E_IR_LFS_Near;
	has_new_message_ = true;
	return true;
}

bool search_type_001_ir_len_protocol_out::focus_stop()
{
	focus_ = E_IR_LFS_Stop;
	has_new_message_ = true;
	return true;
}

bool search_type_001_ir_len_protocol_out::mirror_rotating_start()
{
	mirror_ = E_IR_Len_MS_Start;
	has_new_message_ = true;
	return true;
}

bool search_type_001_ir_len_protocol_out::mirror_rotating_stop()
{
	mirror_ = E_IR_Len_MS_Stop;
	has_new_message_ = true;
	return true;
}

bool search_type_001_ir_len_protocol_out::need_to_translate()
{
	return has_new_message_;
}

bool search_type_001_ir_len_protocol_out::set_value(std::string name, bool value)
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool search_type_001_ir_len_protocol_out::set_value(std::string name, double value)
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool search_type_001_ir_len_protocol_out::set_value(std::string name, int value)
{
	throw std::logic_error("The method or operation is not implemented.");
}

int search_type_001_ir_len_protocol_out::to_bin(unsigned char* buffer, int len)
{
	if (buffer == 0)
	{
		return 0;
	}
	int index = 0;
	int sum = 0;
	unsigned __int8* ptr_u8bit = 0;
	// 1
	ptr_u8bit = reinterpret_cast<unsigned __int8*>
		(buffer + index);
	(*ptr_u8bit) = 0x7E;
	index += 1;
	// 2
	ptr_u8bit = reinterpret_cast<unsigned __int8*>
		(buffer + index);
	(*ptr_u8bit) = 0x04;
	sum += (*ptr_u8bit);
	index += 1;
	// 3
	ptr_u8bit = reinterpret_cast<unsigned __int8*>
		(buffer + index);
	switch (focus_)
	{
	case E_IR_LFS_Far:
	{
		(*ptr_u8bit) = 0x02;
		break;
	}
	case E_IR_LFS_Near:
	{
		(*ptr_u8bit) = 0x01;
		break;
	}
	default:
	{
		(*ptr_u8bit) = 0x00;
		break;
	}
	}	
	sum += (*ptr_u8bit);
	index += 1;
	// 4
	ptr_u8bit = reinterpret_cast<unsigned __int8*>
		(buffer + index);
	switch (mirror_)
	{
	case E_IR_Len_MS_Stop:
	{
		(*ptr_u8bit) = 0x00;
		break;
	}
	case E_IR_Len_MS_Start:
	{
		(*ptr_u8bit) = 0x01;
		break;
	}
	default:
	{
		(*ptr_u8bit) = 0x00;
		break;
	}
	}
	sum += (*ptr_u8bit);
	index += 1;
	// 5
	ptr_u8bit = reinterpret_cast<unsigned __int8*>
		(buffer + index);
	(*ptr_u8bit) = 0x00;
	sum += (*ptr_u8bit);
	index += 1;
	// 6
	ptr_u8bit = reinterpret_cast<unsigned __int8*>
		(buffer + index);
	(*ptr_u8bit) = 0x00;
	sum += (*ptr_u8bit);
	index += 1;
	// 7
	ptr_u8bit = reinterpret_cast<unsigned __int8*>
		(buffer + index);
	(*ptr_u8bit) = 0x00;
	sum += (*ptr_u8bit);
	index += 1;
	// 8
	ptr_u8bit = reinterpret_cast<unsigned __int8*>
		(buffer + index);
	(*ptr_u8bit) = 0x00;
	sum += (*ptr_u8bit);
	index += 1;
	// 9
	ptr_u8bit = reinterpret_cast<unsigned __int8*>
		(buffer + index);
	(*ptr_u8bit) = 0x00;
	sum += (*ptr_u8bit);
	index += 1;
	// 10
	ptr_u8bit = reinterpret_cast<unsigned __int8*>
		(buffer + index);
	(*ptr_u8bit) = 0x00;
	sum += (*ptr_u8bit);
	index += 1;
	// 11
	ptr_u8bit = reinterpret_cast<unsigned __int8*>
		(buffer + index);
	(*ptr_u8bit) = static_cast<unsigned __int8>(sum);
	index += 1;
	// 12
	ptr_u8bit = reinterpret_cast<unsigned __int8*>
		(buffer + index);
	(*ptr_u8bit) = 0xE7;
	index += 1;

	return index;
}

std::string search_type_001_ir_len_protocol_out::to_string()
{
	throw std::logic_error("The method or operation is not implemented.");
}
