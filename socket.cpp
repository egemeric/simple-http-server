#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
//#include <streambuf>
//#include <sstream>
void error(const char *msg){
 error(msg);
 exit(1);
}
class Server {
protected:
    int sockfd, newsockfd, portno;
    int temp=1;
    socklen_t clilen;
    char buffer[1024];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
public:
    Server(int Portnum=8888)
    {
       this->sockfd=socket(AF_INET,SOCK_STREAM,0);
       setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &temp, sizeof(temp)); //forece reuse the port
       if (sockfd<0){
            error("Error open socket");
       }
        this->serv_addr.sin_family=AF_INET;
        this->serv_addr.sin_addr.s_addr = INADDR_ANY;
        this->serv_addr.sin_port=htons(Portnum);
        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
              error("binding statede  problem var");
        }
        else{
            Listen_com();
            ACCEPT_CON();
        }

    }

    void Listen_com(int queue_tcp=5){
        listen(this->sockfd,queue_tcp);
        this->clilen=sizeof(cli_addr);
    }

    void ACCEPT_CON(){
        this->newsockfd=accept(this->sockfd,(struct sockaddr *)&cli_addr, &clilen);
        if(this->newsockfd<0){
            error("Error on accept state");
        }else{
            printf("Connection from %s\n",inet_ntoa(cli_addr.sin_addr));
        }
    }

    void send_msg(const char *msg){

        send(this->newsockfd,msg,strlen(msg),0);

        this->n=read(newsockfd,this->buffer,1024);
         if (this->n < 0)
            {error("ERROR reading from socket");}
       printf("\n------cli headers------\n %s\n",this->buffer);
    }





};


class HttpServer :protected Server{
    std::string ok = "HTTP/1.1 200 OK\n";
    std::string content= "Content-Type: text/html; charset=UFT-8\n";
    std::string content_ct="Content-Length: ";
    std::string data="\n\n";
    long int content_length=0;

public:
    HttpServer(){

    }

    void read_file(const char *filename){
      std::string line;
      std::ifstream html_page(filename);
      while(!html_page.eof()){
        getline(html_page,line);
        this->content_length+=line.length();
        data=data.append(line);

      }

      //std::cout<<data;
    }


    void send_HTTP(){
        this->read_file("index.html");
        std::string msg_string= this->ok + this->content + this->content_ct + std::to_string(this->content_length) + this->data; //concatenate all http headers
         int msg_len=msg_string.length();
         char send_msg_char[msg_len+1];
         strcpy(send_msg_char, msg_string.c_str());

        printf("%s",send_msg_char);
        this->send_msg(send_msg_char);
        close(newsockfd);
        close(sockfd);

    }

};

int main(void){

            while(1){
                HttpServer *HTTP= new HttpServer;

                HTTP->send_HTTP();
            }


}

