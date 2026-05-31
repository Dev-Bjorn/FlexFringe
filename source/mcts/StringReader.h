//
// Created by bjorn on 31-5-2026.
//

#ifndef FLEXFRINGE_STRING_READER_H
#define FLEXFRINGE_STRING_READER_H

#include <string>

struct StringReader {
private:
    std::string_view str;
    std::size_t idx;

public:
    explicit StringReader(const std::string_view str) : str(str), idx(0) {
    }

    [[nodiscard]] bool hasNext() const { return idx < str.size(); }
    char next() { return str[++idx]; }

    [[nodiscard]] char peek() const { return str[idx]; }


    void expect(char c) {
        if (!hasNext() || next() != c)
            throw std::invalid_argument(std::string("Expected '") + c + "'");
    }

    // Read until one of the stop characters (not consuming it)
    std::string readUntil(const std::initializer_list<char> stops) {
        std::string result;
        while (hasNext()) {
            const char c = peek();
            for (const char stop : stops)
                if (c == stop) return result;
            result += next();
        }
        return result;
    }

    void skipWhitespace() {
        while (hasNext() && peek() == ' ') next();
    }
};



#endif //FLEXFRINGE_STRING_READER_H
