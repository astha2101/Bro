#include <iostream>
#include <list>
#include <map>
#include <forward_list>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
using namespace std;

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef linux
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

// Amit [the programmer]
class FileSystemUtility
{
private:
    FileSystemUtility() {}

public:
    static bool fileExists(const char *path)
    {
        struct stat s;
        int x;
        x = stat(path, &s);
        if (x != 0)
            return false;
        if (s.st_mode & S_IFDIR)
            return false;
        return true;
    }

    static bool directoryExists(const char *path)
    {
        struct stat s;
        int x;
        x = stat(path, &s);
        if (x != 0)
            return false;
        if (s.st_mode & S_IFDIR)
            return true;
        return false;
    }
};
class StringUtility
{
private:
    StringUtility();

public:
    static void toLowerCase(char *str)
    {
        if (str == NULL)
            return;
        for (; *str; str++)
            if (*str >= 65 && *str <= 90)
                *str += 32;
    }
};
class HttpErrorStatusUtility
{
private:
    HttpErrorStatusUtility();

public:
    static void sendBadRequestError(int clientSocketDescriptor)
    {
        // will complete later on
    }
    static void sendHttpVersionNotSupportedError(int clientSocketDescriptor, char *httpVersion)
    {
        // will complete later on
    }

    static void sendNotFoundError(int clientSocketDescriptor, char *requestURI)
    {
        char content[1000];
        char header[200];
        char response[1200];
        sprintf(content, "<!doctype html><html lang='en'><head><meta charset='utf-8'><title>404 Error</title></head><body>Requested Resource[%s] Not Found</body></html>", requestURI);
        int contentLength = strlen(content);
        sprintf(header, "HTTP/1.1 404 Not Found\r\nContent-Type:text/html\nContent-Length:%d\nConnection: close\r\n\r\n", contentLength);
        strcpy(response, header);
        strcat(response, content);
        send(clientSocketDescriptor, response, strlen(response), 0);
    }

    static void sendMethodNotAllowedError(int clientSocketDescriptor, char *method, char *requestURI)
    {
        // will complete this later on
    }
};
class Validator
{
public:
    static bool isValidMIMEType(string &mimeType)
    {
        return true;
    }
    static bool isValidPath(string &path)
    {
        return FileSystemUtility::directoryExists(path.c_str());
    }
    static bool isValidURLFormat(string &url)
    {
        return true;
    }
};

class Error
{
    string error;

public:
    Error(string error)
    {
        this->error = error;
    }
    bool hasError()
    {
        return this->error.size() > 0;
    }
    string getError()
    {
        return this->error;
    }
};

class Request
{
private:
    char *method;
    char *requestURI;
    char *httpVersion;
    Request(char *method, char *requestURI, char *httpVersion)
    {
        this->method = method;
        this->requestURI = requestURI;
        this->httpVersion = httpVersion;
    }
    friend class Bro;
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
        this->contentLength = 0;
        this->contentIterator = this->content.before_begin();
    }
    ~Response()
    {
    }
    void setContentType(string contentType)
    {
        if (Validator::isValidMIMEType(contentType))
        {
            this->contentType = contentType;
        }
    }
    Response &operator<<(string content)
    {
        this->contentLength += content.length();
        this->contentIterator = this->content.insert_after(this->contentIterator, content);
        return *this;
    }
    friend class HttpResponseUtility;
};

class HttpResponseUtility
{
private:
    HttpResponseUtility(){};

public:
    static void sendResponse(int clientSocketDescriptor, Response &response)
    {
        char header[200];
        int contentLength = response.contentLength;
        sprintf(header, "HTTP/1.1 404 Not Found\r\nContent-Type:text/html\nContent-Length:%d\nConnection: close\r\n\r\n", contentLength);
        send(clientSocketDescriptor, header, strlen(header), 0);
        auto contentIterator = response.content.begin();
        while (contentIterator != response.content.end())
        {
            string str = *contentIterator;
            send(clientSocketDescriptor, str.c_str(), str.length(), 0);
            ++contentIterator;
        }
    }
};

enum __request_method__
{
    __GET__,
    __POST__,
    __PUT__,
    __TRACE__,
    __DELETE__,
    __OPTIONS__,
    __HEAD__
};
typedef struct __url_mapping
{
    __request_method__ requestMethod;
    void (*mappedFunction)(Request &request, Response &response);
} URLMapping;
class Bro
{
private:
    string staticResourcesFolder;
    map<string, URLMapping> urlMappings;

public:
    Bro()
    {
    }
    ~Bro()
    {
    }
    void setStaticResourcesFolder(string staticResourcesFolder)
    {
        if (Validator::isValidPath(staticResourcesFolder))
        {
            this->staticResourcesFolder = staticResourcesFolder;
        }
        else
        {
            string exception = ("Invalid static resources folder path : ") + staticResourcesFolder;
            throw exception;
        }
    }
    void get(string url, void (*callBack)(Request &, Response &))
    {
        if (Validator::isValidURLFormat(url))
        {
            urlMappings.insert(pair<string, URLMapping>(url, {__GET__, callBack}));
        }
    }
    void listen(int portNumber, void (*callback)(Error &))
    {
#ifdef _WIN32
        WSADATA wsaData;
        WORD ver;
        ver = MAKEWORD(1, 1);
        WSAStartup(ver, &wsaData);
#endif
        int serverSocketDescriptor;
        char requestBuffer[4097];
        int requestLength;
        int x;
        serverSocketDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocketDescriptor < 0)
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
        int successCode = bind(serverSocketDescriptor, (struct sockaddr *)(&serverSocketInformation), sizeof(serverSocketInformation));
        if (successCode < 0)
        {
            close(serverSocketDescriptor);
#ifdef _WIN32
            WSACleanup();
#endif
            char a[101];
            sprintf(a, "Unable to bind the socket to port : %d", portNumber);
            Error error(a);
            callback(error);
            return;
        }
        successCode = ::listen(serverSocketDescriptor, 10);
        if (successCode < 0)
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
        socklen_t len = sizeof(clientSocketInformation);
#endif
#ifdef _WIN32
        int len = sizeof(clientSocketInformation);
#endif

