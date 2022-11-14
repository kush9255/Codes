#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <fstream>
#include <bits/stdc++.h>
#include <tuple>

using namespace std;
const int backLog = 3;
const int maxDataSize = 1460;

void recievefile(string file_name, int socket_id);

void send_file(int socket_id,string s)
{
  int n;
  string filename="demo";
  string file_name="";
  cout << "The file path is" << endl;
  if(s.compare("Image")==0)
  filename+=".png";
  else if(s.compare("Audio/Video")==0)
  {
    cout<<"This server has 2 files audio and video"<<endl;
    cout<<"for audio file type mp3 else type mp4"<<endl;
    string format;
    cin>>format;
  filename+="."+format;
  }
  else if(s.compare("PDF")== 0)
  filename+=".pdf";
  else if(s.compare("TEXT")==0)
  filename+=".txt";
  cout << filename<<" is downloading "<<endl;

  int i=0;
  send(socket_id, filename.c_str(), filename.length(), 0);
  string recieved_file = "";
  while (filename[i] != '.')
  {
    recieved_file += filename[i];
    i++;
  }
  while (i != filename.length())
  {
    file_name += filename[i];
    i++;
  }
  recieved_file += "_recieved" + file_name;
  cout << "Recieved  file name is : " << recieved_file << endl;
  recievefile(recieved_file, socket_id);
}

void recievefile(string file_name, int socket_id)
{
  char buffer_length[40]; // placeholder for storing file size.
  FILE *fp = fopen(file_name.c_str(), "wb");
  int p = recv(socket_id, buffer_length, sizeof(buffer_length), 0);
  if(sizeof(buffer_length)== 0)
  {
    cout<<"Wrong file name entered";
    exit(2);
  }
  long total_buffer_size = atoi(buffer_length); // converting file length (in String) to int
  char *modified_buffer = new char[total_buffer_size]; // updated buffer to accomodate entire file
  int data_sent = 0;
  int data_left = total_buffer_size;
  int q;
  while (data_sent < total_buffer_size)
  { // keep recieving file until buffer overflows
    q = recv(socket_id, modified_buffer + data_sent, data_left, 0);
    data_sent += q;
    data_left -= q;
  }

  fwrite(modified_buffer, 1, total_buffer_size, fp);
  fclose(fp);
  cout << "File transferred successfuly"<<endl;

  return;
}
int main()
{
  int check;
  uint16_t udpPort = 3008;
  uint16_t tcpPort = 3009; // server's tcp port
  const char *localhost = "127.0.0.1";
  socklen_t sinSize = sizeof(struct sockaddr_in);
  int flags = 0;
  int dataRecvd = 0, dataSent = 0;
  struct sockaddr_in server, tcp_server;
  char recvBuff[maxDataSize];
  char sendBuf[maxDataSize];
  string sendStr;
  string rcvStr;

  int sockUdp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sockUdp)
    cout << "UDP socket created successfully . . . \n";
  else
  {
    cout << "UDP socket can't be created" << endl;
    exit(1);
  }
  server.sin_addr.s_addr = inet_addr(localhost);
  server.sin_port = htons(udpPort);
  server.sin_family = AF_INET;

  cout << "***************************************************************\n\n"
       << endl;
  cout << "pinging boot strap for meta data\n\n"
       << endl;

  //#################### pinging boot strap for meta data  ##############################

  string sendData = "Hello from client\n";

  int snd = sendto(sockUdp, sendData.c_str(), maxDataSize, 0, (struct sockaddr *)&server, sinSize);
  if (snd > 0)
    cout << "boot strap server is pinged\n\n"
         << endl;

  cout << "***************************************************************\n\n"
       << endl;

  //#################### pinging boot strap for meta data ends ##############################

  //####################  recieving from bootstrap  ###################################

  cout << "Waiting for meta data to be recieved\n\n"
       << endl;

  int r = recvfrom(sockUdp, recvBuff, sizeof(recvBuff), 0, (struct sockaddr *)&server, &sinSize);
  string data_rcvd = "";
  data_rcvd += recvBuff;
  int l = data_rcvd.length();

  cout << "meta data recieved from client" << endl;

  cout << "***************************************************************\n"
       << endl;

  cout << data_rcvd << endl;

  //######################### recieving from bootstrap end ##################

  // **************** parsing meta data  begins********************
  int i = 0;
  int c = 4;
  while (c != 0)
  {
    string port = "";
    string type = "", code = "", ip = "";
    for (; data_rcvd[i] != '*'; i++)
      type += data_rcvd[i];
    i++;
    for (; data_rcvd[i] != '*'; i++)
      port += data_rcvd[i];
    i++;
    for (; data_rcvd[i] != '*'; i++)
      code += data_rcvd[i];
    i++;
    for (; data_rcvd[i] != '/'; i++)
      ip += data_rcvd[i];
    i++;
    stringstream str(port);
    str >> tcpPort; // tcp port is modified here

    // **************** parsing meta data  ends********************

    //##################### enabling tcp conection with server begins  #######################

    if (type.compare("1") == 0)
      type = "Image";
    if (type.compare("2") == 0)
      type = "Audio/Video";
    if (type.compare("3") == 0)
      type = "PDF";
    if (type.compare("4") == 0)
      type = "TEXT";

    cout << "***************************************************************\n"
         << endl;
    cout << "***************************************************************\n"
         << endl;
    cout << "file server is " << type << " server\n"
         << endl;
    cout << type << " server port is " << port << endl;
    cout << type << " server code is " << code << endl;
    cout << type << " server ip is " << ip << endl;
    cout << "***************************************************************\n"
         << endl;
    cout << "***************************************************************\n"
         << endl;

    cout << "enabling tcp conection with " << type << "server" << endl;

    int sockTCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // tcp connection
    if (sockTCP)
      cout << "TCP socket created successfully . . . \n";
    else
    {
      cout << "TCP socket can't be created" << endl;
      exit(1);
    }
    memset(&tcp_server, 0, sizeof(struct sockaddr_in));
    memset(&recvBuff, 0, maxDataSize);
    inet_pton(AF_INET, ip.c_str(), &(tcp_server.sin_addr));
    tcp_server.sin_family = AF_INET;
    tcp_server.sin_port = htons(tcpPort);

    int chck = connect(sockTCP, (struct sockaddr *)&tcp_server, sizeof(struct sockaddr));

    if (chck != -1)
      cout << "Established TCP with " << type << " server" << endl;
    else
    {
      exit(1);
    }
    cout << "***************************************************************\n"
         << endl;

    //##################### enabling tcp conection with server ends  #######################
    memset(&recvBuff, 0, maxDataSize);
    cout << "the access token for " << type << " server is :" << code << endl
         << endl;
    cout << "enter the token access for " << type << " server\n"
         << endl;
    cin >> sendStr;
    dataSent = send(sockTCP, sendStr.c_str(), sendStr.length(), 0);

    int r = recv(sockTCP, &recvBuff, maxDataSize, 0);
    rcvStr = "";
    rcvStr += recvBuff;
    if (rcvStr.compare("Ok") == 0)
    {
      memset(&recvBuff, 0, maxDataSize);
      send_file(sockTCP,type);
    }
    else{
      cout << "!!!!  Invalid Token !!!!  FILE DOESNOT EXIST    !!!!";
    }
    cout << "###############################################################"<< endl;
    cout << "###############################################################"<< endl;
    c--;
  }
}
