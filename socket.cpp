#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fstream>
#include <vector>
//#include <sstream>
//#include <netinet/in.h>
//#include <stdlib.h>
//#include <sys/types.h>
//#include <stdlib.h>
void error(const char *msg){
 error(msg);

}
class Server {
protected:
    int sockfd, newsockfd, portno;
    int temp=1;
    socklen_t clilen;
    char buffer[1024];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
//public:
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
            this->n=read(newsockfd,this->buffer,1024);
            //printf("\tBuffer:%s:\t",buffer);
         if (this->n < 0)
            {error("ERROR reading from socket");}
        }
    }

    void send_msg(const char *msg){

        send(this->newsockfd,msg,strlen(msg),0);




    }
    public:


};


class HttpServer :protected Server{
    std::string ok = "HTTP/1.1 200 OK\n";
    std::string content= "Content-Type: text/html; charset=UFT-8\n";
    std::string content_ct="Content-Length: ";
    std::string data="\n\n";
    long int content_length=0;
    char *requested_page="index.html", *client_address, *http_version;

public:
    HttpServer(){

    }
protected:
    void read_file(const char *filename){

        if (std::string(filename)=="/")
        {
            filename="/index.html";
        }
      std::string openfile(std::string("www")+filename); //index file location set
      openfile.c_str();

      std::string line;
      std::ifstream html_page(openfile);
      if(!html_page.is_open()){
        html_page.close();
        //perror("\n\nCant open the index file\n");
        this->data="<html><body>Not found</body></html>";
        this->content_length=data.length();
      }else{
          while(!html_page.eof()){
                getline(html_page,line);
                this->content_length+=line.length();
                data=data.append(line);
          }
          html_page.close();
       }
      //std::cout<<data;
    }

public:
    void send_HTTP(){
        this->decode_client_header();
        //std::cout<<"ok";
        this->read_file(requested_page);
        std::string msg_string= this->ok + this->content + this->content_ct + std::to_string(this->content_length) + this->data; //concatenate all http headers
         int msg_len=msg_string.length();
         char send_msg_char[msg_len+1];
         strcpy(send_msg_char, msg_string.c_str());

        //printf("%s",send_msg_char);
        this->send_msg(send_msg_char);
        close(newsockfd);
        close(sockfd);
        //decode_client_header();

    }

    void decode_client_header(){
        int ct=0;
        char *parse_dat =this->buffer;
        std::vector<char*> stack_headers;
        char *http_headers=strtok(parse_dat," ");

        while (http_headers!=NULL){
            //printf("%s\n",http_headers);
            http_headers=strtok(NULL," ");
            stack_headers.push_back(http_headers);
            if (stack_headers.size()>=3)
                break;
        }
        this->requested_page=stack_headers[0]; //reemove  first "/" from get request

        this->client_address=stack_headers[2];
        this->http_version=stack_headers[1];
        printf("%s\n %s\n %s\n ",requested_page,client_address,http_version);
        stack_headers.clear();
    }

};

int main(void){

            while(1){
                HttpServer *HTTP= new HttpServer;

                HTTP->send_HTTP();
            }


}

