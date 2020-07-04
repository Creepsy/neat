#pragma once

#include <vector>

#include "population_config.h"
#include "species.h"

class population {

private:
    size_t generation;
    population_config& config;

    score_func_t evaluate_function;
    activation_t activation;

    std::vector<species> specs;
    innovations innovation_history;
public:
    population(population_config& config, score_func_t evaluate_function, activation_t activation);
    double play();
    void add_to_species(const genome& g);
    void breed();
    ~population();
};


