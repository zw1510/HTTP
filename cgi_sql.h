#pragma once

#include<iostream>
#include<string>
#include<stdlib.h>
#include<mysql.h>

class cgi_sql
{
public:
	void my_connect();
	void my_insert(std::string &type,std::string &msg);
	void my_select(std::string options="");
	void my_delete(std::string &key);
	void my_update(std::string &msg,std::string &key);
public:	
	cgi_sql(const std::string host="127.0.0.1",const std::string user="root",const std::string passwd="",const int port=3306,const std::string db="test",std::string table_name="http"):
	_host(host),
	_user(user),
	_passwd(passwd),
	_port(port),
	_db(db),
	_table_name(table_name)
	{}
	~cgi_sql();
	
private:
	MYSQL* _mysql;
	std::string _table_name;
	std::string _host;
	std::string _user;
	std::string _db;
	std::string _passwd;
	int _port;
};
