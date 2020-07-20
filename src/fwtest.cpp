#include <cmath>
#include "lib/genome.h"

double sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}

int main(int argc, char const *argv[])
{
    genome g(0, 0, sigmoid);
    g.input_count = 3;
    g.add_node(0, 0);
    g.add_node(0, 0);
    g.add_node(0, 0);
    g.add_node(1, 3);
    g.add_node(2, 4);
    g.add_connection(0, 0, 3, 10.37, true);
    g.add_connection(0, 1, 3, -11.77, true);
    g.add_connection(0, 2, 3, 6.68, true);
    g.add_connection(0, 3, 4, -12.33, true);
    g.add_connection(0, 0, 4, 6.14, true);
    g.add_connection(0, 1, 4, -6.09, true);
    g.add_connection(0, 2, 4, 9.20, true);
    std::cout << g << std::endl
              << std::endl;
    std::cout << "0 xor 0: " << g.evaluate_node(4, std::vector<double>{0, 0, 1}) << std::endl;
    std::cout << "1 xor 0: " << g.evaluate_node(4, std::vector<double>{1, 0, 1}) << std::endl;
    std::cout << "0 xor 1: " << g.evaluate_node(4, std::vector<double>{0, 1, 1}) << std::endl;
    std::cout << "1 xor 1: " << g.evaluate_node(4, std::vector<double>{1, 1, 1}) << std::endl;

    return 0;
}
