#include "species.h"

#include <algorithm>

species::species(const genome& g) : fitness(0), champ(g) {
    this -> members.push_back(g);
}

double species::play(score_func_t evaluate_function) {
    double highest = INT32_MIN;
    for(genome& g : this -> members) {
        evaluate_function(g);
        if(g.get_fitness() > highest) {
            highest = g.get_fitness();
        }
    }

    return highest;
}

bool species::add(const genome& g, population_config& config) {
    if(g.compare(this -> champ, config) <= config.compatibility_threshold) {
        this -> members.push_back(g);
        return true;
    }
    return false;
}

double species::recalculate_fitness() {
    this -> fitness = 0;

    for(genome& g : this -> members) {
        g.set_fitness(g.get_fitness() / this -> members.size());
        this -> fitness += g.get_fitness();
    }

    this -> fitness /= this -> members.size();

    return this -> fitness;
}

double species::get_fitness() {
    return this -> fitness;
}

void species::prepare_breeding() {
    std::sort(this -> members.begin(), this -> members.end(), [](genome& first, genome& second) -> bool {return first.get_fitness() > second.get_fitness();});

    size_t to_kill = (size_t)(this -> members.size() * 0.5);

    for(int k = 0; k < to_kill; k++) {
        this -> members.pop_back();
    }
}

size_t species::get_size() {
    return this -> members.size();
}

void species::reset() {
    if(this -> get_size() > 0) {
        this -> champ = this -> members.at(0);
    }
    this -> members.clear();
}

genome species::breed(population_config& config, innovations& innovation_history) {
    if(genome::rand_range(0, 1) <= config.crossover_chance) {
        genome& parent0 = this -> members.at((size_t)genome::rand_range(0, this -> members.size()));
        genome& parent1 = this -> members.at((size_t)genome::rand_range(0, this -> members.size()));

        if(parent0.get_fitness() > parent1.get_fitness()) {
            return parent0.breed(parent1).mutate(config, innovation_history);
        } else {
            return parent1.breed(parent0).mutate(config, innovation_history);
        }
    } else {
        genome child = this -> members.at((size_t)genome::rand_range(0, this -> members.size()));
        return child.mutate(config, innovation_history);
    }
}

species::~species()
{
}