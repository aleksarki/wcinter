#pragma once
#ifndef CINTER_INCLUDE_CONSOLE_HPP
#define CINTER_INCLUDE_CONSOLE_HPP

#include <memory>
#include <string>

namespace cinter {

    class Console
    {
    private:
        class Impl;
        std::unique_ptr<Impl> pImpl;

    public:
        Console();
        ~Console();

        Console(const Console&) = delete;
        Console& operator=(const Console&) = delete;

        Console(Console&&) = default;
        Console& operator=(Console&&) = default;

        void write(const char* string);
        void write(const std::string& string);
        void write(const wchar_t* wstring);
        void write(const std::wstring& wstring);
    };

}

#endif  // CINTER_INCLUDE_CONSOLE_HPP
