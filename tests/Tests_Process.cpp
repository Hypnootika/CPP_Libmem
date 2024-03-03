#include <cassert>
#include <iostream>
#include "../include/Process.hpp" // Assuming this includes all the necessary function prototypes
void test_get_system_bits() {
    size_t bits = get_system_bits();
    std::cout << "System bits: " << bits << std::endl;
    assert(bits == 32 || bits == 64); // Assumes running on either 32 or 64 bit systems
    std::cout << "test_get_system_bits passed." << std::endl;
}

void test_get_process_bits() {
    HANDLE hProc = open_process(GetCurrentProcessId(), PROCESS_QUERY_INFORMATION);
    assert(hProc != NULL);
    size_t bits = get_process_bits(hProc);
    std::cout << "Process bits: " << bits << std::endl;
    assert(bits == 32 || bits == 64);
    close_handle(hProc);
    std::cout << "test_get_process_bits passed." << std::endl;
}

void test_filetime_to_number() {
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    uint64_t number = filetime_to_number(&ft);
    std::cout << "Filetime: " << number << std::endl;
    assert(number != 0);
    std::cout << "test_filetime_to_number passed." << std::endl;
}

void test_get_process_start_time() {
    HANDLE hProc = open_process(GetCurrentProcessId(), PROCESS_QUERY_INFORMATION);
    assert(hProc != NULL);
    uint64_t startTime = 0;
    BOOL result = get_process_start_time(hProc, &startTime);
    assert(result == TRUE && startTime != 0);
    close_handle(hProc);
    std::cout << "test_get_process_start_time passed." << std::endl;
}

void test_enum_process_entries() {
    auto entries = enum_process_entries();
    assert(!entries.empty());
    std::cout << "test_enum_process_entries passed." << std::endl;
    for (const auto& entry : entries) {
        std::cout << "PID: " << entry.th32ProcessID << " Name: " << wcstoutf8(entry.szExeFile) << std::endl;
    }
}

void test_LM_EnumProcesses_get_process_entry() {
    auto processes = LM_EnumProcesses();
    assert(!processes.empty()); // Ensure we can enumerate processes

    auto currentPID = GetCurrentProcessId();
    auto processOpt = get_process_entry(currentPID);
    assert(processOpt.has_value()); // Ensure we can find the current process
    std::cout << "test_LM_EnumProcesses_get_process_entry passed." << std::endl;
    for (const auto& process : processes) {
        std::cout << "PID: " << process.pid << " Name: " << process.name << std::endl;
    }
}

void test_LM_GetProcess_LM_GetProcessEx() {
    lm_process_t process;
    lm_bool_t result = LM_GetProcess(&process);
    assert(result == LM_TRUE);
    std::cout << "test_LM_GetProcess_LM_GetProcessEx passed." << result << std::endl;
    lm_process_t processEx;
    result = LM_GetProcessEx(GetCurrentProcessId(), &processEx);
    assert(result == LM_TRUE);
    assert(process.pid == processEx.pid); // Should be the same process ID
    std::cout << "test_LM_GetProcess_LM_GetProcessEx passed." << result << std::endl;
    std::cout << "Process Name: " << process.name << " Path: " << process.path << " Start Time: " << process.start_time << " Bits: " << process.bits << std::endl;
}

//lm_bool_t LM_GetProcess(lm_process_t *process_out)
void test_LM_GetProcess() {
    lm_process_t process;
    lm_bool_t result = LM_GetProcess(&process);
    assert(result == LM_TRUE);
    std::cout << "test_LM_GetProcess passed."<< result << std::endl;
    std::cout << "Process Name: " << process.name << " Path: " << process.path << " Start Time: " << process.start_time << " Bits: " << process.bits << std::endl;
}


//lm_bool_t LM_GetProcessEx(lm_pid_t pid, lm_process_t *process_out)
void test_LM_GetProcessEx() {
    lm_process_t process;
    lm_bool_t result = LM_GetProcessEx(GetCurrentProcessId(), &process);
    assert(result == LM_TRUE);
    std::cout << "test_LM_GetProcessEx passed." << result << std::endl;
    std::cout << "Process Name: " << process.name << " Path: " << process.path << " Start Time: " << process.start_time << " Bits: " << process.bits << std::endl;
}

int main() {
    test_get_system_bits();
    test_get_process_bits();
    test_filetime_to_number();
    test_get_process_start_time();
    test_enum_process_entries();
    test_LM_EnumProcesses_get_process_entry();
    test_LM_GetProcess_LM_GetProcessEx();
    test_LM_GetProcess();
    test_LM_GetProcessEx();
    std::cout << "All tests passed." << std::endl;
    return 0;
}
