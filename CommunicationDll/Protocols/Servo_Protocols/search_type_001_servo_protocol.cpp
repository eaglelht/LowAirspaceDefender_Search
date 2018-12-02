#include "stdafx.h"
#include "search_type_001_servo_protocol.h"
#include "../../Json/value.h"

search_type_001_servo_protocol_in::search_type_001_servo_protocol_in()
{
	protocol_size_ = 14;
	linked_pack_check_ = true;

	tick_count_ = 0;
	tick_count_error_times_ = 0;
}

search_type_001_servo_protocol_in::~search_type_001_servo_protocol_in()
{

}

bool search_type_001_servo_protocol_in::from_bin(unsigned char* buffer, int& len)
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

std::string search_type_001_servo_protocol_in::to_string()
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool search_type_001_servo_protocol_in::find_protocol_index(const unsigned char* buffer, const int len, int& protocol_index)
{
	protocol_index = 0;
	int head_index = 0;
	int tail_index = 0;
	const unsigned __int16* ptr_u16bits = 0;

	while (protocol_index + protocol_size_ <= len)
	{
		// 查看头
		head_index = protocol_index;
		ptr_u16bits = reinterpret_cast<const unsigned __int16*>
			(buffer + head_index);
		if (ptr_u16bits == 0
			|| (*ptr_u16bits) != 0xE7E7)
		{
			++protocol_index;
			continue;
		}
		// 查看尾
		tail_index = protocol_index + protocol_size_ - sizeof(unsigned __int16);
		ptr_u16bits = reinterpret_cast<const unsigned __int16*>
			(buffer + tail_index);
		if (ptr_u16bits == 0
			|| (*ptr_u16bits) != 0x7E7E)
		{
			++protocol_index;
			continue;
		}
		return true;
	}
	return false;
}

bool search_type_001_servo_protocol_in::translate_buffer(const unsigned char* buffer, int& translate_len)
{
	double azimuth = 0;
	double elevation = 0;
	E_ServoWorkPattern pattern;
	unsigned int tick_count;
	//
	translate_len = 0;
	const unsigned __int16* ptr_u16bit = 0;
	const __int16* ptr_16bit = 0;
	const unsigned __int8* ptr_u8bit = 0;
	// 1 - 2 头
	translate_len += 2;
	// 3 状态
	translate_len += 1;
	// 4 - 5 方位
	ptr_u16bit = reinterpret_cast<const unsigned __int16*>
		(buffer + translate_len);
	azimuth = static_cast<double>((*ptr_u16bit) * 0.01);
	translate_len += 2;
	// 6 - 7 俯仰
	ptr_16bit = reinterpret_cast<const __int16*>
		(buffer + translate_len);
	elevation = static_cast<double>((*ptr_16bit) * 0.01);
	translate_len += 2;
	// 8 模式
	ptr_u8bit = reinterpret_cast<const unsigned __int8*>
		(buffer + translate_len);
	pattern = static_cast<E_ServoWorkPattern>(*ptr_u8bit);
	translate_len += 1;
	// 9 计数
	ptr_u8bit = reinterpret_cast<const unsigned __int8*>
		(buffer + translate_len);
	tick_count = static_cast<unsigned int>(*ptr_u8bit);
	translate_len += 1;
	// 10 - 11 
	translate_len += 2;
	// 12
	translate_len += 1;
	// 13 - 14 尾
	translate_len += 2;
	//
	if (tick_count_ == tick_count)
	{
		++tick_count_error_times_;
	}
	else
	{
		tick_count_error_times_ = 0;
	}
	tick_count_ = tick_count;
	if (tick_count_error_times_ > 5)
	{
		return false;
	}
	if (pattern_ != pattern)
	{
		has_value_changed_ = true;
	}
	pattern_ = pattern;
	if (fabs(azimuth_ - azimuth) >= 0.01)
	{
		has_value_changed_ = true;
	}
	azimuth_ = azimuth;
	if (fabs(elevation_ - elevation) >= 0.01)
	{
		has_value_changed_ = true;
	}
	elevation_ = elevation;
	return true;
}

void search_type_001_servo_protocol_in::post_process_buffer(unsigned char* buffer, int& len, const int translate_len)
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

