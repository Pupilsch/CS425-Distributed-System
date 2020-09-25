//
// Created by Kitsch on 2020/9/23.
//

#include "../include/HeartBeatAll.h"

suseconds_t get_local_time_ms(){
    struct  timeval    tv;
    struct  timezone   tz;

    gettimeofday(&tv,&tz);

    return tv.tv_usec;
}

int heartBeat_all::create_send_sock() {
    int sock;
    try{
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            throw std::runtime_error("Creating udp socket failed.");
        }
        return sock;
    } catch (std::runtime_error &e) {
        std::cerr << "error: " << e.what() << std::endl;
    }
    return sock;
}

void heartBeat_all::convert_MembershipEntry_to_Message(MembershipEntry &row, MessageEntry &mes){
    strcpy(mes.ip_add, row.ip_add);
    mes.time_stamp = row.time_stamp;
    mes.incarnation_num = row.incarnation_num;
    mes.mType = GOSSIP;
    mes.status = row.status;
}

void heartBeat_all::convert_Message_to_MembershipEntry(MembershipEntry &row, MessageEntry &mes){
    strcpy(row.ip_add, mes.ip_add);
    row.time_stamp = get_local_time_ms();
    row.incarnation_num = mes.incarnation_num;
    row.status = mes.status;
}

heartBeat_all::heartBeat_all(int send_port_num, int listen_port_num, suseconds_t kill_t) {
    this->send_port_num_ = SEND_PORT;
    this->listen_port_num_ = LISTEN_PORT;
    this->kill_t_ = kill_t;

    /// Get the ip_address of current VM
    char const *default_net ="eth0";
    struct ifaddrs *ifAddrStruct = NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    while (ifAddrStruct != NULL){
        if (ifAddrStruct->ifa_addr->sa_family==AF_INET){
            if(strcmp(ifAddrStruct->ifa_name, default_net) == 0){ /// It's the eth0
                tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
                inet_ntop(AF_INET, tmpAddrPtr, this->local_ip_addr, INET_ADDRSTRLEN);
            }
        }
    }

    this->send_sock = create_send_sock();
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

int heartBeat_all::send_message_to_des(MessageEntry& mes, struct sockaddr_in *des_addr, int sock) {
    char buffer[MAX_BUFFER_LEN];
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, &mes, sizeof(MessageEntry));
    int len = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)des_addr, sizeof(des_addr)); //could be wrong
}

void make_sockaddr(struct sockaddr_in *addr, int port){
    addr->sin_family = AF_INET;
    ///the socket accepts connections to all the IPs of the machine.
    addr->sin_addr.s_addr = INADDR_ANY;
    addr->sin_port = htons(port);
}

int heartBeat_all::handle_message(MessageEntry mes, struct sockaddr_in* pusher){  /// Need Mutex
    if(mes.mType == GOSSIP){  /// Incoming message is gossip, check the ip and inc_num to update
        int flag = 0; // update to flag = 1 only when current node list have this ip address
        for(int i = 0; i < this->MembershipList.size(); ++i){
            if(strcmp(mes.ip_add, this->MembershipList[i].ip_add) == 0){
                flag = 1;
                if(mes.incarnation_num < this->MembershipList[i].incarnation_num) return 0;
                else if (mes.incarnation_num == this->MembershipList[i].incarnation_num){ /// Could be a message telling other it has fall
                    if(!mes.status && this->MembershipList[i].status){
                        this->MembershipList[i].status = false;
                        this->MembershipList[i].time_stamp = get_local_time_ms();
                        auto t = std::thread(&heartBeat_all::send_message_all, this, mes);  //all to all
                        t.detach();
                    }
                }
                else{//update the listentry
                    this->MembershipList[i].incarnation_num = mes.incarnation_num;
                    this->MembershipList[i].time_stamp = get_local_time_ms();
                    this->MembershipList[i].status = mes.status;
                    auto t = std::thread(&heartBeat_all::send_message_all, this, mes);   //all to all
                    t.detach();
                }
            }
        }
        if(!flag && mes.status){ /// This ip address is not found in the list, add it in
            MembershipEntry new_entry;
            convert_Message_to_MembershipEntry(new_entry, mes);
            this->MembershipList.push_back(new_entry);
            auto t = std::thread(&heartBeat_all::send_message_all, this, mes);   //all to all
            t.detach();
        }
    }

    if (mes.mType == JOIN){ /// Incoming message is join, send this join as gossip to all other node in the list, send back current list to the new join node
        int flag = 0;
        for(int i = 0; i < this->MembershipList.size(); ++i){
            if(strcmp(this->MembershipList[i].ip_add, mes.ip_add) == 0){
                flag = 1;
                this->MembershipList[i].status = true;
                this->MembershipList[i].incarnation_num = mes.incarnation_num;
                this->MembershipList[i].time_stamp = get_local_time_ms();
            }
        }
        if(!flag){
            MembershipEntry new_entry;
            convert_Message_to_MembershipEntry(new_entry, mes);
            this->MembershipList.push_back(new_entry);
        }
        send_message_all(mes);

        /// Then send it's entire MembershipList to the new joined VM
        for(int i = 0; i < this->MembershipList.size(); ++i){
            struct MessageEntry new_mes;
            convert_MembershipEntry_to_Message(this->MembershipList[i], new_mes);
            auto t = std::thread(&heartBeat_all::send_message_to_des, this, new_mes, pusher, this->send_sock);
            t.detach();
        }

    }
    return 0;
}

