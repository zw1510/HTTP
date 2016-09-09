#include"cgi_sql.h"
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
int main()
{
	char poststr[50];
	char* lenstr;
	std::string key;
	std::string name;
	std::string age;
	std::string school;
	memset(poststr,'\0',sizeof(poststr));
	std::cout<<"<html>"<<std::endl;
	std::cout<<"<head>"<<std::endl<<"post method</title >"<<std::endl<<"</head>"<<std::endl;
	std::cout<<"<body>"<<std::endl;
	std::cout<<"<div style=\"font-size:12px\">"<<std::endl;
	lenstr = getenv("CONTENT_LENGTH");
	int len=atoi(lenstr);
	fgets(poststr,len+1,stdin);
	std::cout<<poststr<<std::endl;
	std::cout<<len<<std::endl;
	int index=4;
	while(poststr[index]!='%')
	{
		key+=poststr[index++];
	}
	index+=3;
	key+="=\"";
	while(poststr[index]!='&')
	{
		key+=poststr[index++];
	}
	key+="\"";
	index+=6;
	while(poststr[index]!='&')
	{
		name+=poststr[index];
		++index;
	}
	index+=5;
	while(poststr[index]!='&')
	{
		age+=poststr[index];
		++index;
	}
	index+=8;
	while(poststr[index]!='\0')
	{
		school+=poststr[index];
		++index;
	}
	
	cgi_sql* _sql = new cgi_sql();
	_sql->my_connect();
	std::string msg;
	msg+="name=";
	msg+="\"";
	msg+=name;
	msg+="\",";
	msg+="age=";
	msg+=age;
	msg+=",school=\"";
	msg+=school;
	msg+="\"";
	std::cout<<msg<<std::endl;
	std::cout<<key<<std::endl;
	_sql->my_update(msg,key);
	std::cout<<"</body>"<<std::endl;
	std::cout<<"</html>"<<std::endl;
	return 0;
}
