#include <mybench/command.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage : " << argv[0] << " \"<commande>\" [runs]\n";
        std::cerr << "Exemple : " << argv[0] << " \"sleep 0.05\"\n";
        return 1;
    }

    const std::string commande = argv[1];
    const int runs = (argc >= 3) ? std::atoi(argv[2]) : 20;

    mybench::bench_command(commande, runs);
    return 0;
}
