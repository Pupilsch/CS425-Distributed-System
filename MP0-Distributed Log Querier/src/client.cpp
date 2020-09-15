//
// Created by Kitsch on 2020/9/4.
//

#include "../include/client.h"

#define MAX_BUFFER_SIZE 2048

Client::Client(int port_num) {
    this->port_num = port_num;
    ip_status.resize(ip_address.size(), true);
    query_status.resize(MAX_VM_NUM, false);
}

void delete_all_temp_files() {
    /// Temp files are named "out_*.txt".
    for (unsigned long vm_id = 0; vm_id < MAX_VM_NUM; vm_id++) {
        std::string file_name = "out_";
        file_name += (std::to_string(vm_id) + ".out");
        remove(file_name.c_str());
    }
}


int Client::run_client() { //get the input and get the query request into vector
    std::string input;

    auto processing_thread = std::thread(&Client::process_queries, this); //???

    processing_thread.detach();

    while(getline(std::cin, input)){
        /// "exit" to end the client
        if(input == "exit") return 0;
        if(input.empty()) continue;
        //Need implement when invalid query was entered
        /// Use mutex to push the query message to a quere
        queries_lock.lock();
        queries.push(input);
        queries_lock.unlock();
    }

    return 0;
}

int Client::send_query(int vm_id, std::string input) {
    int sock = 0;
    char buffer[MAX_BUFFER_SIZE] = {0};
    try {
        struct sockaddr_in serv_addr{};
        if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            throw std::runtime_error("Creating socket failed.");
        }
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(this->port_num);

        if (inet_pton(AF_INET, ip_address[vm_id].c_str(), &serv_addr.sin_addr) <= 0)
            throw std::runtime_error("Invalid address");

        /// Try to connect to server, if fail then mark server as down.
        if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
            throw std::runtime_error("Connection failed");

        const char *query = input.c_str();
        send(sock, query, strlen(query), 0);

        /// Now the server send back the result
        write_result.lock();
        std::string output_file_name = "out_";
        output_file_name += (std::to_string(vm_id) + ".out");
        std::ofstream outfile(output_file_name);

        /// Write the received message into the file
        while(read(sock, buffer, MAX_BUFFER_SIZE)){
            outfile << buffer;
            memset(buffer, 0, sizeof(buffer)); // clear the buffer for further reading
        }

        query_status_lock.lock();
        query_status[vm_id] = true; //now we now this vm is alive
        outfile.close();
        query_status_lock.unlock();
        write_result.unlock();

        
    } catch (std::runtime_error &e) {
        if (strcmp(e.what(), "Connection failed") == 0) {
            /// If connection failed, mark server as down.
            ip_status_lock.lock();
            cout_lock.lock();
            ip_status[vm_id] = false;
            std::cout << "@@@ Connection failed with vm " << vm_id + 1 << std::endl;
            cout_lock.unlock();
            ip_status_lock.unlock();
        } else {
            std::cerr << "error: " << e.what() << std::endl;
        }
        close(sock);
        return -1;
    }
}


/// Now process the queries you entered
void Client::process_queries() {
    while (true){
        if(queries.empty()) continue;

        queries_lock.lock();
        std::string curr_query = queries.front();
        queries.pop();
        queries_lock.unlock();

        delete_all_temp_files();

        ip_status_lock.lock();
        for(unsigned int vm_id = 0; vm_id < MAX_VM_NUM; ++vm_id){
            if(ip_status[vm_id]) std::thread(&Client::send_query, this, vm_id, curr_query).detach();
        }
        ip_status_lock.unlock();

        bool is_complete = false;
        while (!is_complete){
            is_complete = true;
            query_status_lock.lock();

            for(unsigned int vm_id = 0; vm_id < MAX_VM_NUM && is_complete; ++vm_id){
                is_complete = !(ip_status[vm_id] ^ query_status[vm_id]);
            }
            query_status_lock.unlock();
        }

        /// Implement for combining all results from different servers into one file
    }
}