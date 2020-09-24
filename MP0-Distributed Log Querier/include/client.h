//
// Created by Kitsch on 2020/9/8.
//

#ifndef MP0_DISTRIBUTED_LOG_QUERIER_CLIENT_H
#define MP0_DISTRIBUTED_LOG_QUERIER_CLIENT_H

#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <iterator>
#include <fstream>
#include <cassert>
#include <sstream>
#include <unordered_map>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <queue>
#include <algorithm>

#define MAX_VM_NUM 10

class Client{
public:
    Client(int port_num = 0);

    int run_client();

private:
    /// Port number which the server is listening
    int port_num;

    std::mutex cout_lock;
    std::mutex write_result;

    std::queue<std::string> queries;
    std::mutex queries_lock;

    std::unordered_map<int, std::string> ip_address;

    /// The bool vector that stores the status (alive or died) for vm servers.
    std::vector<bool> ip_status;
    std::mutex ip_status_lock;

    /// The bool that stores the status (alive or died) for different VMs.
    std::vector<bool> query_status;
    std::mutex query_status_lock;

    /// For sending queries to specific VM.
    int send_query(int vm_id, std::string input);

    void process_queries();
};

#endif //MP0_DISTRIBUTED_LOG_QUERIER_CLIENT_H
