#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>

using namespace std;

#define MAXDATASIZE 2048

void read_file(int socket_id)
{
  char file_name[MAXDATASIZE];
  int p = recv(socket_id, file_name, MAXDATASIZE, 0);

  FILE *fp = fopen(file_name, "rb");
  FILE *fp1 = fopen(file_name, "rb");
  if(fp == NULL)
  {
    cout<<"Wrong file name is entered"<<endl;
    exit(2);
  }
  fseek(fp1, 0, SEEK_END);
  long filelen = ftell(fp1);
  string buff_length = to_string(filelen);
  const char *file_size = buff_length.c_str();
  send(socket_id, file_size, buff_length.length(), 0);
  char *buffer = new char[filelen];
  while (fread(buffer, filelen, 1, fp) != NULL);
  fclose(fp);
  cout << buffer;
  int bytesent = 0;
  int bytesleft = filelen;
  int n1 = send(socket_id, buffer, bytesleft, 0);
}

int main()
{
  sockaddr_in server, tcp_server, clientAddr;

  const char *localhost = "127.0.0.1";
  int UDP_PORT = 3008;
  int TCP_PORT = 3011;
  int sockUdp, sockTCP, len = sizeof(server);
  socklen_t svr_size = sizeof(struct sockaddr_in);

  // data structure for sening and recieving.
  char recvBuff[MAXDATASIZE];
  string sendData = "3*3011*1211*127.0.0.4/";
  string token_code = "1211";
  // socket creation
  sockUdp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
pid_t childpid;
  if (sockUdp)
    cout << "UDP socket created successfully . . . \n";
  else
  {
    cout << "UDP socket can't be created" << endl;
    exit(1);
  }

  server.sin_addr.s_addr = inet_addr(localhost);
  server.sin_port = htons(UDP_PORT);
  server.sin_family = AF_INET;

  cout << "***************************************************************\n"
       << endl;
  //#################meta data sent to boot strap #####################
  int snd = sendto(sockUdp, sendData.c_str(), MAXDATASIZE, 0, (struct sockaddr *)&server, svr_size);
  if (snd > 0)
    cout << "Meta data sent to boot strap" << endl;

  else
  {
    cout << "Error while sending";
    exit(1);
  }

  close(sockUdp);
  cout << "closing UDP connection with Bootstrap\n"
       << endl;
  cout << "***************************************************************\n"
       << endl;
 
}
