#include "../include/server.h"

using namespace std;

server::server(int port_num) {
    this->my_ip_address = "no address yet";
    struct ifaddrs *ifAddrStruct = nullptr;
    struct ifaddrs *tmpAddrPtr = nullptr;
    getifaddrs(&ifAddrStruct);

    while (ifAddrStruct != NULL){
        if (ifAddrStruct->ifa_addr->sa_family==AF_INET){
            if(strcmp(ifAddrStruct->ifa_name, "eth0") == 0){ /// It's the eth0
                this->my_ip_address=inet_ntoa(((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr);
            }
        }
        tmpAddrPtr = tmpAddrPtr->ifa_next;
    }

    /// Check whether we got the correct message
    if(this->my_ip_address == "no address yet"){
        cerr<< "Failure in getting this VM's ip address!"<<endl;
    }

    /// Init the member variable;
    this->port_num = port_num;
    this->is_joined = false;
    this->inc = 0;
    this->is_gossip = true;
    this->last_send_heartbeat_time = get_curr_mseconds();

    cout<<"$$ My ip address: "<< this->my_ip_address << endl;
    cout<<"$$ Introducer ip address: "<< this->introducer_ip<<endl;
}

void make_sockaddr(struct sockaddr_in *addr, int port){
    addr->sin_family = AF_INET;
    ///the socket accepts connections to all the IPs of the machine.
    addr->sin_addr.s_addr = INADDR_ANY;
    addr->sin_port = htons(port);
}

uint64_t server::get_curr_mseconds() {
    return (uint64_t) chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch()).count();
}

int server::run_server() {
    /// Multithread the three main function
    auto t1 = thread(&server::run_udp_receiver, this);
    auto t2 = thread(&server::heartbeat_sender, this);
    auto t3 = thread(&server::failure_detector, this);
    t1.detach();
    t2.detach();
    t3.detach();

    cout<< "Server is now running, type 'join' or 'exit" << endl;

    /// Waiting user input
    string input;
    while(getline(cin, input)){
        cout<<endl;
        if(input == "join"){
            if(!is_joined){
                handle_join_group();
            }else{
                cout<< "Already joined, invalid command!" << endl;
            }
        }else if(input == "exit"){
            exit_print();
            return 0;
        }else if(input == "leave"){
            if(is_joined){
                handle_leave_group();
            }else{
                cout<< "Already left, invalid command!" << endl;
            }
        }else if(input == "switch to all"){
            switch_heartbeat_method("all");
        }else if(input == "switch to gossip"){
            switch_heartbeat_method("gossip");
        }
        else{
            invalid_command_print();
        }
    }

    return 0;
}

void server::handle_join_group(){
    /// Create and send a membership list with only myself to introducer.
    Message join_mes{};
    strcpy(join_mes.ip_address, my_ip_address.c_str());
    this->inc += 1;
    join_mes.inc_num = this->inc;
    join_mes.mes_t = JOIN;
    vector<Message> mes = update_newmes_timestamp({join_mes});
    auto t = thread(&server::run_udp_sender, this, introducer_ip, mes);
    t.detach();
    cout<<"This VM" << my_ip_address<< "join the group"<< endl;
}

void server::handle_leave_group() {
    Message leave_mes{};
    strcpy(leave_mes.ip_address, my_ip_address.c_str());
    this->inc += 1;
    leave_mes.inc_num = this->inc;
    leave_mes.mes_t = LEAVE;
    vector<Message> mes = update_newmes_timestamp({leave_mes});
    vector<string> send_target = find_send_targets();
    for (auto &target : send_target){
        auto t = thread(&server::run_udp_sender, this, target, mes);
        t.detach();
    }
    membershipList_lock.lock();
    membershipList.clear();
    this->is_joined = false;
    membershipList_lock.unlock();
    cout<< "This VM voluntarily leaves."<<endl;
}

void server::switch_heartbeat_method(string command) {
    Message switch_mes;
    strcpy(switch_mes.ip_address, my_ip_address.c_str());
    if(command == "all"){
        this->is_gossip = false;
        switch_mes.mes_t = SWITCH_TO_ALL;
    }
    if(command == "gossip"){
        switch_mes.mes_t = SWITCH_TO_GOSSIP;
    }
    this->inc += 1;
    switch_mes.inc_num = inc;
}

void server::heartbeat_sender() {
    int cur_time = get_curr_mseconds();
    while (true){
        if(membershipList.size() <= 1) continue;
        if(get_curr_mseconds() - cur_time >= HEARTBEAT_DURATION){
            this->inc += 1;
            last_send_heartbeat_time = get_curr_mseconds();
            this->last_time_target_list = find_send_targets();
            Message mes_entry{};
            strcpy(mes_entry.ip_address, my_ip_address.c_str());
            mes_entry.inc_num = this->inc;
            mes_entry.mes_t = HEARTBEAT;
            for(auto s : last_time_target_list){
                run_udp_sender(s, {mes_entry});
            }
        }
    }
}

void server::failure_detector(){
    int cur_time = get_curr_mseconds();
    while (true){
        if(membershipList.size() <= 1) continue;
        if(get_curr_mseconds() - cur_time >= FAILURE_DETECTOR_DURATION){
            vector<string> listen_target = find_listen_targets();
            vector<Member> failed_member;
            membershipList_lock.lock();
            for(auto &target :listen_target){
                if(get_curr_mseconds() - membershipList[target].local_VM_timestamp > TIMEOUT_SUSPECT && !membershipList[target].is_suspect && !membershipList[target].is_leave){
                    membershipList[target].is_suspect = true;
                    suspectList[target] = membershipList[target];
                    suspectList[target].local_VM_timestamp = get_curr_mseconds();
                    cout<< "# Suspecting " << suspectList[target].ip_address << endl;
                }
            }

            for(auto &suspect : suspectList){
                if(get_curr_mseconds() - suspect.second.local_VM_timestamp > TIMEOUT_ERASE_MILLISECONDS){
                    cout<< " Time out, "<<suspect.first<< "is erased."<<endl;
                    suspect.second.is_leave = true;
                    if(membershipList[suspect.first].is_suspect){
                        send_one_mes_to_all(member_to_message(suspect.second));
                    }
                    suspectList.erase(suspect.first);
                    membershipList.erase(suspect.first);
                }
            }
        }
    }
}

Message server::member_to_message(Member &mem){
    Message mes;
    strcpy(mes.ip_address, mem.ip_address);
    if(mem.is_leave && mem.is_suspect){
        mes.mes_t = FAILURE;
    }
    if(mem.is_leave && !mem.is_suspect){ /// could be wrong
        mes.mes_t = LEAVE;
    }
    mes.inc_num = mem.inc_num + 1; /// could be wrong
    return mes;
}

vector<string> server::find_listen_targets() {
    membershipList_lock.lock();
    vector<string> listen_target;
    auto it = membershipList.begin();
    for(; it != membershipList.end(); ++it){
        if (it->first == my_ip_address || it->second.is_leave) continue;
        listen_target.push_back(it->first);
    }
    membershipList_lock.unlock();
    return listen_target;
}

int server::run_udp_receiver() {
    int server_fd, sock;
    struct sockaddr_in serveraddr{}, clientaddr{};
    int yes = 1;
    int clientaddr_len = sizeof(clientaddr);

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        throw runtime_error("Failed in creat socket fd");
        return -1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))){
        throw runtime_error("Failed in set socket opt");
        return -1;
    }

    make_sockaddr(&serveraddr, this->port_num);

    if (bind(server_fd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0){
       throw runtime_error("Failed in bind");
        return -1;
    }

    /// Receiving message using while true loop
    while (true){
        char buffer[MAX_BUFFER_SIZE] = {0};
        int recv_len = recvfrom(server_fd, (char *) buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *) &clientaddr,reinterpret_cast<socklen_t *>(&clientaddr_len));
        if(recv_len < 0) continue;

        vector<Message> recv_mes = buffer_to_message(buffer, recv_len);
        uint64_t message_type = recv_mes[0].mes_t;

        vector<Message> mes_to_send;

        if(message_type == JOIN){
            handle_join_request(recv_mes, clientaddr);
        }else if(message_type == HEARTBEAT || message_type == LEAVE || message_type == ANNOUNCE || message_type == FAILURE ||message_type == SWITCH_TO_GOSSIP || message_type == SWITCH_TO_ALL){
            auto t = thread(&server::update_membership, this, recv_mes);
            t.detach();
        }else if(message_type == JOIN_BACK){
            cout<< "$$ Received JOIN_BACK from introducer"<<endl;
            /// Update my local membership list according to introducer's list.
            this->is_joined = true;
            auto t = thread(&server::update_membership, this, recv_mes);
            t.detach();
        }else{
            invalid_command_print();
        }
    }
}

