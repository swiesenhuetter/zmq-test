#pragma once

#include <string>
#include <Windows.h>

inline std::string to_utf8(std::wstring_view wstr) {
    if (wstr.empty()) {
        return {};
    }
    int len = ::WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    std::string out(len, 0);
    ::WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &out[0], len, nullptr, nullptr);
    return out;
}


template <typename T>
concept Char8 = (sizeof(T) == 1);

template <Char8 T>
inline std::wstring from_utf8(std::basic_string_view<T> str) {
    if (str.empty()) {
        return {};
    }
    int len = ::MultiByteToWideChar(CP_UTF8, 0, (LPCH)(& str[0]), (int)str.size(), NULL, 0);
    std::wstring out(len, 0);
    ::MultiByteToWideChar(CP_UTF8, 0, (LPCH)&str[0], (int)str.size(), &out[0], len);
    return out;
}