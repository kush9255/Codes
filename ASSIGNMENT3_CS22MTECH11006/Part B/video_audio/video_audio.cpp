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
  int TCP_PORT = 3010;
  int sockUdp, sockTCP, len = sizeof(server);
  socklen_t svr_size = sizeof(struct sockaddr_in);

  // data structure for sening and recieving.
  char recvBuff[MAXDATASIZE];
  string sendData = "2*3010*6543*127.0.0.3/";
  string token_code = "6543";
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
  cout << "TCP initiation with client " << endl;

  //############################### TCP initiation with client #################################
  sockTCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (sockTCP)
    cout << "TCP socket created successfully . . . \n";
  else
  {
    cout << "TCP socket can't be created" << endl;
    exit(1);
  }
  tcp_server.sin_family = AF_INET;
  tcp_server.sin_port = htons(TCP_PORT);
  inet_pton(AF_INET, "127.0.0.3", &(tcp_server.sin_addr)); ///////////////// CHECK
  memset(&(tcp_server.sin_zero), '\0', 8);
  int check = bind(sockTCP, (struct sockaddr *)&tcp_server, sizeof(struct sockaddr));

  if (check)
  {
    cout << "Error binding TCP socket with  \n"
         << endl;
    close(sockTCP);
    exit(1);
  }
  else
  {
    cout << "TCP Binding Successful \n\n";
  }

  //##################listening for client #########################
 int acpt_Client;
  while (1)
  {

    check = listen(sockTCP, 3); // start listining for new connections

    cout << "***************************************************************\n"
         << endl;
    if (check == -1)
    {
      cout << "Error listening on TCP socket" << endl;
      close(sockTCP);
      exit(1);
    }
    else
    {
      cout << "listening to client" << endl;
    }

     acpt_Client = accept(sockTCP, (struct sockaddr *)&clientAddr, &svr_size);
    if (!acpt_Client)
    {
      cout << "Error with new file server connection :" << endl;
      close(sockTCP);
      exit(1);
    }
    cout << "connected with client with ip : " << inet_ntoa(clientAddr.sin_addr) << "and port :" << ntohs(clientAddr.sin_port) << endl;
    // cout<<"client ip : "<<clientAddr.sin_addr.s_addr<<endl;

    memset(&recvBuff, 0, MAXDATASIZE);
    int r = recv(acpt_Client, &recvBuff, MAXDATASIZE, 0);

    //############  Token authentication begin   #########################

    if ((childpid = fork()) == 0)
    {
      close(sockTCP);
      string s = "";
      s += recvBuff;
      int pass = 0;
      if (s.compare(token_code) == 0)
      {
        cout << "Access token accepted";
        pass = 1; // token is passed
        sendData = "Ok";
      }
      else
      {
        sendData = "NOK";
      }
      send(acpt_Client, sendData.c_str(), sendData.length(), 0);
      if (pass) // our token
      {
        memset(&recvBuff, 0, MAXDATASIZE);
        read_file(acpt_Client); /// file is sent
      }
      else
      {
        cout << "!!!!  Invalid Tken !!!!  FILE DOESNOT EXIST    !!!!";
      }
      //############  Token authentication ends   #########################
    }
  }
  close(acpt_Client);
}
