#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <socklib.hpp>
#include "server.h"

using namespace std;
using namespace socklib;

int main(int argc, char *argv[])
{
	if(argc != 3 && argc != 4){
		cerr << "usage: server <port> <local domain> [<relay host>:<port>]" << endl;
		return 0;
	}

	Server serv(argv[1], argv[2]);

	if(argc == 4)
	{
		bool ok = true;
		string server;
		string port;
		if((server = string(argv[3]).substr(0, string(argv[3]).find(":"))) == string(argv[3])){
			cout << "<server>:<port> expected !" << endl;
			ok = false;
		}
		else if(server == ""){
			cout << "<server>:<port> expected, server not specified !" << endl;
			ok = false;
		}
		else if((port = string(argv[3]).erase(0, string(argv[3]).find(":")+1)) == ""){
			cout << "<server>:<port> expected, port not specified !" << endl;
			ok = false;
		}

		if(ok){
			serv.relayHost(server, port);
			cout << "Relay Host mode enabled on "+server+":"+port << endl;
		}
		else
			cout << "server is starting in local mode only" << endl;
	}
	
	serv.start();
	return 0;
}
