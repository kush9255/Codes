#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<bits/stdc++.h>
#include <arpa/inet.h>
#include<sys/socket.h>
#include<stdbool.h>
#define SIZE 1024
using namespace std;

void recievefile(string file_name,int socket_id);

void send_file(int socket_id){
  int n;
  string filename;
  cout << "Enter the file path" << endl;
  cin >> filename;

    send(socket_id,filename.c_str(),filename.length(),0);
    string file_name ="";
   
	int i=0;
	string recieved_file="";
	while(filename[i]!='.')
	{
		recieved_file+=filename[i];
		i++;
	}
	while(i!=filename.length())
	{
		file_name+=filename[i];
		i++;
	}
	recieved_file+="_recieved"+file_name;
	cout << "Recieved  file name is : "<< recieved_file <<endl;
    recievefile(recieved_file,socket_id);
  }
  
void recievefile(string file_name,int socket_id)
{
  char buffer_length[40]; 						//placeholder for storing file size.
  FILE *fp =fopen(file_name.c_str(), "wb");
 int  p = recv(socket_id,buffer_length,sizeof(buffer_length),0);
  long total_buffer_size = atoi(buffer_length);   //converting file length (in String) to int
  cout << "Total size of file is :" << total_buffer_size <<endl;
  char * modified_buffer = new char[total_buffer_size];  // updated buffer to accomodate entire file
  int data_sent = 0;
  int data_left = total_buffer_size;
  int q;
  while( data_sent < total_buffer_size){   // keep recieving file until buffer overflows
   q = recv(socket_id,modified_buffer+data_sent,data_left,0);
    data_sent+=q;
    data_left-=q;
  }
  
  fwrite(modified_buffer,1,total_buffer_size,fp);
  fclose(fp);
  cout << "done";

  return;
}

int main(){

  int socket_id = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_id < 0) {
    cout<<"socket Error";
    exit(1);
  }
  cout<<" Socket successfully created \n";

 struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = 8080;
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

  int bnd = connect(socket_id, (struct sockaddr*)&server_address, sizeof(server_address));
  if(bnd == -1) {
    cout<<"Socket Error";
    exit(1);
  }
	cout<<"Connected to Server.\n";
  send_file(socket_id);
  cout<<"Data sent done.\n";

	cout<<"Closing the connection.\n";
  close(socket_id);

  return 0;
}
