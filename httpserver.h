class HttpServer : protected Server
{
    std::string ok = "HTTP/1.1 200 OK\r\n";
    std::string content = "Content-Type: ";
    std::string content_ct = "Content-Length: ";
    std::string data = "\n\n";
    long int content_length = 0;
    char *requested_page = "index.html", *client_address, *http_version;
    std::string MIME_TYPE;

public:
    HttpServer()
    {
    }

protected:
    void read_file(const char *filename)
    {
        if (std::string(filename) == "/")
        {
            filename = "/index.html";
        }

        std::string openfile(std::string("www") + filename); //index file location set
        openfile.c_str();
        std::string line;
        std::ifstream html_page(openfile);

        if (!html_page.is_open())
        {
            html_page.close();
            //perror("\n\nCant open the index file\n");
            this->data = data.append("<html><body>Not found</body></html>"); //todo :not found page
            this->content_length = data.length() - 2;
        }

        else
        {
            while (!html_page.eof())
            {
                getline(html_page, line);
                this->content_length += line.length();
                data = data.append(line);
            }

            html_page.close();
        }
        //std::cout<<data;
    }

    void set_headers()
    {
        bool dot_is_found = false;
        MIME_TYPE = std::string(requested_page);
        size_t req_size = MIME_TYPE.find_last_of("/.");
        MIME_TYPE = MIME_TYPE.substr(req_size + 1);
        std::cout << "---MIME:" << MIME_TYPE << "------\n";

        if ((MIME_TYPE == "html") || MIME_TYPE == "")
        {
            MIME_TYPE = "text/html";
            this->content = "Content-Type: text/html; charset=UFT-8\r\n";
        }

        else if (MIME_TYPE == "css")
        {
            MIME_TYPE == "Content-Type: text/css\r\n";
            this->content = MIME_TYPE;
        }

        else if (MIME_TYPE == "ico")
        {
            MIME_TYPE = "Content-Type: image/x-icon\r\n";
            this->content = MIME_TYPE;
        }

        else if (MIME_TYPE == "svg")
        {
            MIME_TYPE = "Keep-Alive: timeout=5, max=98\r\nConnection: Keep-Alive\r\nContent-Type: image/svg+xml\r\n ";
            this->content = MIME_TYPE;
        }

        else if ((MIME_TYPE == "jpg") || (MIME_TYPE == "jpeg") || (MIME_TYPE == "jiff"))
        {
            MIME_TYPE = "Content-Type: image/jpeg\r\n\r";
            this->content = MIME_TYPE;
        }

        else if (MIME_TYPE == "png")
        {
            MIME_TYPE = "Content-Type: image/png\r\n\r";
            this->content = MIME_TYPE;
            this->content_length = 0;
        }

        else if (MIME_TYPE == "js")
        {
            MIME_TYPE = "Content-Type: text/javascript\r\n\r";
            this->content = MIME_TYPE;
        }

        else if (MIME_TYPE == "ttf")
        {
            MIME_TYPE = "Content-Type: font/ttf\r\n\r";
            this->content = MIME_TYPE;
        }

        else if (MIME_TYPE == "woff")
        {
            MIME_TYPE = "Content-Type: font/woff\r\n\r";
            this->content = MIME_TYPE;
        }

        else if (MIME_TYPE == "woff2")
        {
            MIME_TYPE = "Content-Type: font/woff2\r\n\r";
            this->content = MIME_TYPE;
        }

        else if (MIME_TYPE == "pdf")
        {
            MIME_TYPE = "Content-Type: application/pdf\r\n\r";
            this->content = MIME_TYPE;
        }

        else if (MIME_TYPE == ".tif" || MIME_TYPE == ".tiff")
        {
            MIME_TYPE = "Content-Type: image/tiff\r\n\r";
            this->content = MIME_TYPE;
        }

        else
        {
            MIME_TYPE = "text/plain\r\n";
            this->content = MIME_TYPE;
        }
    }

public:
    void send_HTTP()
    {
        this->decode_client_header();
        this->set_headers();
        //std::cout<<"ok";
        this->read_file(requested_page);
        std::string msg_string = this->ok + this->content + this->content_ct + std::to_string(this->content_length) + "" + this->data; //concatenate all http headers
        std::cout << msg_string << std::endl;

        this->send_str(msg_string);
        close(newsockfd);
        close(sockfd);
        //decode_client_header();
    }

    void decode_client_header()
    {
        int ct = 0;
        char *saveptr;
        char *parse_dat = this->buffer;
        std::vector<char *> stack_headers;
        char *http_headers = strtok_r(parse_dat, " ", &saveptr);

        while (http_headers != NULL)
        {
            //printf("%s\n",http_headers);
            http_headers = strtok_r(NULL, " ", &saveptr);
            stack_headers.push_back(http_headers);
            if (stack_headers.size() >= 3)
                break;
        }

        this->requested_page = stack_headers[0]; //reemove  first "/" from get request
        this->client_address = stack_headers[2];
        this->http_version = stack_headers[1];

        printf("%s\n %s\n %s\n ", requested_page, client_address, http_version);
        stack_headers.clear();
    }
};
