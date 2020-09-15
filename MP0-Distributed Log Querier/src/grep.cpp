//
// Created by Kitsch on 2020/9/9.
//

#include "../include/grep.h"

std::string grep(const std::string &input){
    ///Check whether system function is available
    if(!system(nullptr)){
        throw std::runtime_error("System is not available\n");
    }

    /// Run grep command.
    std::ostringstream command;
    command << input << " > result";
    if(system(command.str().c_str()) != 0){
        throw std::runtime_error("Not a grep command\n");
    }
    /// output now in a "result" file, put it back to file stream and get the str
    std::ifstream res("result");
    std::string str((std::istreambuf_iterator<char>(res)), std::istreambuf_iterator<char>());

    return str;
}
