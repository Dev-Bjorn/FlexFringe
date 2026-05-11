//
// Created by bjorn on 8-5-2026.
//

#ifndef FLEXFRINGE_STRINGS_H
#define FLEXFRINGE_STRINGS_H
#include <algorithm>
#include <ranges>
#include <string>

inline std::string toLower(std::string s) {
    std::ranges::transform(s, s.begin(), [](unsigned char c) -> char {
        return static_cast<char>(std::tolower(c));
    });
    return s;
}




inline auto toLowerView(std::string_view s) {
    return s | std::views::transform([](unsigned char c) -> char {
        return static_cast<char>(std::tolower(c));
    });
}

inline std::string toLower(std::string_view s) {
    auto r = toLowerView(s);
    return {r.begin(), r.end()};
}

#endif //FLEXFRINGE_STRINGS_H
