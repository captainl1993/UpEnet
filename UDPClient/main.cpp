#include <iostream>
#include "enet.h"
#include <string>
#include <string.h>
#include <chrono>
#include <thread>
#include <memory>
#include <vector>
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
		if (enet_host_service(client, &event, 1000) >= 0)
		{
			if (event.type == ENET_EVENT_TYPE_CONNECT)
			{
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
				std::cout << event.packet->data << std::endl;
				std::cout << index << std::endl;

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

std::vector<std::shared_ptr<std::thread>> threads;
int main()
{
	/*for (int i = 0; i < 20; i++)
	{
		threads.push_back(std::shared_ptr<std::thread>(new std::thread(func)));
	}*/
	func();
}