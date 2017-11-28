#include <iostream>
#include "enet.h"
#include <string>
#include <string.h>
int main()
{
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	ENetHost * client;
	client = enet_host_create(NULL /* create a client host */,
		1 /* only allow 1 outgoing connection */,
		2 /* allow up 2 channels to be used, 0 and 1 */,
		57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
		14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);
	if (client == NULL)
	{
		fprintf(stderr,
			"An error occurred while trying to create an ENet client host.\n");
		exit(EXIT_FAILURE);
	}

	ENetAddress address;
	ENetEvent event;
	ENetPeer *peer;
	enet_address_set_host(&address, "127.0.0.1");

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
				puts("Connection to some.server.net:1234 succeeded.");
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
				/*printf("A packet of length %u containing %s was received from %d on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->address.host,
					event.channelID);*/
				/* Clean up the packet now that we're done using it. */
				std::cout << event.packet->data << std::endl;


				enet_packet_destroy(event.packet);

				//puts("Connection to some.server.net:1234 succeeded.");
				/* Create a reliable packet of size 7 containing "packet\0" */
				ENetPacket * packet = enet_packet_create("packet",
					strlen("packet") + 1,
					ENET_PACKET_FLAG_RELIABLE);
				/* Extend the packet so and append the string "foo", so it now */
				/* contains "packetfoo\0"                                      */
				enet_packet_resize(packet, strlen("packetfoo000000000000") + 1);
				strcpy((char*)&packet->data[strlen("packet")], "foo");
				enet_peer_send(peer, 0, packet);
				enet_host_flush(client);
			}
		}
		else
		{
			enet_peer_reset(peer);
			puts("Connection to some.server.net:1234 failed.");
			//enet_host_connect(client, &address, 2, 0);
		}
	}
	enet_host_destroy(client);
}