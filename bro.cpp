#include<iostream>
using namespace std;

// Amit [the programmer]
class Error
{
public:
bool hasError()
{
    return false;
}
string getError()
{
    return "";
}
};

class Request
{

};

class Response
{
private:
string contentType;
public:
void setContentType(string contentType)
{
    if(Validator::isValidMIMEType(contentType))
    {
        this->contentType=contentType;
    }
}
Response & operator<<(string content)
{
    return *this;
}
};

class Bro
{
public:
void setStaticResourcesFolder(string staticResourcesFolder)
{
    // do nothing
}
void get(string url, void (*callBack)(Request &, Response &))
{
    // do nothing
}
void listen(int portNumber, void (*callback)(Error &))
{
    // do nothing
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