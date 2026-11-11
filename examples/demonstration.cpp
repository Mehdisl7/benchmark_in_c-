#include <mybench/bench.hpp>

#include <vector>

int main(int argc, char**) {
    std::vector<mybench::Result> results;

    results.push_back(mybench::bench("SANS protection", [argc] {
        unsigned long x = argc;
        for (int i = 0; i < 100000; ++i) {
            x = x * 1103515245u + 12345u;
        }
    }));

    results.push_back(mybench::bench("AVEC protection", [argc] {
        unsigned long x = argc;
        for (int i = 0; i < 100000; ++i) {
            x = x * 1103515245u + 12345u;
        }
        mybench::DoNotOptimize(x);
    }));

    mybench::print_table(results);
    mybench::csv(results, "results.csv");
    return 0;
}
