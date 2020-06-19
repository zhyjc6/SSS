/**
 * "httpd.h"
 *
 * @author zhyjc6
 * @date 2020.6.16
 */

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdint.h>

#define ISspace(x) isspace((int)(x))

#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n"
#define STDIN   0
#define STDOUT  1
#define STDERR  2

namespace httpserver {

class HttpServer{
private:
    int server_sock = -1;
    struct sockaddr_in server_name;
    u_short port = 4000;
    int client_sock = -1;
    struct sockaddr_in client_name;
    socklen_t  client_name_len = sizeof(client_name);
    pthread_t newthread;
public:
    void startup(u_short port);
    void run();
    void* accept_request();
private:
    static void* help(void* arg);
    void bad_request(int fd);
    void cat(int, FILE *);
    void cannot_execute(int);
    void execute_cgi(int, const char *, const char *, const char *);
    int get_line(int, char *, int);
    void headers(int, const char *);
    void not_found(int);
    void serve_file(int, const char *);
    void unimplemented(int);

};//class HttpServer

}//namespace httpserver


