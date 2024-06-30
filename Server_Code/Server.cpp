#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <ctime>
#include <fstream>
struct  TimeSampleADC
{
	tm *tm_local;
 	char buffer[1024];


}MyTimeSampleADC;

using namespace std;
int main()
{
     ofstream myfile;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    myfile.open("Gas_Reading.txt",ios::trunc);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 5);

    int clientSocket = accept(serverSocket, nullptr, nullptr);
    while(1)
    {
	time_t curr_time=time(0);
	MyTimeSampleADC.tm_local=localtime(&curr_time);
        char buffer[4] = {0};

        recv(clientSocket, MyTimeSampleADC.buffer, sizeof(MyTimeSampleADC.buffer), 0);
	myfile.seekp(0,ios::beg);
	myfile<<MyTimeSampleADC.tm_local->tm_hour<<":"<<MyTimeSampleADC.tm_local->tm_min<<"Message from client: " << MyTimeSampleADC.buffer << endl;
        //cout<<MyTimeSampleADC.tm_local->tm_hour<<":"<<MyTimeSampleADC.tm_local->tm_min<<"Message from client:"<<MyTimeSampleADC.buffer<<endl; 
    }
    close(serverSocket);
    myfile.close();
    return 0;
}
