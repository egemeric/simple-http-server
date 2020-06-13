#ifndef Server_h
#define Server_h
#define _GLIBCXX_USE_CXX11_ABI 0/1
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






#endif
