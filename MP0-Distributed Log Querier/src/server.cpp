//
// Created by Kitsch on 2020/9/4.
//

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <cstring>
#include <thread>
#include <stdexcept>
#include <string>
#include "../include/server.h"

#define MAX_BUFFER_SIZE 1024

//Accept query request from client
//Execute the query request
//send back the result;

void make_sockaddr(sockaddr_in *addr, int port){
    addr->sin_family = AF_INET;
    ///the socket accepts connections to all the IPs of the machine.
    addr->sin_addr.s_addr = INADDR_ANY;
    addr->sin_port = htons(port);
}

Server::Server(int port_num) {
    this->port_num = port_num;
}

int Server::run() {
    int server_fd, sock;
    struct sockaddr_in address{};
    int yes = 1;
    int addrlen = sizeof(address);

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        throw std::runtime_error("Create socket failed");
    }

    ///SO_REUSEADDR 地址复用
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))){
        throw std::runtime_error("Failure in setsocketopt");
    }

    make_sockaddr(&address, port_num);

    /**bind() assigns
    *       the address specified by addr to the socket referred to by the file
    *       descriptor sockfd.  addrlen specifies the size, in bytes, of the
    *       address structure pointed to by addr.  Traditionally, this operation
    *       is called “assigning a name to a socket”.
    */
    if(bind(server_fd, (sockaddr*) &address, sizeof(address)) < 0){
        throw std::runtime_error("Bind failed");
    }

    /** int listen(int sockfd, int backlog) backlog means maximum requesting waiting this server can maintain.
     *
     */
    if(listen(server_fd, 3) < 0){
        throw std::runtime_error("listen failed.");
    }

    ///Starting receiving message using accept()
    while(true) {
        ///sock is a new socket using for send and receive data. New client has new socket
        sock = accept(server_fd, (sockaddr *) &address, (socklen_t *) &addrlen);
        auto t = std::thread(&Server::handle_connection, this, sock);
        t.detach();
    }
    return 0;
}

int Server::handle_connection(int sock) {
    char buffer[MAX_BUFFER_SIZE] = {0};
    try{
        if(sock == -1) throw std::runtime_error("Bad socket connection");
        read(sock, buffer, MAX_BUFFER_SIZE);
        ///implement a grep function here to get the grep result
        std::string s_res = grep(buffer);
        const char * res = s_res.c_str();

        ///Send back the result
        send(sock, res, strlen(res), 0);

        close(sock);
        return 0;
    } catch (std::runtime_error &e) {
        std::cerr << "error: " << e.what() << std::endl;
        std::string empty = "\0";
        const char *res = empty.c_str();
        send(sock, res, strlen(res), 0);
        close(sock);
        return -1;
    }
}




