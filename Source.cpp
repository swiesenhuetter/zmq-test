#include <string>
#include <zmq.hpp>
#include <iostream>
#include <thread>
#include "Header.h"


int main()
{
	int major = 0;
	int minor = 0;
	int patch = 0;
	zmq_version(&major, &minor, &patch);
	std::wcout << "Current Zero MQ version is " << major << '.' << minor << '.' << patch << '\n';

	zmq::context_t ctx;

	bool stoken = false;
	std::thread t1([&]()
		{
			zmq::socket_t receiver(ctx, zmq::socket_type::pair);
			receiver.connect("inproc://test");
			auto msg = zmq::message_t();
			while (!stoken) {
				std::this_thread::sleep_for(std::chrono::seconds(1));
				auto received = receiver.recv(msg, zmq::recv_flags::none);
				auto pers = msgpack::unpack<TestRecord>(static_cast<uint8_t*>(msg.data()), msg.size());
				std::wcout << "Received : " << pers << "\n";

			}
			receiver.send(zmq::buffer("Hello from thread 1"), zmq::send_flags::none);
			std::cout << "Exit thread 1\n";  
		});

	zmq::socket_t sock(ctx, zmq::socket_type::pair);
	zmq::socket_t py_sock(ctx, zmq::socket_type::pair);
	sock.bind("inproc://test");
	py_sock.connect("tcp://localhost:5555");
	
	TestRecord person;
	auto sbuf = msgpack::pack(person);
	
	sock.send(zmq::message_t(sbuf), zmq::send_flags::none);

	py_sock.send(zmq::message_t(sbuf), zmq::send_flags::none);

	
	std::this_thread::sleep_for(std::chrono::seconds(2));
	sock.send(zmq::message_t(sbuf), zmq::send_flags::none);
	stoken = true;
	t1.join();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	auto msg = zmq::message_t();

	auto received = sock.recv(msg, zmq::recv_flags::dontwait);
	if (received.has_value()) {
		std::cout << "Received " << msg.to_string() << "\n";
	}
	else {
		std::cout << "No message received\n";
	}
	std::cout << "Received " << msg.to_string() << "\n";
	received = sock.recv(msg, zmq::recv_flags::dontwait);

}