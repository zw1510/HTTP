#include"cgi_sql.h"
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
int main()
{
	char poststr[50];
	char* lenstr;
	char name[20];
	int age[3];
	std::string school[25];
	memset(poststr,'\0',sizeof(poststr));
	memset(name,'\0',sizeof(name));
	memset(age,'\0',sizeof(age));
	memset(school,'\0',sizeof(school));
	std::cout<<"<html>"<<std::endl;
	std::cout<<"<head>"<<std::endl<<"post method</title >"<<std::endl<<"</head>"<<std::endl;
	std::cout<<"<body>"<<std::endl;
	std::cout<<"<div style=\"font-size:12px\">"<<std::endl;
	lenstr = getenv("CONTENT_LENGTH");
	int len=atoi(lenstr);
	fgets(poststr,len+1,stdin);
//	std::cout<<poststr<<std::endl;
	printf("%s\r\n",poststr);
	std::cout<<len<<std::endl;
//	int index=5;
//	int j=0;
//	while(poststr[index]!='&')
//	{
//		name[j]=poststr[index];
//		++j;
//		++index;
//	}
//	j=0;
//	++index;
//	while(poststr[index]!='&')
//	{
//		age[j]=poststr[index];
//		++j;
//		++index;
//	}
//	j=0;
//	++index;
//	while(poststr[index]!='\0')
//	{
//		school[j]=poststr[index];
//		++j;
//		++index;
//	}
	cgi_sql* _sql = new cgi_sql();
	_sql->my_connect();
	_sql->my_select();
	std::cout<<"</body>"<<std::endl;
	std::cout<<"</html>"<<std::endl;
	return 0;
}
