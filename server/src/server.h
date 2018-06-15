#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <socklib.hpp>
#include <bufferedreaderwriter.hpp>
#include <regex>
#include <fstream>
#include <vector>
#include <unistd.h>
#include "request.h"

using namespace std;
using namespace socklib;

const string PATH = ".";

class Server
{
public:
	Server(string port, string domain);
	~Server();
	
	void relayHost(string server, string port); //activate relay host service
	void start();

private:
	int id_client;
	string relayServer, relayPort;
	string port, localDomain;
	vector<string> localUsers;
	int socketServer;
	void reply(string message, int socket);
	void save(Request * req); //store or transfer e-mail from client
	void relayCommand(string message, BufferedReaderWriter &sock); //send message to relay host
	void discuss(int con);
	bool userExists(string mail);
};

#endif //SERVER_H