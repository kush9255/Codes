#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;
const int backLog = 3;
const int maxDataSize = 1460;

int main()
{
  int UDP_PORT = 3008;
  int TCP_PORT = 3009;

  sockaddr_in server, clntserver;

  string serverIpAddr = "127.0.0.1";
  int sockFd, len = sizeof(clntserver);
  socklen_t svr_size = sizeof(struct sockaddr_in);

  // data structure for sening and recieving.
  char recvBuff[maxDataSize];
  char recvBUff1[maxDataSize];
  string sendData = "hello from boot strap";

  // socket creation
  sockFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sockFd)
    cout << "UDP socket created successfully . . . \n";
  else
  {
    cout << "UDP socket can't be created" << endl;
    exit(1);
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(UDP_PORT);
  inet_pton(AF_INET, serverIpAddr.c_str(), &(server.sin_addr));
  memset((char *)&server.sin_zero, '\0', 8);

  // bind
  int bnd_udp = bind(sockFd, (sockaddr *)&server, sizeof(struct sockaddr));
  if (bind)
    cout << "Bind Successful" << endl;
  else
  {
    cout << "Error in binding" << endl;
    exit(1);
  }
  cout << "***************************************************************" << endl;
  int c = 0;
  int n = 4;
  string data_rcvd = "";
  while (c != n)
  {
    memset(&clntserver, 0, sizeof(struct sockaddr_in));
    memset(&recvBuff, 0, maxDataSize);

    int p = recvfrom(sockFd, recvBuff, maxDataSize, 0, (struct sockaddr *)&clntserver, &svr_size);
    if (p > 0)
      data_rcvd += recvBuff;
    c++;
    cout << c << " server is registered.Waiting for " << n - c << " servers to register" << endl;
  }
  cout << "All "
       << " servers have been registered" << endl;

  cout << "***************************************************************" << endl;

  cout << "META data recieved \n\n"
       << data_rcvd << "\n\n"
       << endl;

  string dataTosend = data_rcvd;

  cout << "***************************************************************" << endl;
  while (1)
  {
    cout << "waiting for client initiation" << endl;

    //############################ waiting for client initiation ########################################################

    memset(&recvBUff1, 0, maxDataSize);
    int p = 0;
    p = recvfrom(sockFd, recvBUff1, maxDataSize, 0, (struct sockaddr *)&clntserver, &svr_size);
    cout << recvBUff1;
    data_rcvd += recvBUff1;

    if (p > 0)
      cout << "Client is acknowledged" << endl;

    cout << "***************************************************************" << endl;
    //##################### sending meta data to client #######################################
    cout << "Sending data to Client\n\n"
         << endl;
    int snd = 0;
    snd = sendto(sockFd, dataTosend.c_str(), maxDataSize, 0, (struct sockaddr *)&clntserver, svr_size);

    if (snd > 0)
      cout << "Meta data is sent to the client\n\n"
           << endl;
    else
      cout << "error";
    cout << "***************************************************************\n"
         << endl;
  }
}