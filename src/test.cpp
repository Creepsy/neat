#include <iostream>
#include <random>
#include <math.h>

#include "lib/population.h"
#include "lib/innovations.h"

double sigmoid(double x);
void xor_evaluation(genome& g);
double ReLU(double x);
void max_evaluation(genome& g);

//improve innov numbers?!? IMPORTANT

int main() {
      srand(time(nullptr));
  //  srand(10);

    population_config config = population_config{150, 2, 1};
   

   /* genome a = genome{3, 2, sigmoid};
    genome b = genome{3, 2, sigmoid};
    a.mutate(config);
    b.mutate(config);

    std::cout << "A: " <<  a << std::endl;
    std::cout << "B: " <<  b << std::endl;

    std::cout << "=====" << std::endl;
    
    genome c = a.breed(b);

    std::cout << c << std::endl;
    std::cout << "A<->B: " << a.compare(b, config) << std::endl;
    std::cout << "B<->A: " << b.compare(a, config) << std::endl;
    std::cout << "A<->C: " << a.compare(c, config) << std::endl;
    std::cout << "B<->C: " << b.compare(c, config) << std::endl;

    species s = species{a};
    std::cout << "B to Species-A: " << s.add(b, config) << std::endl;
    std::cout << "A to Species-A: " << s.add(a, config) << std::endl;
    std::cout << "C to Species-A: " << s.add(c, config) << std::endl;*/
/*
    population p = population{config, xor_evaluation, sigmoid};
    for(int i = 0; i < 3000; i++) {
        std::cout << "Highest: " << p.play() << std::endl;
        p.breed();   
    }*/

    /*for(double val : a.run({1, 2, 3})) {
        std::cout << val << std::endl;
    }
    std::cout << "=====" << std::endl;
    for(double val : b.run({1, 2, 3})) {
        std::cout << val << std::endl;
    }*/
    population p = population{config, xor_evaluation, ReLU};
    for(int i = 0; i < 1000; i++) {
        std::cout << "Highest: " << p.play() << std::endl;
        p.breed();   
    }
}

void xor_evaluation(genome& g) {
    g.set_fitness(0);
    double fitness = 0;
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 2; j++) {
            double result = g.run({(double)i, (double)j}).at(0);
            double expected = (i != j) ? 1 : 0;
            std::cout << i << ", " << j << " -> " << result << std::endl;
            fitness += 1 - fabs(expected - result);
        }    
    }

    g.set_fitness(fitness);
}

void max_evaluation(genome& g) {
    g.set_fitness(g.run({1}).at(0));
}

double ReLU(double x) {
    return (x > 0) ? x : 0;
}

double sigmoid(double x) {
    return 1 / (1 + pow(M_E, x));
}