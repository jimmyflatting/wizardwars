#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <iostream>
#include <SDL2/SDL_net.h>

class Network
{
public:
  Network();
  ~Network();

  bool client_init();
  bool server_init();

  bool client_connect(const char *ip);
  bool server_accept();

  void send(const char *message);
  void receive(char *message);

  void close();

private:
  IPaddress ip;
  TCPsocket socket;
  SDLNet_SocketSet socketSet;
};

#endif // NETWORK_HPP