void server::handle_join_request(vector<Message> &recv_mes, sockaddr_in& clientaddr) {
    cout<< "$$ Received join request form" << inet_ntoa(clientaddr.sin_addr) <<endl;
    update_membership(recv_mes); // to be continued
    vector<Message> mes;
    for(auto it : membershipList){
        Message temp = member_to_message(it.second);
        temp.mes_t = JOIN_BACK;
        mes.push_back(temp);
    }
    mes = update_newmes_timestamp(mes);
    run_udp_sender(inet_ntoa(clientaddr.sin_addr), mes);
}

void server::update_membership(vector<Message> recv_mes) {
    membershipList_lock.lock();
    bool chain_send = false;
    uint64_t recv_type = recv_mes[0].mes_t;
    Message first_mes = recv_mes[0];

    switch (recv_type) {
        case ANNOUNCE:
            if(membershipList.find(first_mes.ip_address) == membershipList.end()){
                cout << "$$ Received ANNOUNCE form "+ string(first_mes.ip_address) << endl;
                membershipList[first_mes.ip_address] = message_to_member(first_mes);
                chain_send = true;
            }
            break;

        case JOIN:
            cout << "$$ Received JOIN form "+ string(first_mes.ip_address) << endl;
            membershipList[first_mes.ip_address] = message_to_member(first_mes);
            chain_send = true;
            break;

        case JOIN_BACK:
            cout << "$$ Receive JOIN_BACK list from introducer, updating list" <<endl;
            for(auto m : recv_mes){
                Member member_row = message_to_member(m);
                membershipList[m.ip_address] = member_row;
            }
            break;

        case FAILURE:
            cout << "$$ Received FAILURE form "+ string(first_mes.ip_address) << endl;
            membershipList.erase(first_mes.ip_address);
            chain_send = true;
            break;

        case LEAVE:
            cout << "$$ Received LEAVE form "+ string(first_mes.ip_address) << endl;
            membershipList.erase(first_mes.ip_address);
            chain_send = true;
            break;

        case SWITCH_TO_ALL:
            is_gossip = false;
            break;

        case SWITCH_TO_GOSSIP:
            is_gossip = true;
            break;

        default: /// mes_type is heartbeat
            if(membershipList[first_mes.ip_address].is_suspect || membershipList[first_mes.ip_address].is_leave){ // Receive heart beat, unsuspect
                if(first_mes.inc_num > membershipList[first_mes.ip_address].inc_num){
                    cout<< "$$ Unsuspect "<< first_mes.ip_address <<endl;
                    suspectList.erase(first_mes.ip_address);
                    chain_send = true;
                }
            }
            /// Update inc_num and timestamp
            if(first_mes.inc_num > membershipList[first_mes.ip_address].inc_num){
                membershipList[first_mes.ip_address].inc_num = first_mes.inc_num;
                membershipList[first_mes.ip_address].local_VM_timestamp = get_curr_mseconds();
                chain_send = true;
            }
    }
    membershipList_lock.unlock();
    if(chain_send){
        send_one_mes_to_all(first_mes);
    }
}

