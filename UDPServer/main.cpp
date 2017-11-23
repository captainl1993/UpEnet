#include <iostream>
#include "enet.h"


int main()
{
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	ENetAddress address;
	ENetHost * server;
	address.host = ENET_HOST_ANY;
	enet_address_set_host(&address, "0.0.0.0");
	address.port = 8888;
	server = enet_host_create(&address ,32,2,0,0);
	if (server == NULL)
	{
		std::cout << "An error occurred while trying to create an ENet server host.\n" << std::endl;
		exit(EXIT_FAILURE);
	}

	ENetEvent event;
	while (true)
	{
		while (enet_host_service(server, &event, 10000) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("A new client connected from %x:%u.\n",
					event.peer->address.host,
					event.peer->address.port);
				/* Store any relevant client information here. */
				event.peer->data = (void*)"Client information";
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				printf("A packet of length %u containing %s was received from %s on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->data,
					event.channelID);
				/* Clean up the packet now that we're done using it. */
				std::cout << event.packet->data << std::endl;
				enet_packet_destroy(event.packet);
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s disconnected.\n", event.peer->data);
				/* Reset the peer's client information. */
				event.peer->data = NULL;
			}
		}
	}
	enet_host_destroy(server);
}