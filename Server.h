#ifndef Server_h
#define Server_h
#include <cassert>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

class Server
{
protected:
    int sockfd, newsockfd, portno;
    int temp = 1;
    socklen_t clilen;
    char buffer[1024];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    //public:

    Server(int Portnum = 8888)
    {
        this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &temp, sizeof(temp)); //forece reuse the port
        if (sockfd < 0)
        {
            assert("Error open socket");
        }

        this->serv_addr.sin_family = AF_INET;
        this->serv_addr.sin_addr.s_addr = INADDR_ANY;
        this->serv_addr.sin_port = htons(Portnum);

        if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            assert("binding statede  problem var");
        }

        else
        {
            Listen_com();
            ACCEPT_CON();
        }
    }

    void Listen_com(int queue_tcp = 10)
    {
        listen(this->sockfd, queue_tcp);
        this->clilen = sizeof(cli_addr);
    }

    void ACCEPT_CON()
    {
        this->newsockfd = accept(this->sockfd, (struct sockaddr *)&cli_addr, &clilen);

        if (this->newsockfd < 0)
        {
            assert("Error on accept state");
        }

        else
        {
            printf("Connection from %s\n", inet_ntoa(cli_addr.sin_addr));
            this->n = read(newsockfd, this->buffer, 1024);
            //printf("\tBuffer:%s:\t",buffer);
            if (this->n < 0)
            {
                assert("ERROR reading from socket");
            }
        }
    }

    void send_data(const char *msg, int datalen)
    {
        const char *ptr = static_cast<const char *>(msg);
        while (datalen > 0)
        {
            int bytes = send(this->newsockfd, ptr, datalen, 0);
            if (bytes <= 0)
                break;
            ptr += bytes;
            datalen -= bytes;
        }
        // send(this->newsockfd,msg,strlen(msg),0);
    }

    void send_str(const std::string &http_string)
    {
        send_data(http_string.c_str(), http_string.size());
    }

public:
};
#endif

//TEST