        int clientSocketDescriptor;
        while (1)
        {
            clientSocketDescriptor = accept(serverSocketDescriptor, (struct sockaddr *)(&clientSocketInformation), &len);
            requestLength = recv(clientSocketDescriptor, requestBuffer, sizeof(requestBuffer) - sizeof(char), 0);
            if (requestLength == 0 || requestLength == -1)
            {
                close(clientSocketDescriptor);
                continue;
            }
            int i;
            char *method, *requestURI, *httpVersion;
            requestBuffer[requestLength] = '\0';
            cout << "--------- Request Start--------------" << endl;
            cout << requestBuffer << endl;
            cout << "--------- Request End ---------------" << endl;
            // code to parse the first line http server starts here
            // first line should be REQUEST_METHOD SPACE URI SPACE HTTPVERSIONCRLF
            method = requestBuffer;
            i = 0;
            while (requestBuffer[i] && requestBuffer[i] != ' ')
                i++;
            if (requestBuffer[i] == '\0')
            {
                HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
                close(clientSocketDescriptor);
                continue;
            }
            requestBuffer[i] = '\0';
            i++;
            if (requestBuffer[i] == ' ' || requestBuffer[i] == '\0')
            {
                HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
                close(clientSocketDescriptor);
                continue;
            }
            StringUtility::toLowerCase(method);
            if (!(strcmp(method, "get") == 0 ||
                  strcmp(method, "put") == 0 ||
                  strcmp(method, "post") == 0 ||
                  strcmp(method, "trace") == 0 ||
                  strcmp(method, "delete") == 0 ||
                  strcmp(method, "options") == 0 ||
                  strcmp(method, "head") == 0))
            {
                HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
                close(clientSocketDescriptor);
                continue;
            }
            cout << "-------method---------" << method << endl;
            requestURI = requestBuffer + i;
            while (requestBuffer[i] && requestBuffer[i] != ' ')
                i++;
            if (requestBuffer[i] == '\0')
            {
                HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
                close(clientSocketDescriptor);
                continue;
            }
            requestBuffer[i] = '\0';
            cout << "-------requestURI--------" << requestURI << endl;
            i++;
            if (requestBuffer[i] == ' ' || requestBuffer[i] == '\0')
            {
                HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
                close(clientSocketDescriptor);
                continue;
            }
            httpVersion = requestBuffer + i;
            cout << "----------httpVersion----------" << httpVersion << endl;
            while (requestBuffer[i] && requestBuffer[i] != '\r' && requestBuffer[i] != '\n')
                i++;
            if (requestBuffer[i] == '\0')
            {
                HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
                close(clientSocketDescriptor);
                continue;
            }
            if (requestBuffer[i] == '\r' && requestBuffer[i + 1] != '\n')
            {
                HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
                close(clientSocketDescriptor);
                continue;
            }
            if (requestBuffer[i] == '\r')
            {
                requestBuffer[i] = '\0';
                i = i + 2;
            }
            else
            {
                requestBuffer[i] = '\0';
                i = i + 1;
            }

            StringUtility::toLowerCase(httpVersion);
            if (strcmp(httpVersion, "http/1.1") != 0)
            {
                HttpErrorStatusUtility::sendHttpVersionNotSupportedError(clientSocketDescriptor, httpVersion);
                close(clientSocketDescriptor);
                continue;
            }

            auto urlMappingIterator = urlMappings.find(requestURI);
            if (urlMappingIterator == urlMappings.end())
            {
                if(!serveStaticResource(clientSocketDescriptor,requestURI))
                {
                HttpErrorStatusUtility::sendNotFoundError(clientSocketDescriptor, requestURI);
                }
                close(clientSocketDescriptor);
                continue;
            }

            URLMapping urlMapping = urlMappingIterator->second;
            if (urlMapping.requestMethod == __GET__ && strcmp(method, "get") != 0)
            {
                HttpErrorStatusUtility::sendMethodNotAllowedError(clientSocketDescriptor, method, requestURI);
                close(clientSocketDescriptor);
                continue;
            }
            // code to parse the header and then the payload if exists starts here
            // code to parse the header and then the payload if exists ends here

            Request request(method, requestURI, httpVersion);
            Response response;
            urlMapping.mappedFunction(request, response);
            HttpResponseUtility::sendResponse(clientSocketDescriptor, response);
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
    try
    {
        Bro bro;
        bro.setStaticResourcesFolder("whatever");
        bro.get("/", [](Request &request, Response &response)
                {
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
    response<<html; });

        bro.get("/getCustomers", [](Request &request, Response &response)
                {
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
        response<<html; });

        bro.listen(6060, [](Error &error)
                   {
        if(error.hasError())
        {
            cout<<error.getError()<<endl;
            return;
        }
        else
        {
            cout<<"Bro HTTP web server is ready to accept the requests on port 6060"<<endl;
        } });
    }
    catch (string exception)
    {
        cout << exception << endl;
    }
    return 0;
}
