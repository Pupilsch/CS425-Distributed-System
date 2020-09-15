//
// Created by Kitsch on 2020/9/14.
//

#include "../include/client.h"

int main(int argc, char const *argv[]) {
    int port_num = 8000;
    auto *my_client = new Client(port_num);

    try {
        my_client->run_client();
    } catch (std::runtime_error &e) {
        std::cerr << "error: " << e.what() << std::endl;
        delete my_client;
        return -1;
    }

    delete my_client;
    return 0;
}
