//
// Created by Hypno on 3/3/2024.
//

#ifndef CPP_LIBMEM_PROCESS_HPP
#define CPP_LIBMEM_PROCESS_HPP

#include <vector>
#include <string>
#include <optional>
#include "../internal/winutils/winutils.hpp"
#define LM_TRUE 1
#define LM_FALSE 0
using lm_bool_t = BOOL;
using lm_pid_t = DWORD;
using lm_void_t = void;
using lm_size_t = size_t;

struct lm_process_t {
    lm_pid_t pid;             // Process ID
    lm_pid_t ppid;            // Parent Process ID
    std::string name;         // Process name
    std::string path;         // Full path to the process
    uint64_t start_time;      // Process start time
    size_t bits;              // Process bitness
};
std::vector<lm_process_t> LM_EnumProcesses();
std::optional<lm_process_t> get_process_entry(lm_pid_t pid);
static lm_process_t convert_entry_to_process(const PROCESSENTRY32W& entry);
lm_bool_t LM_GetProcessEx(lm_pid_t pid, lm_process_t *process_out);
lm_bool_t LM_GetProcess(lm_process_t *process_out);
#endif //CPP_LIBMEM_PROCESS_HPP
