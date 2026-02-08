#pragma once
#include <unordered_set>
#include <string>
#include <vector>

inline std::size_t exactF0(const std::vector<std::string>& prefix) {
    std::unordered_set<std::string> st;
    st.reserve(prefix.size());
    for (const auto& s : prefix) {
        st.insert(s);
    }
    return st.size();
}