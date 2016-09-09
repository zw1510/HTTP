#include"cgi_sql.h"

void cgi_sql::my_connect()
{
	_mysql=mysql_init(NULL);
	if(!mysql_real_connect(_mysql, _host.c_str(),_user.c_str(),_passwd.c_str(), _db.c_str(),_port,NULL,0))
	{
		std::cout<<"connect error\n"<<std::endl;
	}
}

void cgi_sql::my_insert(std::string &type,std::string &msg)
{
	std::string _sql = "INSERT INTO ";
	_sql += _table_name;
	_sql += " ";
	_sql += type;
	_sql += " ";
	_sql +="values ";
	_sql += msg;
	
	int ret = mysql_query(_mysql,_sql.c_str()); // success 0,other none zero
}
void cgi_sql::my_delete(std::string &key)
{
	std::string _sql = "delete from ";
	_sql += _table_name;
	_sql += " where ";
	_sql += key;
	mysql_query(_mysql,_sql.c_str());
}
void cgi_sql::my_select(std::string options)
{
	std::string _sql = "select * from ";
	_sql += _table_name;
//	_sql += optins;
	int ret=mysql_query(_mysql,_sql.c_str());
	if(ret == 0)
	{
		MYSQL_RES* res = mysql_store_result(_mysql);
		if(res)
		{
			int res_lines = mysql_num_rows(res);
			int res_cols = mysql_num_fields(res);
			std::cout<<"lines"<<res_lines<<"cols"<<res_cols<<std::endl;
			MYSQL_FIELD *fn = NULL;
			for(; fn = mysql_fetch_field(res);)
			{
				std::cout<<fn->name<<"\t";
			}
			std::cout<<std::endl;
			
			int i=0;
			for(;i < res_lines; ++i)
			{
				int j = 0;
				MYSQL_ROW row = mysql_fetch_row(res);
				for(;j < res_cols ;++j)
				{
					std::cout<<row[j]<<"\t"	;	
				}
				std::cout<<std::endl;
			}
		}
		std::cout<<"select done...sucess"<<std::endl;
	}
	else
	{
		std::cout<<"select done ,falied"<<std::endl;
	}
}
void cgi_sql::my_update(std::string &msg,std::string &key)
{
	std::string sql="update ";
	sql+=_table_name;
	sql+=" set ";
	sql+=msg;
	sql+=" where ";
	sql+=key;
	mysql_query(_mysql,sql.c_str());
}
cgi_sql::~cgi_sql()
{
	if(_mysql)
	{
		mysql_close(_mysql);
		_mysql = NULL;
	}
}

//int main()
//{
//	cgi_sql* _sql = new cgi_sql();
//	_sql->my_connect();
//	std::string msg="name=\"laowang\",age=29,school=\"xgy\"";
//	std::string key="name=\"xiaowang\"";
//	_sql->my_update(msg,key);
//	_sql->my_select();
//	std::string key = "name=\"zhangsan\"";
//	_sql->my_delete(key);
//	std::string type = "(name,age,school)";
//	std::string msg="('zhangwei',10,'SUST')";
//
//	_sql->my_insert(type,msg);
//	_sql->my_select();	
//	return 0;
//}