//int heartBeat_all::update_list(MembershipEntry& coming_entry){
//    if(!vm_status[coming_entry.vm_id]){
//        return 0;
//    }
//    if(coming_entry.incarnation_num <= this->MembershipList[coming_entry.vm_id].incarnation_num){
//        //adding some log operation
//        return -1;
//    }else{
//        this->MembershipList[coming_entry.vm_id].time_stamp = time(nullptr);
//        return 0;
//    }
//}

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

        MessageEntry coming_Message;
        memcpy(&coming_Message, buffer, sizeof(MessageEntry)); /// wait for a better idea
        auto t = std::thread(&heartBeat_all::handle_message, this, coming_Message, &pusher);
        t.detach();
    }
    return 0;
}

int heartBeat_all::send_heartBeat_all(){
    local_inc_num += 1;  ///increment local inc num first

    /// Init the heartbeat message for current VM
    MessageEntry heartBeatMes;
    strcpy(heartBeatMes.ip_add, this->local_ip_addr);
    heartBeatMes.status = true;
    heartBeatMes.mType = GOSSIP;
    heartBeatMes.incarnation_num = local_inc_num;
    heartBeatMes.time_stamp = get_local_time_ms();


    for(int i = 0; i < this->MembershipList.size(); ++i){ /// Switch All to all to gossip only need to change here
        if(this->MembershipList[i].status){  ///Send heartbeat only when the des VM is alive
            struct sockaddr_in des_addr{};
            des_addr.sin_family = AF_INET;
            des_addr.sin_port = htons(this->listen_port_num_);
            if (inet_pton(AF_INET, MembershipList[i].ip_add, &des_addr.sin_addr) <= 0)
                throw std::runtime_error("Invalid address");

            auto t = std::thread(&heartBeat_all::send_message_to_des, this, heartBeatMes, &des_addr, this->send_sock);  ///dont know why is wrong
            t.detach();
        }
    }

    return 0;
}

int heartBeat_all::send_message_all(MessageEntry& mes) {
    for (int i = 0; i < this->MembershipList.size(); ++i) { /// Switch All to all to gossip only need to change here
        if (this->MembershipList[i].status) {  ///Send heartbeat only when the des VM is alive
            struct sockaddr_in des_addr{};
            des_addr.sin_family = AF_INET;
            des_addr.sin_port = htons(this->listen_port_num_);
            if (inet_pton(AF_INET, MembershipList[i].ip_add, &des_addr.sin_addr) <= 0)
                throw std::runtime_error("Invalid address");

            auto t = std::thread(&heartBeat_all::send_message_to_des, this, mes, &des_addr,
                                 this->send_sock);  ///dont know why is wrong
            t.detach();
        }
    }
}
