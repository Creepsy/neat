#include "population.h"

#include <algorithm>

population::population(population_config& config, score_func_t evaluate_function, activation_t activation) : 
    config(config), evaluate_function(evaluate_function), generation(0), activation(activation), innovation_history(config.inputs + config.outputs) {
        for(size_t g = 0; g < config.population_size; g++){
            //this -> add_to_species(genome{config.inputs, config.outputs, activation}.mutate(config));
            this -> add_to_species(genome{config.inputs, config.outputs, activation});
        }
}

genome population::play() {
    genome best = genome{0, 0, nullptr};
    best.set_fitness(INT32_MIN);
    for(species& s : this -> specs) {
        genome s_highest = s.play(this -> evaluate_function);
        if(s_highest.get_fitness() > best.get_fitness()) {
            best = s_highest;
        }
    }

    return best;
}

void population::add_to_species(const genome& g) {
    for(species& s : this -> specs) {
        if(s.add(g, this -> config)) return;
    }
    this -> specs.push_back(species{g});
}

void population::breed() {
    double total = 0;
    for(int i = 0; i < this -> specs.size(); i++) {
        species& s = this -> specs.at(i);

        if(s.get_size() == 0){
            this -> specs.erase(this -> specs.begin() + i);
            i--;
            continue;
        }

        s.prepare_breeding();
        total += s.recalculate_fitness();
    }

    std::sort(this -> specs.begin(), this -> specs.end(), [](species& first, species& second) -> bool {return first.get_fitness() > second.get_fitness();});

    std::vector<genome> children;
    for(int i = 0; i < this -> specs.size(); i++) {
        species& s = this -> specs.at(i);
        double percentage = s.get_fitness() / total;
        size_t child_count = (size_t)(percentage * this -> config.population_size);

       // std::cout << s.get_fitness() << " -> " << child_count << std::endl;

        if(child_count == 0 || percentage <= 0) {
            total -= s.get_fitness();
            this -> specs.erase(this -> specs.begin() + i);
            i--;
            continue;
        }

        if(child_count > this -> config.population_size) {
            child_count = this -> config.population_size;
        }

        if(children.size() + child_count > this -> config.population_size){
            child_count = this -> config.population_size - children.size();
        }

        if(child_count > 0) {
            children.push_back(s.get_champ());
            child_count--;
        }

        for(int c = 0; c < child_count; c++) {
            children.push_back(s.breed(this -> config, this -> innovation_history));
        }        
    }

    size_t to_produce = this -> config.population_size - children.size();
    for(int i = this -> specs.size() - 1; i >= 0; i--) {
        species& s = this -> specs.at(i);
        if(to_produce <= 0) break;
        to_produce--;
        children.push_back(s.breed(this -> config, this -> innovation_history));
    }

    for(species& s : this -> specs) {
        s.reset();
    }

  /*  std::cout << "=====" << std::endl;
    std::cout << "Children : " << children.size() << std::endl;*/
    std::cout << "Species : " << this -> specs.size() << std::endl;

    for(genome& g : children) {
        this -> add_to_species(g);
    }

    /*for(species& s : this -> specs) {
        std::cout << s.get_size() << std::endl;
    }*/
 //   std::cout << "=====" << std::endl;

    this -> generation++;
    this -> innovation_history.clear();
}

population::~population() {
}