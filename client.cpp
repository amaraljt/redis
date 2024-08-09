// client.cpp : Client Connection

#include <iostream>
#include <sys/socket.h>

int main() {
  int fd = socket(AF_INIT, SOCK_STREAM, 0); // client socket
  if(fd < 0){
    die("socket()");
  }

  // Define client connection
  struct sockaddr_in addr = {};
  addr.sin_family = AF_INIT;
  addr.sin_port = ntohs(1234);
  addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);
  int rv = connect(fd, (const struct sockaddr *) &addr, sizeof(addr));
  if(rv){
    die("connect()");
  }

  char msg[] = "hello";
  write(fd, msg, strlen(msg));

  char rbuf[64] = {};
  ssize_t n = read(fd, rbuf, sizeof(rbuf) - 1);
  if(n < 0){
    die("read()");
  }
  printf("server says: %s\n", rbuf);
  close(fd);

  return 0;
}