void server::send_one_mes_to_all(Message mes) {
    vector<string> send_target = find_send_targets();
    vector<Message> mes_to_send = update_newmes_timestamp({mes});
    for(auto &target : send_target){
        auto t = thread(&server::run_udp_sender, this, target, mes_to_send);
        t.detach();
    }
}

void server::run_udp_sender(string target_ip_addr, vector<Message> messages){/// This part is cited online
    int sock_fd, num_bytes;
    struct sockaddr_in serveraddr{};
    struct hostent *server;

    /// Get socket descriptor.
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0)
        cerr<< "Failure in create UDP socket" <<endl;

    /// Get the target server's DNS entry.
    server = gethostbyname(target_ip_addr.c_str());
    if (server == nullptr)
        cerr << "Failure in find target host" <<endl;

    /// Build the server's Internet address.
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(port_num);

    /// Construct the message item to send.
    Message packet[messages.size()];

    for (int i = 0; (size_t) i < messages.size(); i++)
        packet[i] = messages[i];

    char buf[sizeof(packet)];
    bzero(buf, sizeof(packet));

    memcpy(buf, &packet, sizeof(packet));

    /// Send the message to the server.
    num_bytes = sendto(sock_fd, buf, sizeof(buf), 0,
                       reinterpret_cast<const sockaddr *>(&serveraddr), sizeof(serveraddr));

    if (num_bytes < 0)
        cerr<<"Failure in sending message"<<endl;

    close(sock_fd);
}



