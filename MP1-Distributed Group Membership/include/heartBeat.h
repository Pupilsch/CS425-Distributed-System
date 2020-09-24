//
// Created by Kitsch on 2020/9/23.
//

#ifndef CS425_DISTRIBUTED_SYSTEM_HEARTBEAT_H
#define CS425_DISTRIBUTED_SYSTEM_HEARTBEAT_H

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <cstring>
#include <thread>
#include <stdexcept>
#include <time.h>
#include <unordered_map>
#include <vector>

#define MAX_BUFFER_LEN 1024

void make_sockaddr(struct sockaddr_in *addr, int port);

struct MembershipEntry{ /// a row for membershiplist
    int vm_id;
    int incarnation_num;
    time_t time_stamp;
};


class heartBeat_all{
public:
    heartBeat_all(int send_port_num = 0, int listen_port_num = 1, time_t kill_t = 2);

    int run_heartBeat_all();

private:
    std::unordered_map<int, std::string> ip_address_list ; /// wait for a better idea
    std::vector<MembershipEntry> MembershipList;
    std::vector<bool> vm_status; ///true stands for alive

    int send_heartBeat_all();

    int listen_heartBeat_all();

    int update_list(MembershipEntry& coming_entry);

    int send_port_num_;
    int listen_port_num_;
    time_t kill_t_;  ///After not receving heartbeat for over kill_t a machine is considered failed

};


#endif //CS425_DISTRIBUTED_SYSTEM_HEARTBEAT_H
