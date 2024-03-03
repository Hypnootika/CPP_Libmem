//
// Created by Hypno on 3/3/2024.
//


#include <optional>
#include "process.hpp"




lm_process_t convert_entry_to_process(const PROCESSENTRY32W &entry) {
    lm_process_t process;
    HANDLE hproc;
    WCHAR path[MAX_PATH + 1] = {};
    DWORD path_len = MAX_PATH;

    process.pid = entry.th32ProcessID;
    process.ppid = entry.th32ParentProcessID;
    process.name = wcstoutf8(entry.szExeFile).get();

    hproc = open_process(process.pid, PROCESS_QUERY_LIMITED_INFORMATION);
    if (hproc) {
        if (QueryFullProcessImageNameW(hproc, 0, path, &path_len)) {
            process.path = wcstoutf8(path).get();
        }
        get_process_start_time(hproc, &process.start_time);
        process.bits = get_process_bits(hproc);
        close_handle(hproc);
    }

    return process;
}
std::vector<lm_process_t> LM_EnumProcesses() {
    std::vector<lm_process_t> processes;
    auto entries = enum_process_entries();
    for (const auto& entry : entries) {
        processes.push_back(convert_entry_to_process(entry));
    }
    return processes;
}
std::optional<lm_process_t> get_process_entry(lm_pid_t pid) {
    auto entries = enum_process_entries();
    for (const auto& entry : entries) {
        if (entry.th32ProcessID == pid) {
            return convert_entry_to_process(entry);
        }
    }
    return std::nullopt; // Return empty if not found
}

lm_bool_t LM_GetProcess(lm_process_t *process_out) {
    if (!process_out) return LM_FALSE;

    auto pid = GetCurrentProcessId();
    auto optProcess = get_process_entry(pid);
    if (!optProcess) return LM_FALSE;

    *process_out = *optProcess;
    process_out->bits = get_system_bits();

    return LM_TRUE;
}

lm_bool_t LM_GetProcessEx(lm_pid_t pid, lm_process_t *process_out) {
    if (!process_out || pid == static_cast<lm_pid_t>(-1)) return LM_FALSE;

    auto optProcess = get_process_entry(pid);
    if (!optProcess) return LM_FALSE;

    *process_out = *optProcess;
    process_out->bits = get_system_bits();

    return LM_TRUE;
}

lm_size_t LM_GetSystemBits() {
    return get_system_bits();
}