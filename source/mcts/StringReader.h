//
// Created by bjorn on 31-5-2026.
//

#ifndef FLEXFRINGE_STRING_READER_H
#define FLEXFRINGE_STRING_READER_H

#include <stdexcept>
#include <string>

struct StringReader {
private:
    std::string_view str;
    std::size_t      idx;

public:
    explicit StringReader(const std::string_view str) : str(str), idx(0) {
    }

    [[nodiscard]] bool hasNext() const { return idx < str.size(); }
    char               next() { return str[idx++]; }
    [[nodiscard]] char peek() const { return str[idx]; }


    void expect(char c) {
        if (!hasNext() || next() != c) throw std::invalid_argument(std::string("Expected '") + c + "'");
    }

    std::string readUntil(std::initializer_list<char> stops);

    void skipWhitespace() { while (hasNext() && peek() == ' ') next(); }
};


#endif //FLEXFRINGE_STRING_READER_H
