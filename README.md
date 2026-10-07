# simple-http-server
HTTP Server with Go

Serves static files from a folder. It started as a C++ project; the C++ version is in the git history.

## Run

Needs Go 1.22+. Works on Windows, macOS and Linux.

```sh
go run .                            # serves ./www on port 8888
go run . -port 8080 -root ./public  # other port and folder
go build                            # builds ./simple-http-server
```

Open http://localhost:8888 and stop with Ctrl+C.

## Features

- GET and HEAD (other methods get 405)
- Content-Type based on file extension, works for images and fonts too
- Custom 404 page (`www/404.html`)
- Files outside the web root can't be read (`..` is cleaned out of the path)
- Prints each request: `ip "GET / HTTP/1.1" 200`
