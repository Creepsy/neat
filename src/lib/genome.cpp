#include "genome.h"

#include <random>
#include <iostream>

//size_t genome::innov_number = 0;

genome::genome(const size_t input_count, const size_t output_count, activation_t activation) :
    input_count(input_count), output_count(output_count), activation(activation), fitness(0) {

    for(int i = 0; i < input_count; i++){
        this -> add_node(0, 0);
    }

    for(int o = 0; o < output_count; o++) {
        this -> add_node((size_t) INT32_MAX + 1, input_count);
    }

    size_t innov = 0;
    for(int i = 0; i < input_count; i++){
        for(int o = 0; o < output_count; o++){
            this -> add_connection(innov++, i, input_count + o, this -> rand_range(-1, 1));
        }
    }
   
  /*  std::cout << this -> innov_number << std::endl;
    for(auto p : this -> network) {
        for(auto p0 : p.second ) {
            std::cout << p0.second.innov_number << " -> " << p0.second.from_node << " / " << p0.second.to_node << " (" << p0.second.weight << ")" << std::endl;
        }
    }

    for(int d = 0; d < this -> nodes.size(); d++) {
        node_data& data = this -> nodes.at(d);
        std::cout << d << " -> " << "(" << data.layer << ", " << data.prev_nodes << ")" << std::endl;
    }*/
}

void genome::add_connection(const connection& c) {
    this -> network[c.to_node][c.from_node] = c;
    while(this -> connections.size() <= c.innov_number) {
        this -> connections.push_back(std::make_pair(-1, -1));
    }
    this -> connections.at(c.innov_number) = std::make_pair(c.from_node, c.to_node);
}

void genome::add_connection(const size_t innov_number, const size_t from_node, const size_t to_node, double weight, bool active) {
    this -> network[to_node][from_node] = connection{innov_number, from_node, to_node, weight, active};
    while(this -> connections.size() <= innov_number) {
        this -> connections.push_back(std::make_pair(-1, -1));
    }
    this -> connections.at(innov_number) = std::make_pair(from_node, to_node);
}

void genome::add_node(const node_data& node) {
    this -> nodes.push_back(node);
}

void genome::add_node(const size_t layer, const size_t prev_nodes) {
    this -> nodes.push_back(node_data{layer, prev_nodes});
}

std::vector<double> genome::run(const std::vector<double>& inputs) const {
    std::vector<double> results(this -> output_count);

    for(int o = this -> input_count; o < this -> input_count + this -> output_count; o++){
        results.at(o - this -> input_count) = this -> evaluate_node(o, inputs);
    }

    return results;
}

bool genome::is_input(const size_t node_id) const {
    return node_id < this -> input_count;
}

bool genome::is_output(const size_t node_id) const {
    return !this -> is_input(node_id) && node_id < this -> input_count + this -> output_count;
}

double genome::evaluate_node(const size_t node_id, const std::vector<double>& inputs) const {
    if(this -> is_input(node_id)) return inputs.at(node_id);

    double sum = 0;

    for(const std::pair<size_t, connection>& conn : this -> network.at(node_id)) {
        if(conn.second.active) sum += this -> evaluate_node(conn.first, inputs) * conn.second.weight;
    }

    return this -> activation(sum);
}

bool genome::is_fully_connected() {
    for(int n = 0; n < this -> nodes.size(); n++){
        if(!this -> is_input(n)) {
            if(this -> network.at(n).size() < this -> nodes.at(n).prev_nodes) {
                return false;
            }
        }
    }

    return true;
}

bool genome::does_connection_exist(const size_t from_node, const size_t to_node) const {
    try {
        this -> network.at(to_node).at(from_node);
    }
    catch(const std::out_of_range& e) {
        return false;
    }

    return true;
}

genome& genome::mutate(const population_config& config, innovations& innovation_history) {
    if(this -> rand_range(0, 1) <= config.mutate_weights) {
        this -> mutate_weights(config);
    }

    if(this -> rand_range(0, 1) <= config.mutate_node) {
        this -> mutate_node(innovation_history);
    }

    if(this -> rand_range(0, 1) <= config.mutate_connection) {
        this -> mutate_connection(innovation_history);
    }

    if(this -> rand_range(0, 1) <= config.mutate_disable) {
        this -> mutate_state(false);
    }

    if(this -> rand_range(0, 1) <= config.mutate_enable) {
        this -> mutate_state(true);
    }

    return *this;
}

