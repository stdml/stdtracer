#pragma once
#include <iostream>
#include <string>

class unsafe_guard_t
{
    std::string file_;
    int line_;
    std::string hint_;

  public:
    unsafe_guard_t(std::string file, int line, std::string hint)
        : file_(std::move(file)), line_(line), hint_(std::move(hint))
    {
    }

    std::string msg() const
    {
        return hint_ + " @ " + file_ + ':' + std::to_string(line_);
    }

    template <typename F>
    decltype(auto) operator()(const F &f) const
    {
        try {
            return f();
        } catch (const std::invalid_argument &ex) {
            std::cerr << ex.what() << std::endl;
            throw std::runtime_error(msg());
        } catch (const std::runtime_error &ex) {
            std::cerr << ex.what() << std::endl;
            throw std::runtime_error(msg());
        } catch (...) {
            throw std::runtime_error(msg());
        }
    }
};
