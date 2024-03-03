//
// Created by Hypno on 3/3/2024.
//

#ifndef CPP_LIBMEM_WINUTILS_HPP
#define CPP_LIBMEM_WINUTILS_HPP
#include <cstdlib>
#include <cstdint>
#include <windows.h>
#include <cassert>

#include <tlhelp32.h>
#include <memory>
#include <vector>


//static std::string wide_to_utf8(const WCHAR* wideStr);
size_t get_system_bits();
size_t get_process_bits(HANDLE hproc);
uint64_t filetime_to_number(FILETIME *filetime);
BOOL get_process_start_time(HANDLE hproc, uint64_t *timestamp_out);
HANDLE open_process(DWORD pid, DWORD access);
void close_handle(HANDLE handle);
std::vector<PROCESSENTRY32W> enum_process_entries();
std::unique_ptr<char[]> wcstoutf8(const wchar_t *widestr);

#endif //CPP_LIBMEM_WINUTILS_HPP
