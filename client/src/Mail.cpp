#include "Mail.h"

Mail::Mail(string arg_server){
	string server;
	string port;
	if((server = arg_server.substr(0, arg_server.find(":"))) == arg_server){
		cout << "<server>:<port> expected !" << endl;
		exit(0);
	}
	else if(server == ""){
		cout << "<server>:<port> expected, server not specified !" << endl;
		exit(0);
	}
	else if((port = arg_server.erase(0, arg_server.find(":")+1)) == ""){
		cout << "<server>:<port> expected, port not specified !" << endl;
		exit(0);
	}

	this->server = server;
	this->port = port;
	this->attachment = "";
}

bool Mail::ready(const int forwhat){
	cout << forwhat << endl;
	if(this->from != "" && this->dests.size() > 0 && ((this->subject != "" && this->content != "") || forwhat == FORM))
		return true;
	else
		return false;
}

void Mail::send(){
	int s = socklib::CreeSocketClient(this->server, this->port);
	socklib::BufferedReaderWriter sock(s);

	command("HELO localhost", sock);
	command("MAIL FROM:<"+this->from+">", sock);
	for(unsigned int i=0 ; i<dests.size() ; i++)
		command("RCPT TO:<"+this->dests[i]+">", sock);
  	command("DATA", sock);
  	string data = "Date: "+currentDate()+"\n";
  	data += "From: "+parseAddress(this->from)+"\n";
  	data += "Subject: "+this->subject+"\n";
  	data += "To: "+parseAddress(this->dests[0])+"\n";
  	
  	data += "Content-Type: multipart/mixed;\n";
  	data += "	boundary=\"------------09D7C080709C636310FA900E\"\n";

  	data += "--------------09D7C080709C636310FA900E\n";
  	data += "Content-Type: text/plain; charset=\"utf-8\"\n";
  	data += "Content-Transfer-Encoding: 8bit\n\n";
  	data += this->content+"\n\n";

  	if(this->attachment != ""){
  		data += header(this->attachment);
  		data += encode(this->attachment);
  	}

	data += "--------------09D7C080709C636310FA900E--";

  	command(data, sock);
	command(".", sock);
  	command("QUIT", sock);
}

string Mail::header(string attachment)
{
	//file type detection
	const char *mimetype;
	const char *file = attachment.c_str();
	magic_t magic_cookie;
	magic_cookie=magic_open(MAGIC_MIME_TYPE);
	if(magic_cookie != NULL){
		magic_load(magic_cookie, MIME_DB);
		mimetype = magic_file(magic_cookie,file);
	}
	else
		mimetype = "application/octet-stream";

	//writing header
	string filename = basename(strdup(attachment.c_str()));
	string data;
	data = "--------------09D7C080709C636310FA900E\n";
  	data += "Content-Type: "+string(mimetype)+"; ";
  	if(string(mimetype) == "plain/text")
  		data += "charset=\"UTF-8\"; ";
  	data += "name=\""+filename+"\"\n";
  	data += "Content-Transfer-Encoding: base64\n";
  	data += "Content-Disposition: attachment; filename=\""+filename+"\"\n\n";
	magic_close(magic_cookie);
  	return data;
}

string Mail::encode(string attachment){
	base64::encoder e;
	stringstream buffer;
	file.open(attachment, ios::out | ios::binary);
	if(file.is_open()){
		e.encode(file, buffer);
		//cout << buffer.str() << endl;
		return buffer.str()+"\n";
	}
	else
		cout << "enable to open the file" << endl;
	file.close();

	return "error";
}

string Mail::currentDate(){
	time_t t = time(0);
    struct tm * now = localtime(&t);
    char buffer[80];
    strftime(buffer,sizeof(buffer),"%a, %d %b %Y %X %z",now);
    return string (buffer);
}

string Mail::parseAddress(string address){
	smatch m;
	regex r("(.*)@(.*)");
	regex_match(address, m, r);
	return string (m[1])+"<"+address+">";
}

void Mail::command(string message, socklib::BufferedReaderWriter &sock){
	string rep;
	message += "\r\n";

	cout << "\033[32m(⌐■_■)✎__ \033[0m\n\033[92m>>> " << message << "\033[0m";
	sock.write(message);
	rep = sock.read_line();
	cout <<"\033[94;1m<<< \033[0m\033[94m" << rep << "\033[0m\n";
}

void Mail::setFrom(string from){
	this->from = from;
}

void Mail::setTo(string to){
	string tmp = "";

	this->dests.clear();

	if(to.length() > 5){
		for(unsigned int i=0 ; i<to.length() ; i++){
			if(to[i] == ','){
				if(tmp.length() > 5)
					this->dests.push_back(tmp);
				tmp = "";
			}
			else
				tmp += to[i];
		}
		this->dests.push_back(tmp);
	}
}

void Mail::setSubject(string subj){
	this->subject = subj;
}

void Mail::setContent(string content){
	this->content = content;
}

void Mail::setAttachment(string attachment){
	this->attachment = attachment;
}

string Mail::getFrom(){
	return from;
}

string Mail::getTo(){
	string to = "";
	for(unsigned int i=0 ; i<dests.size() ; i++){
		if(i != 0)
			to += ",";
		to += dests[i];
	}
	cout << "to : " << to << endl;
	return to;
}

string Mail::getSubject(){
	return subject;
}

string Mail::getContent(){
	return content;
}