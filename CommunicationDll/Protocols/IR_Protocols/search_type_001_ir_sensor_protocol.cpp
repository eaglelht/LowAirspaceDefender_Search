#include "stdafx.h"
#include "search_type_001_ir_sensor_protocol.h"
#include <boost/format.hpp>

search_type_001_ir_sensor_protocol_in::search_type_001_ir_sensor_protocol_in()
{
	min_protocol_size_ = 5;
}

search_type_001_ir_sensor_protocol_in::~search_type_001_ir_sensor_protocol_in()
{

}

bool search_type_001_ir_sensor_protocol_in::from_bin(unsigned char* buffer, int& len)
{
	// 判断数据的有效性
	if (buffer == 0 || len < min_protocol_size_)
	{
		return false;
	}
	bool pos_found = false;
	bool success_translate = false;
	int pos_index = -1;
	pos_found = find_protocol_index(buffer, len, pos_index);
	if (pos_found == true)
	{
		int translate_len = 0;
		// 翻译解析数据
		success_translate = translate_buffer(buffer + pos_index, translate_len);
		pos_index += translate_len;
	}
	else
	{
		;
	}
	// 后处理数据
	post_process_buffer(buffer, len, pos_index);
	return pos_found && success_translate;
}

bool search_type_001_ir_sensor_protocol_in::is_changed()
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::string search_type_001_ir_sensor_protocol_in::to_string()
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool search_type_001_ir_sensor_protocol_in::find_protocol_index(const unsigned char* buffer, const int len, int& protocol_index)
{
	protocol_index = 0;
	int head_index = 0;
	int tail_index = 0;
	const unsigned __int8* ptr_u8bits = 0;
	while (protocol_index + min_protocol_size_ <= len)
	{
		// 查看头
		head_index = protocol_index;
		ptr_u8bits = reinterpret_cast<const unsigned __int8*>
			(buffer + head_index);
		if (ptr_u8bits == 0
			|| (*ptr_u8bits) != C_STX)
		{
			++protocol_index;
			continue;
		}
		// 计算长度
		const unsigned __int8* ptr_byte_low = 
			reinterpret_cast<const unsigned __int8*>(buffer + head_index + 1);
		const unsigned __int8* ptr_byte_high = 
			reinterpret_cast<const unsigned __int8*>(buffer + head_index + 2);
		// 查看尾
		data_len_ = (*ptr_byte_high);
		data_len_ = (data_len_ << 8) | (*ptr_byte_low);
		tail_index = protocol_index + data_len_ + min_protocol_size_ - sizeof(unsigned char);
		ptr_u8bits = reinterpret_cast<const unsigned __int8*>
			(buffer + tail_index);
		if (ptr_u8bits == 0
			|| (*ptr_u8bits) != C_ETX)
		{
			++protocol_index;
			continue;
		}
		return true;
	}
	return false;
}

bool search_type_001_ir_sensor_protocol_in::translate_buffer(const unsigned char* buffer, int& translate_len)
{
	translate_len = data_len_ + min_protocol_size_ + 1;
	return true;
}

void search_type_001_ir_sensor_protocol_in::post_process_buffer(unsigned char* buffer, int& len, const int translate_len)
{
	if (linked_pack_check_)
	{
		len = 0;
	}
	else
	{
		if (len == translate_len)
		{
			len = 0;
		}
		else
		{
			memcpy_s(buffer, len,
				buffer + translate_len, len - translate_len);
			len -= translate_len;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

bool search_type_001_ir_sensor_protocol_out::activate_nuc(E_IR_Sensor_NUC_Type type /*= IR_Sensor_NT_Default*/)
{
	std::string cmd_str = "UN1,3,10,1,";
	list_strings_.push(cmd_str);
	return true;
}

bool search_type_001_ir_sensor_protocol_out::set_synchronization_model(E_IR_Sensor_Sync_Model model, bool save /*= false*/)
{
	boost::format fmt("SEX,%d");
	switch (model)
	{
	case IR_Sensor_SM_External:
	{
		fmt % 1;
		break;
	}
	case IR_Sensor_SM_Internal:
	{
		fmt % 0;
		break;
	}
	default:
	{
		fmt % 1;
		break;
	}
	}
	list_strings_.push(fmt.str());
	return true;
}

bool search_type_001_ir_sensor_protocol_out::need_to_translate()
{
	return (list_strings_.is_empty() == false);
}

int search_type_001_ir_sensor_protocol_out::to_bin(unsigned char* buffer, int len)
{
	if (list_strings_.is_empty())
	{
		return 0;
	}
	std::string cmd_str = list_strings_.pop();
	return format_command(cmd_str, buffer, len);
}

std::string search_type_001_ir_sensor_protocol_out::to_string()
{
	throw std::logic_error("The method or operation is not implemented.");
}

int search_type_001_ir_sensor_protocol_out::format_command(std::string str_cmd, unsigned char* buffer, const int buffer_len)
{
	if (buffer == 0
		|| buffer_len < 3)
	{
		return 0;
	}
	int buffer_index = 0;
	int sum_check = 0;
	int len = static_cast<int>(str_cmd.length());
	buffer[buffer_index++] = C_STX;
	buffer[buffer_index++] = (len & 0xFF);
	buffer[buffer_index++] = ((len >> 8) & 0xFF);
	//
	for (int i = 0;
		i < len && buffer_index < buffer_len - 2;
		++i, ++buffer_index)
	{
		unsigned char temp = static_cast<unsigned char>(str_cmd.at(i));
		buffer[buffer_index] = (temp & 0xFF);
		sum_check += temp;
	}
	if (buffer_index < buffer_len - 2)
	{
		buffer[buffer_index++] = (sum_check & 0xFF);
		buffer[buffer_index++] = C_ETX;
		return buffer_index;
	}
	else
	{
		return 0;
	}
}
