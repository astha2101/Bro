#include<iostream>
using namespace std;

// Amit's part (made the web server)
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
public:
void setContentType(string contentType)
{

}
Response & operator<<(string content)
{
    return *this;
}
};

class Bro
{
public:
void setStaticResourcesFolder(string staticResourceFolder)
{

}
void get(string urlPattern,void (*callBack)(Request &request,Response &response))
{

}
void listen(int portNumber,void (*callBack)(Error &error))
{

}
};




// Bobby's part (web application developer, user of bro)
int main()
{
    Bro bro;
    bro.setStaticResourcesFolder("whatever");
    bro.get("/",[](Request &request, Response &response) void{
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

    bro.get("/getCustomers",[](Request &request,Response &response) void{
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
    response.setContentType("text/html")
    response<<html;
    });

    bro.listen(6060,[](Error &error) void{
        if(error.hasError())
        {
            cout<<error.getError()<<endl;
            return 0;
        }
        cout<<"Bro http web server is ready to accept request at port 6060"<<endl;
    });
    return 0;
}
