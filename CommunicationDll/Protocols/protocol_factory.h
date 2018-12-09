#ifndef PROTOCOL_FACTORY_H_
#define PROTOCOL_FACTORY_H_
#include <string>
#include "../Bases/loki/Factory.h"
#include "../Bases/loki/Sequence.h"
#include "i_protocol.h"
#include "Servo_Protocols/search_type_001_servo_protocol.h"
#include "IR_Protocols/search_type_001_ir_len_protocol.h"
#include "IR_Protocols/search_type_001_ir_sensor_protocol.h"

typedef Loki::Factory
<
	i_protocol_in,
	std::string
> protocol_in_factory;

typedef Loki::Factory
<
	i_protocol_out,
	std::string
> protocol_out_factory;

typedef Loki::SingletonHolder
<
	protocol_in_factory,
	Loki::CreateUsingNew,
	Loki::LongevityLifetime::DieAsSmallObjectChild
> singleton_protocol_in_factory;

typedef Loki::SingletonHolder
<
	protocol_out_factory,
	Loki::CreateUsingNew,
	Loki::LongevityLifetime::DieAsSmallObjectChild
> singleton_protocol_out_factory;

inline bool register_protocol_factory()
{
	protocol_in_factory& in_factory =
		singleton_protocol_in_factory::Instance();
	protocol_out_factory& out_factory =
		singleton_protocol_out_factory::Instance();

	bool res = true;

	res = res && in_factory.Register("servo_type_001",
		Loki::CreateUsingNew<search_type_001_servo_protocol_in>::Create);
	res = res && out_factory.Register("servo_type_001",
		Loki::CreateUsingNew<search_type_001_servo_protocol_out>::Create);

	//res = res && out_factory.Register("ir_len_type_001",
	//	Loki::CreateUsingNew<search_type_001_ir_len_protocol_out>::Create());

	//res = res && in_factory.Register("ir_sensor_type_001",
	//	Loki::CreateUsingNew<search_type_001_ir_sensor_protocol_in>::Create());
	//res = res && out_factory.Register("ir_sensor_type_001",
	//	Loki::CreateUsingNew<search_type_001_ir_sensor_protocol_out>::Create());

	return res;
}


#endif // PROTOCOL_FACTORY_H_
