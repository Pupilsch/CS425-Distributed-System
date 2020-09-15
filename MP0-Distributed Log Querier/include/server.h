//
// Created by Kitsch on 2020/9/8.
//


#ifndef MP0_DISTRIBUTED_LOG_QUERIER_SERVER_H
#define MP0_DISTRIBUTED_LOG_QUERIER_SERVER_H

#include "grep.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <cstring>
#include <thread>
#include <stdexcept>

//class Server{
//public:
//    Server();
//
//    static std::shared_ptr<Server> create(const Configuration& configuration_golbal, const Configuration& configuration_server);
//
//    bool configure(const Configuration& configuration_golbal, const Configuration& configuration_server);
//
//    short getMachineNumber();
//
//    std::shared_ptr<tcp::socket> accept();
//
//    void send(std::shared_ptr<tcp::socket> client_socket, std::string message);
//
//    std::string receive(std::shared_ptr<tcp::socket> client_socket);
//
//private:
//    boost::asio::io_service io_service_;
//    std::shared_ptr<tcp::acceptor> acceptor;
//    short machine_number_;
//};

class Server{
public:
    explicit Server(int port_num = 0);

    int run();

private:
    ///The port which the server is listening
    int port_num;

    /**
     * Thread for handle connection from client.
     *
     * Parameters:
     *      sock: The socket parameter for connection.
     */
    int handle_connection(int sock);
};

#endif //MP0_DISTRIBUTED_LOG_QUERIER_SERVER_H
