#pragma once
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace mybench {

// Si l'entree est un fichier source (.c/.cpp), on le compile et on renvoie le chemin
inline std::string preparer_cible(const std::string& entree, bool& a_compile) {
    a_compile = false;

    std::string compilateur;
    if (entree.ends_with(".c")) {
        compilateur = "gcc";
    } else if (entree.ends_with(".cpp") || entree.ends_with(".cc")) {
        compilateur = "g++";
    } else {
        return entree;
    }

    const std::string binaire = "/tmp/mybench_tmp_bin";
    const std::string cmd_compil = compilateur + " -O2 " + entree + " -o " + binaire;

    std::cout << "Compilation : " << cmd_compil << "\n";
    const int code = std::system(cmd_compil.c_str());
    if (code != 0) {
        std::cerr << "Erreur : la compilation de " << entree << " a echoue.\n";
        return "";
    }

    a_compile = true;
    return binaire;
}

inline void bench_command(const std::string& entree, int runs = 20) {
    bool a_compile = false;
    const std::string cible = preparer_cible(entree, a_compile);
    if (cible.empty()) {
        return;
    }

    std::cout << "Benchmark : " << cible << "  (" << runs << " runs)\n";

    std::vector<double> temps_ms;
    temps_ms.reserve(runs);
    const std::string cmd_run = cible + " > /dev/null 2>&1";
    for (int i = 0; i < runs; ++i) {
        const auto debut = std::chrono::steady_clock::now();
        const int code = std::system(cmd_run.c_str());
        const auto fin = std::chrono::steady_clock::now();

        if (code != 0) {
            std::cerr << "Attention : la commande a renvoye un code non nul (" << code << ")\n";
        }

        const double ms =
            std::chrono::duration_cast<std::chrono::microseconds>(fin - debut).count() / 1000.0;
        temps_ms.push_back(ms);
    }

    std::sort(temps_ms.begin(), temps_ms.end());
    const size_t n = temps_ms.size();

    const double min = temps_ms.front();
    const double max = temps_ms.back();

    double somme = 0.0;
    for (double t : temps_ms) {
        somme += t;
    }
    const double moyenne = somme / n;

    double mediane;
    if (n % 2 == 0) {
        mediane = (temps_ms[n / 2 - 1] + temps_ms[n / 2]) / 2.0;
    } else {
        mediane = temps_ms[n / 2];
    }

    double somme_carres = 0.0;
    for (double t : temps_ms) {
        const double ecart = t - moyenne;
        somme_carres += ecart * ecart;
    }
    const double ecart_type = std::sqrt(somme_carres / n);

    std::cout << std::fixed << std::setprecision(3)
              << "   min    " << min << " ms\n"
              << "   median " << mediane << " ms\n"
              << "   mean   " << moyenne << " ms\n"
              << "   max    " << max << " ms\n"
              << "   stddev " << ecart_type << " ms\n";

    if (a_compile) {
        std::remove(cible.c_str());
    }
}

}
