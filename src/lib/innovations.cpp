#include "innovations.h"

#include <iostream>

innovations::innovations(size_t start) : current_innovation(start) {
}

size_t innovations::get_innovation(const std::vector<std::pair<ssize_t, ssize_t>>& connections, size_t from, size_t to) {
    try {
        std::vector<innovation>& possible_innovations = this -> innovs.at(from).at(to);

        for(innovation& i : possible_innovations) {
            if(i.connections.size() == connections.size() && i.from == from && i.to == to) {
                bool match = true;
                for(int c = 0; c < connections.size(); c++) {
                    if(connections.at(c) != i.connections.at(c)) {
                        match = false;
                        break;
                    }
                }

                if(match) {
                    return i.innovation_number;
                }
            }
        }
        return new_innovation(connections, from, to);

    } catch(std::out_of_range& e) {
        return new_innovation(connections, from, to);
    }
}

void innovations::clear() {
}

size_t innovations::new_innovation(const std::vector<std::pair<ssize_t, ssize_t>>& connections, size_t from, size_t to) {
    this -> innovs[from][to] = {innovation{this -> current_innovation, connections, from, to}};
   // std::cout << "Innov: " << this -> current_innovation << std::endl;
    return this -> current_innovation++;
}

innovations::~innovations() {
}