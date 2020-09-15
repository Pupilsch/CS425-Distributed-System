//
// Created by Kitsch on 2020/9/9.
//

#ifndef MP0_DISTRIBUTED_LOG_QUERIER_GREP_H
#define MP0_DISTRIBUTED_LOG_QUERIER_GREP_H

#include <iostream>
#include <iterator>
#include <fstream>
#include <cassert>
#include <sstream>
#include <stdexcept>

/**
 * Run the given grep command using system call.
 *
 * Parameters:
 *      input: A grep command entered by user in command line.
 *
 * Returns:
 *      A string of grep result (contain tailing \n char).
 */

std::string grep(const std::string input);

#endif //MP0_DISTRIBUTED_LOG_QUERIER_GREP_H
