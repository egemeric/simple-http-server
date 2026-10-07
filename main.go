package main

import (
	"flag"
	"fmt"
	"log"
	"net"
	"net/http"
	"os"
	"path"
	"path/filepath"
	"time"
)

type server struct {
	root string
}

func main() {
	port := flag.Int("port", 8888, "port to listen on")
	root := flag.String("root", "www", "folder to serve")
	flag.Parse()

	srv := &http.Server{
		Addr:              fmt.Sprintf(":%d", *port),
		Handler:           &server{root: *root},
		ReadHeaderTimeout: 5 * time.Second,
	}

	log.Printf("Serving %s on http://localhost:%d", *root, *port)
	log.Fatal(srv.ListenAndServe())
}

func (s *server) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	rec := &statusRecorder{ResponseWriter: w, status: http.StatusOK}
	s.serve(rec, r)

	ip, _, _ := net.SplitHostPort(r.RemoteAddr)
	log.Printf("%s \"%s %s %s\" %d", ip, r.Method, r.URL.RequestURI(), r.Proto, rec.status)
}

func (s *server) serve(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet && r.Method != http.MethodHead {
		w.Header().Set("Allow", "GET, HEAD")
		http.Error(w, "405 Method Not Allowed", http.StatusMethodNotAllowed)
		return
	}

	// path.Clean on "/..." can't go above "/", so the file always stays inside root
	name := filepath.Join(s.root, filepath.FromSlash(path.Clean("/"+r.URL.Path)))

	info, err := os.Stat(name)
	if err == nil && info.IsDir() {
		name = filepath.Join(name, "index.html")
		info, err = os.Stat(name)
	}
	if err != nil || info.IsDir() {
		s.notFound(w)
		return
	}

	f, err := os.Open(name)
	if err != nil {
		s.notFound(w)
		return
	}
	defer f.Close()

	// sets Content-Type and Content-Length, handles HEAD and Range requests
	http.ServeContent(w, r, info.Name(), info.ModTime(), f)
}

func (s *server) notFound(w http.ResponseWriter) {
	page, err := os.ReadFile(filepath.Join(s.root, "404.html"))
	if err != nil {
		page = []byte("<html><body><h1>404 Not Found</h1></body></html>")
	}
	w.Header().Set("Content-Type", "text/html; charset=utf-8")
	w.WriteHeader(http.StatusNotFound)
	w.Write(page)
}

// remembers the status code so it can be logged
type statusRecorder struct {
	http.ResponseWriter
	status int
}

func (r *statusRecorder) WriteHeader(code int) {
	r.status = code
	r.ResponseWriter.WriteHeader(code)
}
