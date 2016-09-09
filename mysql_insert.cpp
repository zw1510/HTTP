#include"cgi_sql.h"
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
int main()
{
	char poststr[50];
	char* lenstr;
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
	int index=5;
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
	msg+="(\"";
	msg+=name;
	msg+="\",";
	msg+=age;
	msg+=",\"";
	msg+=school;
	msg+="\")";
	std::string type="(name,age,school)";
	_sql->my_insert(type,msg);
	std::cout<<"</body>"<<std::endl;
	std::cout<<"</html>"<<std::endl;
	return 0;
}
