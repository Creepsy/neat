#pragma once

#include <vector>

#include "genome.h"
#include "population_config.h"

typedef void(*score_func_t)(genome&);

class species {

private:
    std::vector<genome> members;
    double fitness;
    genome champ;
public:
    species(const genome& g);
    genome play(score_func_t evaluate_function);
    const genome& get_champ() const;
    bool add(const genome& g, population_config& config);
    double recalculate_fitness();
    double get_fitness();
    void prepare_breeding();
    size_t get_size();
    void reset();
    genome breed(population_config& config, innovations& innovation_history);
    ~species();
};
