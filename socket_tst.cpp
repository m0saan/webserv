#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(){

int server_fd, new_socket;
if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
{
	perror("cannot create socket !");
	return -1;
}
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &new_socket, sizeof(new_socket));
struct sockaddr_in address;
const int PORT = 8000;

memset((char *)&address, 0, sizeof(address));
address.sin_family = AF_INET;
address.sin_addr.s_addr = htonl(INADDR_ANY);
address.sin_port = htons(PORT);
int addrlen = sizeof(address);

memset(address.sin_zero, '\0', sizeof address.sin_zero);

if (bind(server_fd, (struct sockaddr *)&address,addrlen) < 0)
{
	perror("Bind failed!");
	return -1;
}

int listening ;
if ((listening = listen(server_fd, 3)) < 0)
{
	perror("Cannot Listen!");
	return -1;
}

while(1)
{
std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
{
	perror("cannot accept!");
	return -1;
}

char buffer[30000] = {0};
	int valread = read(new_socket, buffer, 30000);
	std::cout << buffer << std::endl;
	if (valread < 0)
		std::cout << "No bytes are there to read" << std::endl;
	
	std::string hello = "Hello from the server";
	write(new_socket, hello.c_str(), hello.length());
	std::cout << "Message Sent Succesffully !\n";
	close(new_socket);
}

}
