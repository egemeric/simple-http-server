#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <vector>
#include "Server.h"
#include<bits/stdc++.h>
#define _GLIBCXX_USE_CXX11_ABI 0/1
void error(const char *msg){
 error(msg);

}


class HttpServer :protected Server{
    std::string ok = "HTTP/1.1 200 OK\n";
    std::string content= "Content-Type: ";
    std::string content_ct="\nContent-Length: ";
    std::string data="\n\n";
    long int content_length=0;
    char *requested_page="index.html", *client_address, *http_version;
    std::string MIME_TYPE;

public:
    HttpServer(){
    }

protected:
    void read_file(const char *filename){
        if (std::string(filename)=="/"){
            filename="/index.html";
        }
        
        std::string openfile(std::string("www")+filename); //index file location set
        openfile.c_str();
        std::string line;
        std::ifstream html_page(openfile);
        if(!html_page.is_open()){
            html_page.close();
            //perror("\n\nCant open the index file\n");
            this->data=data.append("<html><body>Not found</body></html>"); //todo :not found page
            this->content_length=data.length()-2;
        }
        
        else{
            while(!html_page.eof()){
                getline(html_page,line);
                this->content_length+=line.length();
                data=data.append(line);
            }
        
            html_page.close();
       }
      
      //std::cout<<data;
    
    }
    
    void set_headers(){
        bool dot_is_found=false;
        MIME_TYPE=std::string(requested_page);
        size_t req_size=MIME_TYPE.find_last_of("/.");
        MIME_TYPE=MIME_TYPE.substr(req_size+1);
        std::cout<<"---MIME:"<<MIME_TYPE<<"------\n";
        
        if((MIME_TYPE=="html")||MIME_TYPE==""){
            MIME_TYPE="text/html";
            this->content="Content-Type: text/html; charset=UFT-8";
        }
        
        else if(MIME_TYPE=="css"){
            MIME_TYPE=="Content-Type: text/css";
            this->content=MIME_TYPE;
        }
    
        else if(MIME_TYPE=="ico"){
            MIME_TYPE="Content-Type: image/x-icon\n";
            this->content=MIME_TYPE;
        }
    
        else if(MIME_TYPE=="svg"){
            MIME_TYPE="Content-Type: image/svg+xml\n";
            this->content=MIME_TYPE;
        }
        
        else if((MIME_TYPE=="jpg")||(MIME_TYPE=="jpeg")){
            MIME_TYPE="Content-Type: image/jpeg\n";
            this->content+=MIME_TYPE;
        }
        
        else if(MIME_TYPE=="png"){
            MIME_TYPE="Content-Type: image/png\n";
            this->content=MIME_TYPE;
        }
        
        else if(MIME_TYPE=="js"){
            MIME_TYPE="Content-Type: text/javascript\n";
            this->content=MIME_TYPE;
        }
        
        else if(MIME_TYPE=="ttf"){
            MIME_TYPE="Content-Type: font/ttf\n";
            this->content=MIME_TYPE;
        }
        
        else if(MIME_TYPE=="woff"){
            MIME_TYPE="Content-Type: font/woff\n";
            this->content=MIME_TYPE;
        }

        else if(MIME_TYPE=="woff2"){
            MIME_TYPE="Content-Type: font/woff2\n";
            this->content=MIME_TYPE;
        }

        else if(MIME_TYPE=="pdf"){
            MIME_TYPE="Content-Type: application/pdf\n";
            this->content=MIME_TYPE;
        }

        else{
            MIME_TYPE="text/plain\n";
            this->content+=MIME_TYPE;
        }
}

public:
    void send_HTTP(){
        this->decode_client_header();
        this->set_headers();
        //std::cout<<"ok";
        this->read_file(requested_page);
        std::string msg_string= this->ok + this->content + this->content_ct + std::to_string(this->content_length) + this->data; //concatenate all http headers
        int msg_len=msg_string.length();
        char send_msg_char[msg_len+1];
        strcpy(send_msg_char, msg_string.c_str());
        printf("---%s----",send_msg_char);
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
            std::cout<<"Your site works on port 8888\n";
            while(1){
                HttpServer *HTTP= new HttpServer;
                HTTP->send_HTTP();
            }
}