int search_type_001_servo_protocol_out::to_bin(unsigned char* buffer, int len)
{
	if (buffer == 0)
	{
		return 0;
	}
	int index = 0;
	unsigned __int16* ptr_u16bit = 0;
	__int16* ptr_16bit = 0;
	unsigned __int8* ptr_u8bit = 0;
	// 1 - 2 头
	ptr_u16bit = reinterpret_cast<unsigned __int16*>
		(buffer + index);
	(*ptr_u16bit) = static_cast<unsigned __int16>(0x7E7E);
	index += 2;
	// 3 模式
	ptr_u8bit = reinterpret_cast<unsigned __int8*>
		(buffer + index);
	*ptr_u8bit = static_cast<unsigned __int8>(pattern_);
	index += 1;
	// 4 - 7
	switch (pattern_)
	{
	case Servo_WP_Stop:
	{
		ptr_u16bit = reinterpret_cast<unsigned __int16*>
			(buffer + index);
		*ptr_u16bit = 0;
		index += 2;
		ptr_u16bit = reinterpret_cast<unsigned __int16*>
			(buffer + index);
		*ptr_u16bit = 0;
		index += 2;
		break;
	}
	case Servo_WP_ManualMove:
	{
		ptr_16bit = reinterpret_cast<__int16*>
			(buffer + index);
		__int16 temp = static_cast<__int16>(manual_azimuth_speed_ * 100);
		temp = (temp > 32767) ? 32767 : ((temp < -32767) ? -32767 : temp);
		*ptr_16bit = temp;
		index += 2;
		ptr_16bit = reinterpret_cast<__int16*>
			(buffer + index);
		*ptr_16bit = static_cast<__int16>(manual_elevation_speed_ * 100);
		index += 2;
		break;
	}
	case Servo_WP_Preset:
	{
		ptr_u16bit = reinterpret_cast<unsigned __int16*>
			(buffer + index);
		*ptr_u16bit = static_cast<unsigned __int16>(preset_azimuth_ * 100);
		index += sizeof(unsigned __int16);
		ptr_16bit = reinterpret_cast<__int16*>
			(buffer + index);
		*ptr_16bit = static_cast<__int16>(preset_elevation_ * 100);
		index += sizeof(__int16);
		break;
	}
	case Servo_WP_Track:
	{
		ptr_u16bit = reinterpret_cast<unsigned __int16*>
			(buffer + index);
		*ptr_u16bit = static_cast<unsigned __int16>(track_img_x_offset_);
		index += 2;
		ptr_16bit = reinterpret_cast<__int16*>
			(buffer + index);
		*ptr_16bit = static_cast<__int16>(track_img_y_offset_);
		index += 2;
		break;
	}
	case Servo_WP_SearchTypeOne:
	case Servo_WP_SearchTypeTwo:
	case Servo_WP_SearchTypeThree:
	case Servo_WP_SearchTypeFour:
	{
		ptr_u16bit = reinterpret_cast<unsigned __int16*>
			(buffer + index);
		*ptr_u16bit = static_cast<unsigned __int16>(search_azimuth_ * 100);
		index += 2;
		ptr_16bit = reinterpret_cast<__int16*>
			(buffer + index);
		*ptr_16bit = static_cast<__int16>(search_elevation_ * 100);
		index += 2;
		break;
	}
	case Servo_WP_Search2Track:
	{
		ptr_u16bit = reinterpret_cast<unsigned __int16*>
			(buffer + index);
		*ptr_u16bit = static_cast<unsigned __int16>(search_to_track_azimuth_ * 100);
		index += 2;
		ptr_16bit = reinterpret_cast<__int16*>
			(buffer + index);
		*ptr_16bit = static_cast<__int16>(search_to_track_elevation_ * 100);
		index += 2;
		break;
	}
	default:
	{
		ptr_u16bit = reinterpret_cast<unsigned __int16*>
			(buffer + index);
		*ptr_u16bit = 0;
		index += 2;
		ptr_u16bit = reinterpret_cast<unsigned __int16*>
			(buffer + index);
		*ptr_u16bit = 0;
		index += 2;
		break;
	}
	}
	// 8 - 9 
	ptr_16bit = reinterpret_cast<__int16*>
		(buffer + index);
	*ptr_16bit = 0;
	index += 2;
	// 10
	ptr_u8bit = reinterpret_cast<unsigned __int8*>
		(buffer + index);
	*ptr_u8bit = 0;
	index += 1;
	// 11 - 12 尾
	ptr_u16bit = reinterpret_cast<unsigned __int16*>
		(buffer + index);
	*ptr_u16bit = static_cast<unsigned __int16>(0xE7E7);
	index += 2;

	return index;
}

bool search_type_001_servo_protocol_out::need_to_translate()
{
	throw std::logic_error("The method or operation is not implemented.");
}

std::string search_type_001_servo_protocol_out::to_string()
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool search_type_001_servo_protocol_out::set_value(std::string name, double value)
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool search_type_001_servo_protocol_out::set_value(std::string name, int value)
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool search_type_001_servo_protocol_out::set_value(std::string name, bool value)
{
	throw std::logic_error("The method or operation is not implemented.");
}
