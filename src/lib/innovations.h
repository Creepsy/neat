#pragma once

#include <vector>
#include <map>
#include <cstdlib>

struct innovation
{
    size_t innovation_number;
    std::vector<std::pair<ssize_t, ssize_t>> connections;
    size_t from, to;
};

class innovations
{
private:
    std::map<size_t, std::map<size_t, std::vector<innovation>>> innovs;
    size_t current_innovation;
public:
    innovations(size_t start);
    size_t get_innovation(const std::vector<std::pair<ssize_t, ssize_t>>& connections, size_t from, size_t to);
    void clear();
    size_t new_innovation(const std::vector<std::pair<ssize_t, ssize_t>>& connections, size_t from, size_t to);
    ~innovations();
};



