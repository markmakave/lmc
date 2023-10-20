#pragma once

#include <istream>
#include <vector>

namespace lmc {

class source {
public:

    source(std::istream& stream) : _stream(stream) {}

    size_t position() {
        return _cache.size();
    }

    char peek() {
        return _stream.peek();
    }

    char get() {
        _cache.push_back(_stream.get());
        return _cache.back();
    }

    bool eof() {
        return _stream.eof();
    }

    std::vector<char>::iterator begin() {
        return _cache.begin();
    }

    char operator [] (size_t index) {
        return _cache[index];
    }

protected:

    std::vector<char> _cache;
    std::istream& _stream;
};

}
