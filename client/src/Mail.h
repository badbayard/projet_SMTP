#ifndef MAIL_H
#define MAIL_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <socklib.hpp>
#include <bufferedreaderwriter.hpp>
#include <sstream>
#include <regex>
#include <ctime>
#include <b64/encode.h>
#include <magic.h>
#include <libgen.h>

#define MIME_DB "/usr/share/file/magic.mgc"

using namespace std;

enum{
	MAIN, FORM
};

class Mail{
public:
	Mail(string arg_server);

	void setFrom(string from);
	void setTo(string to);
	void setSubject(string subj);
	void setContent(string content);
	void setAttachment(string attachment);
	string getFrom();
	string getTo();
	string getSubject();
	string getContent();

	void send();
	bool ready(const int forwhat = MAIN);
	ifstream file;

private:
	string server, port, from, subject, content, attachment;
	vector<string> dests;

	void command(string message, socklib::BufferedReaderWriter &sock);
	string encode(string attachment);
	string header(string attachment);
	string currentDate();
	string parseAddress(string address);
};

#endif //MAIL_H