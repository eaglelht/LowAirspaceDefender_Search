#include "stdafx.h"
#include "laser_type_JDIV4C_protocol.h"
#include <boost/date_time/posix_time/posix_time_types.hpp>

laser_type_JDIV4C_protocol_in::laser_type_JDIV4C_protocol_in()
{
	protocol_size_ = 14;
	linked_pack_check_ = true;
}

bool laser_type_JDIV4C_protocol_in::from_bin(unsigned char* buffer, int& len)
{
	// 判断数据的有效性
	if (buffer == 0 || len < protocol_size_)
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

bool laser_type_JDIV4C_protocol_in::is_changed()
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::string laser_type_JDIV4C_protocol_in::to_message()
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::string laser_type_JDIV4C_protocol_in::to_string()
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool laser_type_JDIV4C_protocol_in::find_protocol_index(const unsigned char* buffer, const int len, int& protocol_index)
{
	protocol_index = 0;
	int head_index = 0;
	int tail_index = 0;
	const unsigned __int8* ptr_u8bits = 0;

	while (protocol_index + protocol_size_ <= len)
	{
		head_index = protocol_index;
		ptr_u8bits = reinterpret_cast<const unsigned __int8*>
			(buffer + head_index);

		if ((ptr_u8bits == 0) 
			|| ((*ptr_u8bits) != '#'))
		{
			++protocol_index;
			continue;
		}

		tail_index = protocol_index + protocol_size_ 
			- sizeof(const unsigned __int8);
		ptr_u8bits =
			reinterpret_cast<const unsigned __int8*>
			(buffer + tail_index);
		if ((ptr_u8bits == 0) 
			|| ((*ptr_u8bits) != '!'))
		{
			++protocol_index;
			continue;
		}
		return true;
	}
	return false;
}

bool laser_type_JDIV4C_protocol_in::translate_buffer(const unsigned char* buffer, int& translate_len)
{
	translate_len = 0;
	const unsigned __int16* ptr_u16bit = 0;
	const unsigned __int8* ptr_u8bit = 0;
	// 1 头
	translate_len += 1;
	// 2 数据响应位
	translate_len += 1;
	// 3 故障状态位
	ptr_u8bit = (buffer + translate_len);
	unsigned __int8 temp_value = (*ptr_u8bit);
	dp_1_error_ = ((temp_value & 0x80) == 0) ? true : false;
	apd_error_ = ((temp_value & 0x40) == 0) ? true : false;
	mw_error_ = ((temp_value & 0x20) == 0) ? true : false;
	power_error_ = ((temp_value & 0x10) == 0) ? true : false;
	during_timeout_ = ((temp_value & 0x08) == 0) ? true : false;
	dp_2_error_ = ((temp_value & 0x04) == 0) ? true : false;
	translate_len += 1;
	// 4 - 5 近距离目标
	ptr_u16bit = reinterpret_cast<const unsigned __int16*>
		(buffer + translate_len);
	unsigned __int16 temp = (*ptr_u16bit);
	boost::posix_time::ptime now 
		= boost::posix_time::microsec_clock::local_time();
	if (temp == 0 || temp == 65535)
	{
		original_distance_ = 0;
	} 
	else
	{
		original_distance_ = static_cast<double>(temp);
		if (fabs(valid_distance_) > 0.001)
		{
			double distance_diff = original_distance_ - valid_distance_;
			double change_rate = distance_diff / (now - got_valid_distance_time_).total_milliseconds();
			if (fabs(change_rate) < 0.05)
			{
				valid_distance_ = original_distance_;
				got_valid_distance_time_ = now;
				valid_distance_change_rate_ = change_rate;
			}
			else
			{ }
		}
		else
		{
			valid_distance_ = original_distance_;
			got_valid_distance_time_ = now;
		}
	}
	// 添加处理
	translate_len += 2;
	// 6 - 7 远距离目标
	translate_len += 2;
	// 8 尾
	translate_len += 1;

	return true;
}

void laser_type_JDIV4C_protocol_in::post_process_buffer(unsigned char* buffer, int& len, const int translate_len)
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

bool laser_type_JDIV4C_protocol_out::power_off()
{
	int index = 0;
	command_buffer_[index++] = 0x05;
	command_buffer_[index++] = 0x00;
	command_buffer_[index++] = 0x00;
	command_buffer_size_ = 3;
	has_new_message_ = true;
	return true;
}

bool laser_type_JDIV4C_protocol_out::power_on()
{
	int index = 0;
	command_buffer_[index++] = 0x05;
	command_buffer_[index++] = 0xFF;
	command_buffer_[index++] = 0xFF;
	command_buffer_size_ = 3;
	has_new_message_ = true;
	return true;
}

bool laser_type_JDIV4C_protocol_out::shoot_once()
{
	int index = 0;
	command_buffer_[index++] = 0x00;
	command_buffer_[index++] = 0x00;
	command_buffer_[index++] = 0x00;
	command_buffer_size_ = 3;
	has_new_message_ = true;
	return true;
}

bool laser_type_JDIV4C_protocol_out::need_to_translate()
{
	return has_new_message_;
}

bool laser_type_JDIV4C_protocol_out::set_value(std::string name, bool value)
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool laser_type_JDIV4C_protocol_out::set_value(std::string name, double value)
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool laser_type_JDIV4C_protocol_out::set_value(std::string name, int value)
{
	throw std::logic_error("The method or operation is not implemented.");
}

int laser_type_JDIV4C_protocol_out::to_bin(unsigned char* buffer, int len)
{
	if (buffer == 0 || (command_buffer_size_ + 2) > len)
	{
		return 0;
	}
	int index = 0;
	unsigned __int8* ptr_u8bit = 0;
	// head
	ptr_u8bit = reinterpret_cast<unsigned __int8*>
		(buffer + index);
	(*ptr_u8bit) = 0x23;
	index += 1;
	// command
	for (int i = 0; i < command_buffer_size_; ++i)
	{
		ptr_u8bit = reinterpret_cast<unsigned __int8*>
			(buffer + index);
		(*ptr_u8bit) = command_buffer_[i];
		index += 1;
	}
	// tail
	ptr_u8bit = reinterpret_cast<unsigned __int8*>
		(buffer + index);
	(*ptr_u8bit) = 0x21;
	index += 1;
	return index;
}

std::string laser_type_JDIV4C_protocol_out::to_string()
{
	throw std::logic_error("The method or operation is not implemented.");
}