genome genome::breed(const genome& other) {
    genome child = genome{this -> input_count, this -> output_count, this -> activation};
    child.nodes = this -> nodes;
    child.network.clear();
    child.connections.clear();

    for(auto& to : this -> network) {
        for(auto& from : to.second ) {
            if(other.does_connection_exist(from.first, to.first) && other.network.at(to.first).at(from.first).innov_number == from.second.innov_number) {
                connection c;
                if(this -> rand_range(0, 1) <= 0.5) {
                    c = connection{from.second.innov_number, from.second.from_node, from.second.to_node, from.second.weight};
                } else {
                    c = other.network.at(to.first).at(from.first);
                }

                if(!from.second.active || !other.network.at(to.first).at(from.first).active){
                    if(this -> rand_range(0, 1) <= 0.75) {
                        c.active = false;
                    }
                }
                child.add_connection(c);

            } else {
                if(!from.second.active && this -> rand_range(0, 1) <= 0.75) {
                    child.add_connection(connection{from.second.innov_number, from.second.from_node, from.second.to_node, from.second.weight, false});
                } else {
                    child.add_connection(connection{from.second.innov_number, from.second.from_node, from.second.to_node, from.second.weight});
                }
            }
        }
    }
    return child;
}

double genome::compare(const genome& other, const population_config& config) const {
    size_t excess = 0, disjoint = 0, weight_count = 0;
    double weight_sum = 0;
    size_t to_compare = (other.connections.size() > this -> connections.size()) ? other.connections.size() : this -> connections.size();

    for(size_t c = 0; c < to_compare; c++) {
        if(other.connections.size() <= c || this -> connections.size() <= c) {
            excess++;
            continue;
        }

        if(other.connections.at(c).first == -1 || this -> connections.at(c).first == -1) {
            disjoint++;
            continue;
        }

        size_t from = other.connections.at(c).first;
        size_t to = other.connections.at(c).second;

        weight_sum += fabs(other.network.at(to).at(from).weight - this -> network.at(to).at(from).weight);
        weight_count++;
    }
   
    size_t n = (to_compare >= 20) ? to_compare : 1;

    return config.c_disjoint * disjoint / n + config.c_excess * excess / n + weight_sum / weight_count * config.c_weight;
}

double genome::get_fitness() const {
    return this -> fitness;
}

void genome::set_fitness(double value) {
    this -> fitness = value;
}

void genome::reward(double value) {
    this -> fitness += value;
}

genome::~genome() {
}

void genome::mutate_weights(const population_config& config) {
    for(auto& node_out : this -> network) {
        for(auto& conn : node_out.second){
            if(this -> rand_range(0, 1) <= config.perturb_weight) {
                conn.second.weight += this -> rand_range(-1, 1) * config.steps;
            } else {
                conn.second.weight = this -> rand_range(-1, 1);
            }
        }
    }
}

void genome::mutate_node(innovations& innovation_history){
    auto beg_0 = this -> network.begin();
    std::advance(beg_0, (size_t)this -> rand_range(0, this -> network.size()));
    std::map<size_t, connection>& first_node = (*beg_0).second;

    auto beg_1 = first_node.begin();
    std::advance(beg_1, (size_t)this -> rand_range(0, first_node.size()));
    connection& to_split = (*beg_1).second;
    
    to_split.active = false;
    
    size_t layer = (this -> nodes.at(to_split.from_node).layer + this -> nodes.at(to_split.to_node).layer) / 2;

    size_t prev_nodes = 0;
    for(node_data& d : this -> nodes){
        if(d.layer < layer) {
            prev_nodes++;
        } else if(d.layer > layer){
            d.prev_nodes++;
        }
    }
    this -> add_node(layer, prev_nodes);
    this -> add_connection(innovation_history.get_innovation(this -> connections, to_split.from_node, this -> nodes.size() - 1), to_split.from_node, this -> nodes.size() - 1);
    this -> add_connection(innovation_history.get_innovation(this -> connections, this -> nodes.size() - 1, to_split.to_node), this -> nodes.size() - 1, to_split.to_node, to_split.weight);
}

void genome::mutate_connection(innovations& innovation_history) {
    if(!this -> is_fully_connected()) {
        size_t from_node, to_node;

        do {
            from_node = this -> rand_range(0, this -> nodes.size());
            to_node = this -> rand_range(0, this -> nodes.size());
        } while(this -> is_input(to_node) || this -> is_output(from_node) ||
                this -> does_connection_exist(from_node, to_node) || from_node == to_node ||
                this -> nodes.at(from_node).layer >= this -> nodes.at(to_node).layer);
        this -> add_connection(innovation_history.get_innovation(this -> connections, from_node, to_node), from_node, to_node, this -> rand_range(-1, 1));
    }
}

void genome::mutate_state(const bool state) {
    auto beg_0 = this -> network.begin();
    std::advance(beg_0, (size_t)this -> rand_range(0, this -> network.size()));
    std::map<size_t, connection>& first_node = (*beg_0).second;

    auto beg_1 = first_node.begin();
    std::advance(beg_1, (size_t)this -> rand_range(0, first_node.size()));
    (*beg_1).second.active = state;
}

double genome::rand_range(const double min, const double max) {
    return (double) rand() / ((double)RAND_MAX / (max - min)) + min; 
}