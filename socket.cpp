#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <vector>
#include<bits/stdc++.h>
#include "Server.h"
#include "httpserver.h"
#include "error.h"
#define _GLIBCXX_USE_CXX11_ABI 0/1



int main(void){
            std::cout<<"Your site works on port 8888\n";

            while(1){
                    HttpServer *HTTP= new HttpServer;
                    HTTP->send_HTTP();
                    usleep(300);
                    delete HTTP;
            }
}
