//
// Created by Kitsch on 2020/9/14.
//

#include "../include/server.h"

using namespace std;

int main(int argc, char const *argv[]) {
    int port_num = 8000;
    auto *my_server = new Server(port_num);

    try {
        my_server->run();
    } catch (std::runtime_error &e) {
        std::cerr << "error: " << e.what() << std::endl;
        delete my_server;
        return -1;
    }

    delete my_server;
    return 0;
}