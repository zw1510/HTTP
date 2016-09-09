#include"cgi_sql.h"
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
int main()
{
	char poststr[50];
	char* lenstr;
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
	
	cgi_sql* _sql = new cgi_sql();
	_sql->my_connect();
	std::string key;
	int index=4;
	while(poststr[index]!='%')
	{
		key+=poststr[index++];
	}
	key+="=\"";
	index+=3;
	while(poststr[index]!='\0')
	{
		key+=poststr[index++];
	}
	key+="\"";
	_sql->my_delete(key);
	std::cout<<"</body>"<<std::endl;
	std::cout<<"</html>"<<std::endl;
	return 0;
}
