#pragma once

#include <map>
#include <vector>
#include <iostream>

#include "population_config.h"
#include "innovations.h"

struct connection {
    size_t innov_number;

    size_t from_node;
    size_t to_node; 

    double weight = 1;
    bool active = true;   
};

struct node_data {
    size_t layer;
    size_t prev_nodes;
};

typedef double(*activation_t)(double);

class genome {
private:
   // static size_t innov_number;

    double fitness;

    activation_t activation;

    std::map<size_t, std::map<size_t, connection>> network;
    std::vector<node_data> nodes;
    std::vector<std::pair<ssize_t, ssize_t>> connections;

    void mutate_weights(const population_config& config);
    void mutate_node(innovations& innovation_history);
    void mutate_connection(innovations& innovation_history);
    void mutate_state(const bool state);
public:
    size_t input_count, output_count;
    genome(const size_t input_count, const size_t output_count, activation_t activation);
    void add_connection(const connection& c);
    void add_connection(const size_t innov_number, const size_t from_node, const size_t to_node, double weight = 1, bool active = true);
    void add_node(const node_data& node);
    void add_node(const size_t layer, const size_t prev_nodes);
    std::vector<double> run(const std::vector<double>& inputs) const;
    bool is_input(const size_t node_id) const;
    bool is_output(const size_t node_id) const;
    double evaluate_node(const size_t node_id, const std::vector<double>& inputs) const;
    bool is_fully_connected();
    bool does_connection_exist(const size_t from_node, const size_t to_node) const;
    genome& mutate(const population_config& config, innovations& innovation_history);
    genome breed(const genome& other);
    double compare(const genome& other, const population_config& config) const;
    double get_fitness() const;
    void set_fitness(double value);
    void reward(double value);
    ~genome();

    friend std::ostream& operator<<(std::ostream& stream, const genome& gen);

    static double rand_range(const double min, const double max);
};

inline std::ostream& operator<<(std::ostream& stream, const genome& gen) {
    stream << "Genome {" << std::endl;
    stream << "    Nodes: [" << std::endl;

    for(int d = 0; d <  gen.nodes.size(); d++) {
        const node_data& data = gen.nodes.at(d);
        stream << "        (ID: " << d << ", Layer: " << data.layer << ", Max_Inputs: " << data.prev_nodes << ")" << std::endl;      
    }
    stream << "    ]" << std::endl;
    stream << "    Connections: [" << std::endl;
    for(const auto& p : gen.network) {
        for(const auto& p0 : p.second ) {
            stream << "        (ID: " << p0.second.innov_number << ", From: " << p0.second.from_node << ", To: " << p0.second.to_node << ", Weight: " << p0.second.weight;
            if(p0.second.active) {
                stream << ", Active: true)" << std::endl;
            } else {
                stream << ", Active: false)" << std::endl;
            }
        }
    }
    stream << "    ]" << std::endl;
    stream << "    Fitness: " << gen.fitness << std::endl;
    stream << "}";

    return stream;
}