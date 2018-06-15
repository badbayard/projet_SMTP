#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

//available input modes
enum{
	DATA,
	NORMAL
};

class Request{
public:
	Request();
	~Request();

	int mode = NORMAL;

	void pushContent(string content);
	void setDomain(string domain);
	void setFrom(string from);
	void setTo(string to);
	void setLocalTo(string to);
	string getFrom();
	vector<string> getDests();
	vector<string> getLocalDests();
	string getContent();
	string getDomain();

private:
	string domain, from, content;
	vector<string> dests;
	vector<string> localDests;
};

#endif //REQUEST_H