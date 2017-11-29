#include <iostream>
#include "enet.h"
#include <string>
#include <string.h>
#include <chrono>
#include <thread>
#include <memory>
#include <vector>
#include "libgo/coroutine.h"
#include <stdio.h>
#include <boost/thread.hpp>
#if _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
using namespace std;
static int connectCon = 0;
int func()
{
	int index = 0;
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	ENetHost * client;
	client = enet_host_create(NULL, 1, 2, 57600 / 8, 14400 / 8);
	if (client == NULL)
	{
		fprintf(stderr,
			"An error occurred while trying to create an ENet client host.\n");
		exit(EXIT_FAILURE);
	}

	ENetAddress address;
	ENetEvent event;
	ENetPeer *peer;
	enet_address_set_host(&address, "192.168.210.169");

	address.port = 8888;
	peer = enet_host_connect(client, &address, 2, 0);
	if (peer == NULL)
	{
		fprintf(stderr,
			"No available peers for initiating an ENet connection.\n");
		exit(EXIT_FAILURE);
	}
	while (true)
	{
		if (enet_host_service(client, &event, 10) >= 0)
		{
			if (event.type == ENET_EVENT_TYPE_CONNECT)
			{
				std::cout <<"connected "<< connectCon << std::endl;
				connectCon++;

				//puts("Connection to some.server.net:1234 succeeded.");
				/* Create a reliable packet of size 7 containing "packet\0" */
				ENetPacket * packet = enet_packet_create("packet",
					strlen("packet") + 1,
					ENET_PACKET_FLAG_RELIABLE);
				/* Extend the packet so and append the string "foo", so it now */
				/* contains "packetfoo\0"                                      */
				enet_packet_resize(packet, strlen("packetfoo") + 1);
				strcpy((char*)&packet->data[strlen("packet")], "foo");
				enet_peer_send(peer, 0, packet);
				enet_host_flush(client);
			}
			else if (event.type == ENET_EVENT_TYPE_DISCONNECT)
			{
			}
			else if (event.type == ENET_EVENT_TYPE_RECEIVE)
			{
				//std::cout << event.packet->data << std::endl;
				//std::cout << index << std::endl;

				index++;
				/*if (index < 1000)*/
				{
					enet_packet_destroy(event.packet);

					/* Create a reliable packet of size 7 containing "packet\0" */
					ENetPacket * packet = enet_packet_create("packet",
						strlen("packet") + 1,
						ENET_PACKET_FLAG_RELIABLE);
					enet_packet_resize(packet, strlen("packetfoo") + 1);
					strcpy((char*)&packet->data[strlen("packet")], "foo");
					enet_peer_send(peer, 0, packet);
					enet_host_flush(client);
				}
			}
		}
		else
		{
			enet_peer_reset(peer);
			puts("Connection to some.server.net:1234 failed.");
		}
	}
	enet_host_destroy(client);
}

void foo()
{
	printf("function pointer\n");
}

struct A {
	void fA() { printf("std::bind\n"); }
	void fB() { printf("std::function\n"); }
};

int main(int argc, char **argv)
{
	go foo;
	for (int i = 0; i < 1200; i++)
	{
		go func;
	}


	go[]{
		printf("lambda\n");
	};

	go std::bind(&A::fA, A());

	std::function<void()> fn(std::bind(&A::fB, A()));
	go fn;

	// 也可以使用go_stack创建指定栈大小的协程
	//   创建拥有10MB大栈的协程
	go_stack(10 * 1024 * 1024) [] {
		printf("large stack\n");
	};

	// 协程创建以后不会立即执行，而是暂存至可执行列表中，等待调度器调度。
	// co_sched是全局唯一的协程调度器，有以下接口可以调度协程：
	//   1.Run 执行单次调度, 返回本次执行的协程数量
	//   2.RunLoop 无限循环执行Run, 不会返回
	//   3.RunUntilNoTask 循环执行Run, 直至协程数量为零.
	//
	// 当仅使用一个线程进行协程调度时, 协程地执行会严格地遵循其创建顺序.
	co_sched.RunUntilNoTask();

	// 多线程模式下, libgo还支持指定协程初始运行于哪个线程
	// 使用go_dispatch关键字来创建协程, 可以分派协程执行的线程.
	// 支持多种分派模式
	// 1.指定线程索引分派 (线程索引从0起, 按照调用Run的顺序决定线程索引)
	go_dispatch(2) [] {
		printf("dispatch to thread[2] run\n");
	};
}