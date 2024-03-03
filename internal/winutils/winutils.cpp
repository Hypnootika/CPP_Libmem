//
// Created by Hypno on 3/3/2024.
//

#include <string>
#include "winutils.hpp"



std::unique_ptr<char[]> wcstoutf8(const wchar_t *widestr) {
    if (!widestr) return nullptr;

    int requiredSize = WideCharToMultiByte(CP_UTF8, 0, widestr, -1, nullptr, 0, nullptr, nullptr);
    if (requiredSize <= 0) return nullptr;

    auto buffer = std::make_unique<char[]>(requiredSize);
    if (WideCharToMultiByte(CP_UTF8, 0, widestr, -1, buffer.get(), requiredSize, nullptr, nullptr) == 0) {
        return nullptr;
    }

    return buffer;
}

HANDLE open_process(DWORD pid, DWORD access) {
    if (pid == GetCurrentProcessId())
        return GetCurrentProcess();
    return OpenProcess(access, FALSE, pid);
}

void close_handle(HANDLE handle) {
    CloseHandle(handle);
}

size_t get_system_bits() {
    SYSTEM_INFO sysinfo = {};
    GetNativeSystemInfo(&sysinfo);
    switch (sysinfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
        case PROCESSOR_ARCHITECTURE_ARM64:
            return 64;
        case PROCESSOR_ARCHITECTURE_INTEL:
        case PROCESSOR_ARCHITECTURE_ARM:
            return 32;
        default:
            return 0; // Unknown architecture
    }
}

size_t get_process_bits(HANDLE hproc) {
    assert(hproc != NULL);
    BOOL is_wow64 = FALSE;
    // Directly return system bits if unable to determine process architecture
    if (!IsWow64Process(hproc, &is_wow64)) {
        return get_system_bits();
    }
    return is_wow64 ? 32 : get_system_bits(); // Adjust based on WOW64 status
}

uint64_t filetime_to_number(FILETIME *filetime) {
    assert(filetime != nullptr);
    return (static_cast<uint64_t>(filetime->dwHighDateTime) << 32) | filetime->dwLowDateTime;
}

BOOL get_process_start_time(HANDLE hproc, uint64_t *timestamp_out) {
    assert(hproc != NULL && timestamp_out != NULL);
    FILETIME creation_time, exit_time, kernel_time, user_time;
    if (!GetProcessTimes(hproc, &creation_time, &exit_time, &kernel_time, &user_time)) {
        return FALSE;
    }
    *timestamp_out = filetime_to_number(&creation_time) / 10000; // Convert to milliseconds
    return TRUE;
}

std::vector<PROCESSENTRY32W> enum_process_entries() {
    std::vector<PROCESSENTRY32W> processEntries;
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hsnap == INVALID_HANDLE_VALUE) {
        return processEntries; // Return empty vector on failure
    }
    PROCESSENTRY32W entry = {};
    entry.dwSize = sizeof(PROCESSENTRY32W);
    if (Process32FirstW(hsnap, &entry)) {
        do {
            processEntries.push_back(entry); // Add to vector
        } while (Process32NextW(hsnap, &entry));
    }
    CloseHandle(hsnap);
    return processEntries;
}