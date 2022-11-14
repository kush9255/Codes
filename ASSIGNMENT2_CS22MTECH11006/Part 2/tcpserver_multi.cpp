#define MAX_LEN 1024
#include <stdio.h>
#include<bits/stdc++.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<exception>
#include<limits>
#include<typeinfo>
#include<unistd.h>
#include<pthread.h>
using namespace std;
void read_file(int socket_id);
void write_file(int socket_id);



void read_file(int socket_id)
{
  char file_name[MAX_LEN];
  int p = recv(socket_id,file_name,MAX_LEN,0);
 
 FILE *fp = fopen(file_name, "rb");
 FILE *fp1 = fopen(file_name, "rb");
  fseek(fp1, 0, SEEK_END);   //iterating file from start to end
  long filelen = ftell(fp1);   			// total size of file
 string buff_length = to_string(filelen);  //length converted to string for send function
 const char *file_size= buff_length.c_str();
  send(socket_id,file_size,buff_length.length(),0);
  char *buffer = new char[filelen]; 
 while(fread(buffer, filelen, 1, fp) != NULL); //keep reading file till end
  fclose(fp);
  //cout << buffer;
  int bytesent = 0;
  int bytesleft = filelen;
  int n1 = send(socket_id,buffer,bytesleft,0);

}
void * hand(void* p){
	int socket_id =(*(int *)p);
	read_file(socket_id);
	close(socket_id);
	
}

int main(){
  const char *ip = "127.0.0.1";
  int port = 8080;
  int e;

  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[MAX_LEN];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("Socket Error");
    exit(1);
  }
  cout<<"Server socket creation is succesfull.\n";

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e < 0) {
    perror("Bind error occured");
    exit(1);
  }
  cout <<" Binding  is successfull.\n" ;

  if(listen(sockfd, 10) == 0){
		cout<<"Listening to socket.....\n";
	}else{
		cout<<"Unable to listen !!!!";
    exit(1);
	}

  addr_size = sizeof(new_addr);
  while(1)
  {
  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
  pthread_t =t;
  int * pclient =NULL;
  pclient = malloc(sizeof(int));
  pthread_create(&t,NULL,hand,pclient);
  
  }
  cout <<"Data written in the file successfully.\n";

  return 0;
}

