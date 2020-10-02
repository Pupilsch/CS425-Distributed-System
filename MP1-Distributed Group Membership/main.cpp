//
// Created by Kitsch on 2020/9/25.
//

#include <iostream>
#include "include/heartbeat.h"

int main(int argc, char* argv[]) {
    heartBeat_all hall(SEND_PORT, LISTEN_PORT);
    hall.run_heartBeat_all();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
