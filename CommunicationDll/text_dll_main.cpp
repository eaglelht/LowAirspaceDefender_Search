#include "stdafx.h"
#include <stdio.h>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/thread.hpp>
#include "Protocols/i_protocol.h"
#include "IOs/i_io.h"
#include "Sessions/base_session.h"
#include "Json/value.h"
#include "Bases/message.h"
#include "Protocols/protocol_factory.h"
#include "IOs/io_factory.h"
#include "Sessions/session_factory.h"

boost::asio::io_service g_service;
boost::asio::steady_timer g_monitor_timer(g_service);

void service_run()
{
	try
	{
		g_service.run();
	}
	catch (const boost::system::system_error&)
	{

	}
}
void do_monitor(int ms, boost::shared_ptr<base_session> session);
void on_monitor(const boost::system::error_code& error,
	boost::shared_ptr<base_session> session)
{
	if (error)
	{
		return;
	}
	session->monitor_process();
	do_monitor(1000, session);
}
void do_monitor(int ms, boost::shared_ptr<base_session> session)
{
#if _MSC_VER <= 1600
	g_monitor_timer.expires_from_now(boost::chrono::milliseconds(ms));
#else
	g_monitor_timer.expires_from_now(std::chrono::milliseconds(ms));
#endif	
	g_monitor_timer.async_wait(
		boost::bind(&on_monitor, 
			boost::asio::placeholders::error, session));
}


int main()
{
	register_protocol_factory();
	register_io_factory();
	regster_session_factory();

	protocol_in_factory& pif = singleton_protocol_in_factory::Instance();
	protocol_out_factory& pof = singleton_protocol_out_factory::Instance();
	io_factory& iof = singleton_io_factory::Instance();
	session_factory& sf = singleton_session_factory::Instance();

	boost::shared_ptr<i_protocol_in> protocol_in(
		pif.CreateObject("servo_type_001")
	);
	boost::shared_ptr<i_protocol_out> protocol_out(
		pof.CreateObject("servo_type_001")
	);
	boost::shared_ptr<i_io> io(
		iof.CreateObject("udp", g_service)
	);
	//
	Json::Value info;
	info["type"] = "udp";
	info["read_ip"] = "127.0.0.1";
	info["read_port"] = "9301";
	info["write_ip"] = "127.0.0.1";
	info["write_port"] = "9302";
	//
	if (io->init(info.toStyledString()) == false)
	{
		return -1;
	}
	boost::shared_ptr<base_session> session(
		sf.CreateObject("servo", g_service)
	);
	if (session->set_protocol_in(protocol_in) == false
		|| session->set_protocol_out(protocol_out) == false
		|| session->set_io(io) == false)
	{
		return -2;
	}
	//
	boost::shared_ptr<message> msg
		= boost::make_shared<message>();
	msg->main_type_ = message::E_MMT_Command;
	msg->sub_type_ = message::E_MST_Servo;
	Json::Value cmd;
	cmd["param_1"] = "109.23";
	cmd["param_2"] = "10.23";
	cmd["command"] = "preset";
	msg->information_ = cmd.toStyledString();
	session->process_message(msg, 0);
	//
	do_monitor(0, session);
	//boost::thread thead_run(
	//	service_run
	//);
	boost::thread_group threads;
	threads.create_thread(service_run);
	threads.create_thread(service_run);
	session->start();
	//	
	getchar();
	g_monitor_timer.cancel();
	session->stop();
	//thead_run.join();
	threads.join_all();
	return 1;
}