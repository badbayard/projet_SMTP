#include "server.h"

Server::Server(string port, string domain){
	this->port = port;
	this->localDomain = domain;
	this->socketServer = CreeSocketServeur(port);

	//get local users list
	string line;
	ifstream f(PATH+"/users.list",ios::in);
	if(f)
		while(getline(f,line))
			localUsers.push_back(line);
	else{
		cerr << "users.list file not found ! aborted" <<endl;
		exit(0);
	}

}

Server::~Server(){}

void Server::relayHost(string server, string port)
{
	this->relayServer = server;
	this->relayPort = port;
}

void Server::reply(string message, int socket)
{
	message = message+"\r\n";
	send(socket,message.c_str(),message.size(),0);
	cout << "["+to_string(this->id_client)+"] \033[94mᕕ(ᐛ)ᕗ\033[0m\033[94;1m >>> \033[0m\033[94m" << message << "\033[0m" << endl;
}

void Server::relayCommand(string message, BufferedReaderWriter &sock){
	string rep;
	message += "\r\n";

	cout << "["+to_string(this->id_client)+"] \033[96;1m(つ▀¯▀)⊃━☆ﾟ.*･｡ﾟ\033[0m\n\033[96m>>> " << message << "\033[0m";
	sock.write(message);
	rep = sock.read_line();
	cout <<"["+to_string(this->id_client)+"] \033[93;1m<<< \033[0m\033[93m" << rep << "\033[0m\n";
}

void Server::save(Request * req)
{
	//for each local recipient
	for(unsigned int i=0 ; i<req->getLocalDests().size() ; i++){
		ofstream file(PATH+"/"+req->getLocalDests()[i],fstream::out | fstream::app);

		if(file){
			file << "@@@@@@@@@@@@@@@@@@@ "+req->getFrom()+" @@@@@@@@@@@@@@@@@@@\n\n";
			file << req->getContent();
			file << "\n\n\n";
		}
		else
			cerr << "Problem to save local mail" << endl;
	}

	//transfer
	if(req->getDests().size() > 0 && !this->relayServer.empty()){
		cout << "Relay Host enabled" << endl;
		cout << "Start connection to "+this->relayServer+":"+this->relayPort << endl;
		int s = socklib::CreeSocketClient(this->relayServer, this->relayPort);
		socklib::BufferedReaderWriter sock(s);
		cout << "Transfering e-mail..." << endl;
		bool begin = true;
		for(unsigned int i=0 ; i<req->getDests().size() ; i++){
			if(begin){
				relayCommand("HELO localhost", sock);
				relayCommand("MAIL FROM:"+req->getFrom(), sock);
			}
			relayCommand("RCPT TO:<"+req->getDests()[i]+">", sock);
			if(begin){
			  	relayCommand("DATA", sock);
			  	relayCommand(req->getContent(), sock);
				relayCommand(".", sock);
				begin = false;
			}
		}
	}
}

bool Server::userExists(string mail){
	mail = mail.substr(0, mail.find("@"));
	for(unsigned int i=0; i<localUsers.size() ; i++)
		if(mail == localUsers[i])
			return true;
	return false;
}

void Server::discuss(int con)
{
	string command;
	std::smatch m;
	Request * req = new Request();
	BufferedReaderWriter in(con);
	while(1){
		command = in.read_line();
		cout << "["+to_string(this->id_client)+"] \033[32m<<< \033[0m\033[92m " << command << "\033[0m";

		//the server is waiting for regular commands 
		if(req->mode == NORMAL)
		{
			if(regex_search(command,m,std::regex("^HELO (.*)")))
			{
				req->setDomain(m[1]);
				reply("220 "+this->localDomain,con);
			}
			else if(regex_search(command, m, std::regex("^MAIL FROM:(.*)"))){
				req->setFrom(m[1]);
				reply("250 OK",con);
			}
			else if(regex_search(command,m,std::regex("^RCPT TO: *<((.*)@(.*))>"))){
				//local
				if(m[3] == this->localDomain){
					if(userExists(m[2])){
						req->setLocalTo(m[2]); //save username only
						reply("250 OK",con);
					}
					else
						reply("550 local user not found",con);
				}
				//disabled transfer
				else if(relayServer.empty()){
					reply("551 non local user, relay host disabled", con);
				}
				//enabled transfer
				else{
					req->setTo(string(m[1])); //save complete address
					reply("251 non local user, transfer to "+this->relayServer+":"+this->relayPort, con);
				}
			}
			else if(regex_search(command,std::regex("^DATA *"))){
				req->mode = DATA;
				reply("250 OK",con);
			}
			else if (command == "QUIT\r\n"){
				reply("221 Bye :)",con);
				break;
			}
			else
				reply("500 unknown command", con);
		}
		//the server is only waiting for random dataset from the client
		else if(req->mode == DATA){
			//toggle DATA mode if a line is received with only '.' char
			if(command == ".\r\n"){
				reply("250 OK: queued as ?",con);
				save(req);
				req->mode = NORMAL;
			}
			else
			{
				req->pushContent(command);

				if(command.size() >= 2 
				&& command.at(command.size()-2) == '\r' 
				&& command.at(command.size()-1) == '\n')
					reply("354 end mail width .", con);
			}
			/*send (con,domaine.data(),domaine.size(),0);
			send (con,"yo",3,0);*/
		}
	}
}

void Server::start()
{
	pid_t child;
	this->id_client = 0;
	while(1){
		int con = socklib::AcceptConnexion(this->socketServer);

		child = fork();

		if(child == 0){
			discuss(con);
		}
		else
			this->id_client++;
	}
}
