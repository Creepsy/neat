#pragma once

#include <stddef.h>

struct population_config
{
    const size_t population_size;
    const size_t inputs;
    const size_t outputs;
    
  //  size_t staleness = 10;
    double compatibility_threshold = 3;

    double steps = 0.01;
    double perturb_weight = 0.9;

    double mutate_connection = 0.05;
    double mutate_node = 0.03;
    double mutate_weights = 0.8;
    double mutate_disable = 0.1;
    double mutate_enable = 0.2;

    double c_excess = 1;
    double c_disjoint = 1;
    double c_weight = 0.4;

    double crossover_chance = 0.75;
};
