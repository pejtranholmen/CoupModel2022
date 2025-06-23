#pragma once

template<typename T, typename KeyFunc>
std::vector<T> FUtil::SortBy(const std::vector<T>& vec, KeyFunc keyFunc) {
    std::vector<T> sorted = vec;
    std::sort(sorted.begin(), sorted.end(), [&](const T& a, const T& b) {
        return keyFunc(a) < keyFunc(b);
    });
    return sorted;
}
