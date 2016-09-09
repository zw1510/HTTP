#include <stdio.h>
#include <stdlib.h>
#include"bigdata.h"
#include<string>
using namespace std;
int main(void)
{

		char* data;
		//string a;
		//string b;
		char a[100],b[100];
		printf("<html>\n");
		printf("<head>\n<title >get method</title>\n</head>\n");
		printf("<body>\n");
		printf("<div style=\"font-size:12px\">\n");
		data = getenv("QUERY_STRING");
		printf("data:%s\n",data);
		if(sscanf(data,"a=%[^&]&b=%s",a,b) != 2)
		{																										     
				printf("<DIV STYLE=\"COLOR:RED\">Error parameters should be entered!</DIV>\n");									
		}
		else
		{
						//printf("<DIV STYLE=\"COLOR:GREEN; font-size:15px;font-weight:bold\">a + b = %s</DIV>\n",BigData(a)+BigData(b));
						cout<<a<<"+"<<b<<"="<<BigData(a)+BigData(b)<<endl;
		}
	    printf("<HR COLOR=\"blue\" align=\"left\" width=\"500\">");
	    printf("<input type=\"button\" value=\"Back CGI\" onclick=\"javascript:window.location='./bigdata.html'\">");
	    printf("\n");
	    printf("</body>\n");
	    printf("</html>\n");								
	    return 0;							
}
