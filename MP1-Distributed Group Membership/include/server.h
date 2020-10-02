#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <cstring>
#include <thread>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <mutex>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <stdlib.h>
#include <unordered_set>

#define SERVER_PORT_NUM 9000

#define HEARTBEAT_DURATION 500
#define FAILURE_DETECTOR_DURATION 100
#define NUM_HEARTBEAT_SEND_TARGET 3
#define TIMEOUT_SUSPECT 2500
#define TIMEOUT_ERASE_MILLISECONDS 4000

#define MAX_BUFFER_SIZE 10000

//#define LOG_FILE_PATH "mp1.log"

using namespace std;

//enum mes_type{
//    JOIN,
//    LEAVE,
//    FAILURE,
//    HEARTBEAT,
//    JOIN_BACK,
//    ANNOUNCE,
//    SWITCH_TO_ALL,
//    SWITCH_TO_GOSSIP;
//};

#define JOIN 1
#define LEAVE 2
#define FAILURE 3
#define HEARTBEAT 4
#define JOIN_BACK 5
#define ANNOUNCE 6
#define SWITCH_TO_ALL 7
#define SWITCH_TO_GOSSIP 8

struct Member{
    char ip_address[14];
    //inc_num initially set to zero
    int inc_num;
    bool is_suspect;
    bool is_leave;
    uint64_t local_VM_timestamp;
};

struct Message{
    uint64_t mes_t;
    char ip_address[14];
    //inc_num initially set to zero
    int inc_num;
};

class server{
private:
    /// The port that server is listening.
    int port_num;

    /// The ip address of current machine.
    string my_ip_address;

    /// The ip address of introducer machine. (g05-02)
    const string introducer_ip = "172.22.94.15";

    /// A boolean parameter indicating whether current machine has joined the group.
    bool is_joined;

    /// A boolean parameter indicating whether it is gossip style now. (False for all to all)
    bool is_gossip;

    ///inc_num;
    int inc;

    unordered_map<string, Member> membershipList;
    unordered_map<string, Member> suspectList;
    mutex membershipList_lock;

//     /// The lock for writing log files.
//    mutex log_file_lock;

    /// The last time of sending a heartbeat to other machines.
    uint64_t last_send_heartbeat_time;

//    void run_tcp_receiver();

    int run_udp_receiver();

//    /**
//     * Handle the grep request from client.
//     */
//    int grep_call(int sock);

    /**
     * Update the current membership list 
     */
    void update_membership(vector<Message> recv_mes);


    /**
     * Convert a message struct to member struct
     */
    Member message_to_member(Message& mes);

    Message member_to_message(Member &mem);

    /**
     * Decode the received membership list from raw char* and convert network byte order to host order.
     *
     */
    vector<Message> buffer_to_message(char *buffer, int len_bytes);

    /**
     * Encode the received membership list before sending, convert host byte order to network order.
     *
     */
    vector<Message> update_newmes_timestamp(vector<Message> mes);


    /**
     * Get the current timestamp on this machine in milliseconds from epoch.
     *
     */
    static uint64_t get_curr_mseconds();

    /**
     * Return the IP address of current machine.
     */

    void handle_join_request(vector<Message>& recv_mes, sockaddr_in& clientaddr);

    /**
     * Handle "join" command from user input.
     */
    void handle_join_group();

    /**
     * Handle "leave" command from user input.
     */
    void handle_leave_group();

    /**
     * Switch the system from all to all to gossip or opposite
     */
    void switch_heartbeat_method(string command);

    /**
     * Handle send failure detected mes to target members
     */
    void send_one_mes_to_all(Message mes);

    /**
     * Send only one row udp mes to given target host.
     *
     */
    void run_udp_sender(string target_ip_addr, vector<Message> messages);

    /**
     * Handle sending heartbeat mess periodically.
     */
    void heartbeat_sender();

    /**
     * Print the current membership list to terminal.
     */
    void print_membershipList();

    void exit_print();

    void invalid_command_print();

    /**
     * Return the send target. For gossip, the target was generated randomly everytime when there is a heart beat send.
     */
    vector<string> find_send_targets();

    /**
     * Return the listen target of current server.
     */
    vector<string> find_listen_targets();  //no need


    /**
     * Detect whether a listen target has failed.
     */
    void failure_detector();

    /**
     * Print the current membership list to log file.
     */
//    void write_list_to_log();
//
//    /**
//     * store the last time heartbeat send target
//     */
     vector<string> last_time_target_list;
public:

    /**
     * Initializer for the server class.
     */
    server(int port_num);

    /**
     * Run the initialized server.
     */
    int run_server();
};


#endif SERVER_H