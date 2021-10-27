#include<iostream>
#include<map>
#include<forward_list>
#include<string.h>
#include<windows.h>
#include<unistd.h>
#ifdef _WIN32
#include<windows.h>
#endif
#ifdef linux
#include<arpa/inet.h>
#include<sys/socket.h>
#endif


using namespace std;
// this is to test working of git
// Amit's part (made the web server)


class Validator
{
private:
Validator()
{
}
public:
static bool isValidMIMEType(string &mimeType)
{
// now do nothing
return true;
}
static bool isValidPath(string &path)
{
// now do nothing
return true;
}
static bool isValidURLFormat(string &url)
{
// now do nothing
return true;
}
};

class Error
{
private:
string error;
public:
Error(string error)
{
this->error=error;
}
bool hasError()
{
return this->error.length()>0;
}
string getError()
{
return this->error;
}
};

class Request
{

};

class Response
{
string contentType;
forward_list<string> content;
forward_list<string>::iterator contentIterator;
unsigned int contentLength;
public:
Response()
{
this->contentLength=0;
this->contentIterator=this->content.before_begin();
}
~Response()
{
// not yet decided
}
void setContentType(string contentType)
{
if(Validator::isValidMIMEType(contentType))
{
this->contentType=contentType;
}
}
Response & operator<<(string content)
{
this->contentLength+=content.length();
this->contentIterator=this->content.insert_after(this->contentIterator,content);
return *this;
}
};

class Bro
{
private:
string staticResourcesFolder;
map<string,void (*)(Request &,Response &)> urlMappings;
public:
Bro()
{
// not yet decided
}
~Bro()
{
// not yet decided
}
void setStaticResourcesFolder(string staticResourcesFolder)
{
if(Validator::isValidPath(staticResourcesFolder))
{
this->staticResourcesFolder=staticResourcesFolder;
}
else
{
//not yet decided.
}
}
void get(string url,void (*callBack)(Request &request,Response &response))
{
if(Validator::isValidURLFormat(url))
{
urlMappings.insert(pair<string, void (*)(Request &, Response &)>(url,callBack));
}
}
void listen(int portNumber,void (*callBack)(Error &error))
{
#ifdef _WIN32
WSADATA wsaData;
WORD ver;
ver=MAKEWORD(1,1);
WSAStartup(ver,&wsaData);
#endif
int serverSocketDescriptor;
char requestBuffer[4096];
int requestLength;
int x;
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
if(serverSocketDescriptor<0)
{
#ifdef _WIN32
WSACleanup();
#endif
Error error("Unable to create a socket\n");
callBack(error);
return;
}
struct sockaddr_in serverSocketInformation;
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(portNumber);
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
int successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
if(successCode<0)
{
close(serverSocketDescriptor);
#ifdef _WIN32
WSACleanup();
#endif
char a[101];
sprintf(a,"Unable to bind to port : %d",portNumber);
Error error(a);
callBack(error);
return;
}
successCode=::listen(serverSocketDescriptor,10);
if(successCode<0)
{
close(serverSocketDescriptor);
#ifdef _WIN32
WSACleanup();
#endif
Error error("Unable to accept client connections.");
callBack(error);
return;
}
Error error("");
callBack(error);
struct sockaddr_in clientSocketInformation;
#ifdef linux
socklen_t len=sizeof(clientSocketInformation);
#endif
#ifdef _WIN32
int len=sizeof(clientSocketInformation);
#endif
int clientSocketDescriptor;
while(1)
{
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
if(clientSocketDescriptor<0)
{
// not yet decided will do this later
}
requestLength=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer),0);
if(requestLength>0)
{
for(x=0;x<requestLength;x++) printf("%c",requestBuffer[x]);
const char *response=
"HTTP/1.1 200 OK\r\n"
"Connection: close\r\n"
"Content-Type: text/html\r\n"
"Content-Length: 144\r\n\r\n"
"<html><head><title>Thinking Machines</title></head>"
"<body><h1>Thinking Machines</h1>"
"<h3>We Teach More Than We Promise To Teach</h3></body></html>";
send(clientSocketDescriptor,response,strlen(response),0);


}
// a lot of code will be written later

#ifdef _WIN32
WSACleanup();
#endif
}
}
};




// Bobby's part (web application developer, user of bro)
int main()
{
    Bro bro;
    bro.setStaticResourcesFolder("whatever");
    bro.get("/",[](Request &request, Response &response) {
    const char *html=R"""(
    <!DOCTYPE HTML>
    <html lang='en'>
    <head>
    <meta charset='utf-8'>
    <title>Whatever</title>
    </head>
    <body>
    <h1>Welcome</h1>
    <h3>Administrator</h3>
    <a href='getCustomers'>Customers List</a>
    </body>
    </html>)""";
    response.setContentType("text/html");
    response<<html;
    });

    bro.get("/getCustomers",[](Request &request,Response &response) {
    const char *html=R"""(
    <!DOCTYPE HTML>
    <html lang='en'>
    <head>
    <meta charset='utf-8'>
    <title>Whatever</title>
    </head>
    <body>
    <h3>List of customers</h3>
    <ui>
    <li>Mahesh</li>
    <li>Suresh</li>
    <li>Ramesh</li>
    </ui>
    <a href='/'>Home</a>
    </body>
    </html>)""";  
    response.setContentType("text/html");
    response<<html;
    });

    bro.listen(6060,[](Error &error) {
        if(error.hasError())
        {
            cout<<error.getError()<<endl;
            return;
        }
        cout<<"Bro http web server is ready to accept request at port 6060"<<endl;
    });
    return 0;
}