//
// Created by bjorn on 31-5-2026.
//

#include "StringReader.h"


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