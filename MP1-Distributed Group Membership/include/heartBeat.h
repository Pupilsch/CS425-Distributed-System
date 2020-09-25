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
#include <sys/time.h>
#include <unordered_map>
#include <vector>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>

#define LISTEN_PORT 8000
#define SEND_PORT 8001

#define MAX_BUFFER_LEN 1024

void make_sockaddr(struct sockaddr_in *addr, int port);

enum MessageType{
    JOIN,
    GOSSIP
};

struct MessageEntry{
    MessageType mType;
    char[14] ip_add;
    int incarnation_num;
    suseconds_t time_stamp;   /// highest priority in this struct, if its greater than current number, update all other information in struct
    bool status;   ///when a VM leaves or down, this status was set to false;
};

struct MembershipEntry{ /// a row for membershiplist
    char[14] ip_add;
    int incarnation_num;
    suseconds_t time_stamp;   /// highest priority in this struct, if its greater than current number, update all other information in struct
    bool status;   ///when a VM leaves or down, this status was set to false;
};


class heartBeat_all{
public:
    heartBeat_all(int send_port_num = 8001, int listen_port_num = 8000, suseconds_t kill_t = 2000);

    int run_heartBeat_all();

private:
    std::vector<MembershipEntry> MembershipList;

    char[14] local_ip_addr;

    int create_send_sock(); /// return a socket for send at the beginning, store it in send_sock;

    int send_message_to_des(MessageEntry& mes, struct sockaddr_in* des_addr, int sock);

    int send_heartBeat_all();

    int send_message_all(MessageEntry& mes); /// For all to all, send a specific message to all other VMs

    int listen_heartBeat_all();

    int check_list(MembershipEntry& coming_entry);

    int handle_message(MessageEntry mes, struct sockaddr_in* pusher);

    void convert_MembershipEntry_to_Message(MembershipEntry &row, MessageEntry &mes);

    void convert_Message_to_MembershipEntry(MembershipEntry &row, MessageEntry &mes);

    int send_port_num_;
    int send_sock;
    int listen_port_num_;
    suseconds_t kill_t_;  ///After not receving heartbeat for over kill_t a machine is considered failed
    int local_inc_num = 0;
};


#endif //CS425_DISTRIBUTED_SYSTEM_HEARTBEAT_H
