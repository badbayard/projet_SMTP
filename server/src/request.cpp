#include "request.h"

Request::Request(){
	this->mode = NORMAL;
	this->content = "";
}

Request::~Request(){}

void Request::pushContent(string content){
	this->content += content;
}

void Request::setDomain(string domain){
	this->domain = domain;
}

void Request::setFrom(string from){
	this->from = from;
}

void Request::setTo(string to){
	this->dests.push_back(to);
}

void Request::setLocalTo(string to){
	this->localDests.push_back(to);
}

string Request::getDomain(){
	return this->domain;
}

string Request::getFrom(){
	return from;
}

vector<string> Request::getDests(){
	return this->dests;
}

vector<string> Request::getLocalDests(){
	return this->localDests;
}

string Request::getContent(){
	return content;
}