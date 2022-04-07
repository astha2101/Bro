#include<iostream>
#include<list>
#include<map>
#include<forward_list>
#include<string.h>
#include<unistd.h>
using namespace std;

#ifdef _WIN32
#include<windows.h>
#endif

#ifdef linux
#include<arpa/inet.h>
#include<sys/socket.h>
#endif

// Amit [the programmer]
class Validator
{
public:
static bool isValidMIMEType(string &mimeType)
{
    return false;
}
static bool isValidPath(string &path)
{
    return false;
}
static bool isValidURLFormat(string &url)
{
    return false;
}
};

class Error
{
    string error;
public:
Error(string error)
{
    this->error=error;
}
bool hasError()
{
    return this->error.size()>0;
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
private:
string contentType;
forward_list<string> content;
forward_list<string>::iterator contentIterator;
unsigned long contentLength;
public:
Response()
{
this->contentLength=0;
this->contentIterator=this->content.before_begin();
}
~Response()
{

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
map<string, void (*)(Request &request, Response &response)> urlMappings;
public:
Bro()
{

}
~Bro()
{

}
void setStaticResourcesFolder(string staticResourcesFolder)
{
    if(Validator::isValidPath(staticResourcesFolder))
    {
        this->staticResourcesFolder=staticResourcesFolder;
    }
    else
    {
        //not decided yet.
    }
}
void get(string url, void (*callBack)(Request &, Response &))
{
    if(Validator::isValidURLFormat(url))
    {
        urlMappings.insert(pair<string, void (*)(Request &request, Response &response)> (url,callBack));
    }
}
void listen(int portNumber, void (*callback)(Error &))
{
    #ifdef _WIN32
    WSADATA wsaData;
    WORD ver;
    ver=MAKEWORD(1,1);
    WSAStartup(ver,&wsaData);
    #endif
    int serverSocketDescriptor;
    char requestBuffer[4097];
    int requestLength;
    int x;
    serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(serverSocketDescriptor<0)
    {
        #ifdef _WIN32
        WSACleanup();
        #endif
        Error error("Unable to create the socket.");
        callback(error);
        return;
    }
    struct sockaddr_in serverSocketInformation;
    serverSocketInformation.sin_family = AF_INET;
    serverSocketInformation.sin_port = htons(portNumber);
    serverSocketInformation.sin_addr.s_addr = htonl(INADDR_ANY);
    int successCode = bind(serverSocketDescriptor,(struct sockaddr *)(&serverSocketInformation),sizeof(serverSocketInformation));
    if(successCode<0)
    {
        close(serverSocketDescriptor);
        #ifdef _WIN32
        WSACleanup();
        #endif
        char a[101];
        sprintf(a,"Unable to bind the socket to port : %d", portNumber);
        Error error(a);
        callback(error);
        return;
    }
    successCode = ::listen(serverSocketDescriptor,10);
    if(successCode<0)
    {
        close(serverSocketDescriptor);
        #ifdef _WIN32
        WSACleanup();
        #endif
        Error error("Unable to accept client connections");
        callback(error);
        return;

    }
    Error error("");
    callback(error);
    struct sockaddr_in clientSocketInformation;
    #ifdef linux
    socklen_t len= sizeof(clientSocketInformation);
    #endif
    #ifdef _WIN32
    int len = sizeof(clientSocketInformation);
    #endif

    int clientSocketDescriptor;
    while(1)
    {
        clientSocketDescriptor = accept(serverSocketDescriptor,(struct sockaddr *)(&clientSocketInformation),&len);
        if(clientSocketDescriptor<0)
        {
            // not yet decided
        }
        forward_list<string> requestBufferDS;
        forward_list<string>::iterator requestBufferDSIterator;
        int requestBufferDSSize =0;
        int requestDataCount=0;
        while(1)
        {
            requestLength = recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer)-sizeof(char),0);
            if(requestLength==0) break;
            requestBuffer[requestLength]='\0';
            requestBufferDSIterator = requestBufferDS.insert_after(requestBufferDSIterator,string(requestBuffer));
            requestBufferDSSize++;
            requestDataCount+=requestLength;
        }
        if(requestBufferDSSize>0)
        {
            char *requestData = new char[requestDataCount+1];
            char *p;
            p = requestData;
            const char *q;
            requestBufferDSIterator = requestBufferDS.begin();
            while(requestBufferDSIterator!=requestBufferDS.end())
            {
                q=(*requestBufferDSIterator).c_str();
                while(*q)
                {
                    *p=*q;
                    p++;
                    q++;
                }
                ++requestBufferDSIterator;
            }
            *p='\0';
            requestBufferDS.clear();
            printf("-------------request data begin ------------------\n");
            printf("%s\n",requestData);
            printf("-------------request data end --------------------\n");
            // the code to parse the request goes here
            delete [] requestData;

        }
        else
        {
            // do something if no data is received.
        }
        close(clientSocketDescriptor);
        // lots of code will come here later
    }   
    #ifdef _WIN32
    WSACleanup();
    #endif
}
};



// bobby [the user of the web server]
int main()
{
    Bro bro;
    bro.setStaticResourcesFolder("whatever");
    bro.get("/",[] (Request &request, Response &response)  {
    const char *html=R"""(
        <!DOCTYPE HTML>
        <html lang = 'en'>
        <head>
        <meta charset = 'utf-8'>
        <title>Whatever</title>
        </head>
        <body>
        <h1>Welcome</h1>
        <h3>Adminsistrator</h3>
        <a href='getCustomers'>Customer List</a>
        </body>
        </html>
    )""";
    response.setContentType("text/html");
    response<<html;
    });

    bro.get("/getCustomers",[] (Request &request, Response &response) {
      const char *html=R"""(
          <!DOCTYPE HTML>
            <html lang = 'en'>
            <head>
            <meta charset = 'utf-8'>
            <title>Whatever</title>
            </head>
            <body>
            <h1>List of customers</h1>
            <ul>
            <li>Ramesh</li>
            <li>Suresh</li>
            <li>Summit</li>
            </ul>
            </body>
            </html>
        )""";
        response.setContentType("text/html");
        response<<html;  
    });

    bro.listen(6060, [] (Error &error)  {
        if(error.hasError())
        {
            cout<<error.getError()<<endl;
            return;
        }
        else
        {
            cout<<"Bro HTTP web server is ready to accept the requests on port 6060"<<endl;
        }
    });
    return 0;
}