vector<Message> server::update_newmes_timestamp(vector<Message> mes) {
    for (int i = 0; (size_t) i < mes.size(); i++) {
        mes[i].inc_num = htonl(mes[i].inc_num);
        mes[i].mes_t = uint64_t(htonl(mes[i].mes_t));
    }
    return mes;
}

vector<string> server::find_send_targets() {
    membershipList_lock.lock();
    vector<string> res;
    if(this->is_gossip){//implement the gossip style find target
        auto it = membershipList.find(my_ip_address);
        //int send_count = 0;
        int len_list = membershipList.size();

        /// Mark the send target
        //unordered_set<string> target_sent;
        for(int j = 0; j < NUM_HEARTBEAT_SEND_TARGET; ++j){
            int count = 0;
            while(count < 100){ /// Could be wrong
                int r = rand() % len_list;
                for(int i = 0; i < r; ++i){
                    it++;
                    if (it == membershipList.end()) it = membershipList.begin();
                }
                if (it->second.is_leave= LEAVE) continue;
                if(it != membershipList.find(my_ip_address) && find(res.begin(),res.end(), it->first) == res.end()){
                    res.push_back(it->first);
                    break;
                }
                count++;
            }
        }
        this->last_time_target_list.clear();
        for(auto str: res){
            last_time_target_list.push_back(str);
        }
        membershipList_lock.unlock();
    }else{
        auto it = membershipList.begin();
        //int send_count = 0;
        //int len_list = membershipList.size();


        ///Here is all to all implementation
        for(; it != membershipList.end(); ++it){
            if (it->second.is_leave) continue;
            if(it != membershipList.find(my_ip_address)){
                res.push_back(it->first);
            }
        }

        this->last_time_target_list.clear();
        for(auto str: res){
            last_time_target_list.push_back(str);
        }
        membershipList_lock.unlock();
    }
    return res;
}

Member server::message_to_member(Message &mes) {
    Member mem_entry;
    strcpy(mem_entry.ip_address,mes.ip_address);
    mem_entry.inc_num = mes.inc_num;
    mem_entry.local_VM_timestamp = get_curr_mseconds();
    mem_entry.is_suspect = false;
}

vector<Message> server::buffer_to_message(char *buffer, int len_bytes) {
    vector<Message> mes;
    char *it = buffer;
    while(len_bytes > 0){
        auto *mes_entry = (Message*) it;
        mes_entry->mes_t = uint64_t(ntohl(mes_entry->mes_t));
        mes_entry->inc_num = ntohl(mes_entry->inc_num);
        mes.push_back(*mes_entry);
        it += sizeof(Message);
    }
    return mes;
}


void server::exit_print() {
    cout<< "Exit the system! Bye" <<endl;
}

void server::invalid_command_print() {
    cout<< "Invalid command! Please input again." <<endl;
}

void server::print_membershipList(){
    membershipList_lock.lock();
    for (const auto &m : membershipList)
        cout << m.first << " " << m.second.inc_num << " " << m.second.local_VM_timestamp<< " "<< m.second.is_suspect <<" "<< m.second.is_leave << endl;
    membershipList_lock.unlock();
    auto listen_list = find_listen_targets();
    auto send_list = find_send_targets();
    cout << "$$ Listening:" << endl;
    for (const auto &m : listen_list)
        cout << m << endl;
    cout << "$$ Last time send to:" << endl;
    for (const auto &m : last_time_target_list)
        cout << m << endl;
}