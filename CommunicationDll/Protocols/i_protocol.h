#ifndef I_PROTOCOL_H_
#define I_PROTOCOL_H_
#include <string>

class i_protocol_in
{
public:
	i_protocol_in() {}
	virtual ~i_protocol_in() {}
public:
	virtual std::string to_message() = 0;
	virtual bool from_bin(unsigned char* buffer, int& len) = 0;
	virtual std::string to_string() = 0;
	virtual bool is_changed() = 0;
};

class i_protocol_out
{
public:
	i_protocol_out() {}
	virtual ~i_protocol_out() {}
public:
	//************************************
	// Method:    from_message
	// FullName:  i_protocol_out::from_message
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier: 从字符串类型的消息转换成实际的协议类型
	// Parameter: std::string msg_str
	//************************************
	virtual bool from_message(std::string msg_str) = 0;
	//************************************
	// Method:    need_to_translate
	// FullName:  i_protocol_out::need_to_translate
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier: 是否存在需要翻译成bin的协议数据来发送
	//************************************
	virtual bool need_to_translate() = 0;
	//************************************
	// Method:    to_bin
	// FullName:  i_protocol_out::to_bin
	// Access:    virtual public 
	// Returns:   int
	// Qualifier: 可能存在需要转换成bin数据的协议，则进行
	// 翻译转换，否则可能会返回0
	// Parameter: unsigned char * buffer
	// Parameter: int len
	//************************************
	virtual int to_bin(unsigned char* buffer, int len) = 0;
	virtual std::string to_string() = 0;
	virtual bool set_value(std::string name, double value) = 0;
	virtual bool set_value(std::string name, int value) = 0;
	virtual bool set_value(std::string name, bool value) = 0;
};

#endif // I_PROTOCOL_H_
