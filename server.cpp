// server.cpp : Create a TCP Server

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

static void do_something(int connfd) {
  char rbuf[64] = {};
  ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
  if(n < 0) {
    msg("read() error");
    return;
  }

  printf("client says: %s\n", rbuf);

  char wbuf[] = "world";
  write(connfd, wbuf, strlen(wbuf));
}

int main(){

  int fd = socket(AF_INET, SOCK_STREAM, 0);

  // 2. Configure Socket
  int val = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

  // 3. Define the server address
  struct sockaddr_in addr = {}; // holds an IPv4 address and port
  addr.sin_family = AF_INET; // IPv4
  addr.sin_port = ntohs(1234); // Port 1234 in big endian
  addr.sin_addr.s_addr = ntohl(0); // internet address 0.0.0.0
  int rv = bind(fd, (const sockaddr *) &addr, sizeof(addr));
  if(rv) {
    die("bind()");
  }  

  // 4. Listen
  rv = listen(fd, SOMAXCONN); // (fd, max # of connections)
  if(rv) {
    die("listen()");
  }

  // 5. Accept connections
  while(true) {
    // accept 
    struct sockaddr_in client_addr = {};
    socklen_t addrlen = sizeof(client_addr);
    int connfd = accept(fd, (struct sockaddr *) &client_addr, &addrlen);
    if(connfd < 0) {
      continue; // error
    }

    do_something(connfd);
    close(connfd);
  }

  return 0;
}
