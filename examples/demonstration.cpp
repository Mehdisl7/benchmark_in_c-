#include <mybench/bench.hpp>

int main(int argc, char**) {
    // argc vaut 1 au lancement, mais le compilateur l'ignore à la compilation :
    // il ne peut donc pas précalculer le résultat à notre place.

    mybench::bench("SANS protection", [argc] {
        unsigned long x = argc;
        for (int i = 0; i < 100000; ++i) {
            x = x * 1103515245u + 12345u;  // chaque valeur dépend de la précédente
        }
        // x n'est jamais utilisée -> le compilateur a le droit de TOUT supprimer
    });

    mybench::bench("AVEC protection", [argc] {
        unsigned long x = argc;
        for (int i = 0; i < 100000; ++i) {
            x = x * 1103515245u + 12345u;
        }
        mybench::DoNotOptimize(x);  // on force le compilateur à garder x
    });

    return 0;
}
