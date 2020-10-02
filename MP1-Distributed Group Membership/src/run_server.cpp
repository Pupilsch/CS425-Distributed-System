#include "../include/server.h"

using namespace std;

int main(int argc, char const *argv[]) {
    auto *a_server = new server(SERVER_PORT_NUM);

    try{
        int res  = a_server->run_server();
    }catch(runtime_error &e){
        cerr << "Error: "<<e.what() << endl;
        delete a_server;
        return -1;
    }

    delete a_server;
    return 0;
}