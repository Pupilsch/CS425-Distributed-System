//
// Created by Kitsch on 2020/9/23.
//

#include "../include/HeartBeatAll.h"

void make_sockaddr(struct sockaddr_in *addr, int port){
    addr->sin_family = AF_INET;
    ///the socket accepts connections to all the IPs of the machine.
    addr->sin_addr.s_addr = INADDR_ANY;
    addr->sin_port = htons(port);
}

heartBeat_all::heartBeat_all(int send_port_num, int listen_port_num, time_t kill_t) {
    this->send_port_num_ = send_port_num;
    this->listen_port_num_ = listen_port_num;
    this->kill_t_ = kill_t;
}

int heartBeat_all::run_heartBeat_all() {
    if(listen_heartBeat_all() < 0){
        std::cerr<< "Listen fail" <<std::endl;
        return -1;
    }
    if(send_heartBeat_all() < 0){
        std::cerr<< "Send HeartBeat fail" <<std::endl;
        return -1;
    }
    return 0;
}

int heartBeat_all::update_list(MembershipEntry& coming_entry){
    if(!vm_status[coming_entry.vm_id]){
        return 0;
    }
    if(coming_entry.incarnation_num <= this->MembershipList[coming_entry.vm_id].incarnation_num){
        //adding some log operation
        return -1;
    }else{
        this->MembershipList[coming_entry.vm_id].time_stamp = time(nullptr);
        return 0;
    }
}

int heartBeat_all::listen_heartBeat_all() {
    int listen_fd;
    int yes = 1;
    struct sockaddr_in address{};

    if((listen_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0){
        throw std::runtime_error("Create listen socket failed");
    }

    make_sockaddr(&address, listen_port_num_);

    if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))){
        throw std::runtime_error("Failure in setsocketopt");
    }

    if(bind(listen_fd, (sockaddr*) &address, sizeof(address)) < 0){
        throw std::runtime_error("Bind failed");
    }

    struct sockaddr_in pusher;
    int iAddrLen = sizeof(pusher);

    char buffer[MAX_BUFFER_LEN] = {0};
    while (true){
        memset(buffer, 0, MAX_BUFFER_LEN);

        int len = recvfrom(listen_fd,buffer,MAX_BUFFER_LEN,0,(sockaddr*)&pusher,&iAddrLen);

        MembershipEntry coming_Membership;
        memcpy(&coming_Membership, buffer, sizeof(MembershipEntry)); /// wait for a better idea

    }
}
