#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

using Date = std::string;

template<typename T>
struct Series {
    std::vector<Date> dates;
    std::vector<T> values;
    std::unordered_map<Date, size_t> idx;
    void finalize(){
        idx.clear();
        for (size_t i=0;i<dates.size();++i) idx[dates[i]] = i;
    }
    bool has(const Date& d) const { return idx.find(d)!=idx.end(); }
    T at(const Date& d) const { auto it=idx.find(d); if (it==idx.end()) throw std::runtime_error("Date missing: "+d); return values.at(it->second); }
};
