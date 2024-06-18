#ifndef NETWORK_H
#define NETWORK_H

#define MAX_CLIENTS 4
#define SERVER_PORT 1234
#define MAX_PACKET_SIZE 1024

void sendMessage(const char *data);
void receiveMessage();

#endif /* NETWORK_H */
