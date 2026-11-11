#pragma once
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

namespace mybench {

	//Fn = nimporte quoi, (une fonction lambda...).
	
template <typename T>
inline void DoNotOptimize(const T& valeur){
	asm volatile("" : : "r,m"(valeur) : "memory");
}

struct Result {

	std::string nom;
	double min, mediane, mean, max, stddev, ops_par_sec;
	int samples;
	long iterations;
};


template <typename Fn>
Result bench(const std::string& nom, Fn fonction, int samples = 50, double cible_ns = 1'000'000.0, int warmup = 100){

	for(int i = 0; i < warmup; i++){
		fonction();
	}

	long iterations = 1;
	while(true){
		const auto debut = std::chrono::steady_clock::now();
		for(long i = 0; i < iterations; ++i){
			fonction();
		}


		const auto fin = std::chrono::steady_clock::now();
		const double duree = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - debut).count();

		if(duree >= cible_ns || iterations > 1'000'000){
			break;
		}
		iterations*=2;
	}


	std::vector<double> temps;
	temps.reserve(samples);
	for(int s = 0; s < samples; ++s){
		const auto debut = std::chrono::steady_clock::now();
		for(long i = 0; i < iterations; i++){
			fonction(); //c'est ici qu'on va executer le code afin de le mesurer.	    
		}

		const auto fin = std::chrono::steady_clock::now();
		const auto duree=
			std::chrono::duration_cast<std::chrono::nanoseconds>(fin - debut).count();
		temps.push_back(duree / iterations);
	}

	std::sort(temps.begin(), temps.end());
	const size_t n = temps.size();
	double somme = 0.0;
	for(double t : temps){
		somme+=t;
	}

	const double moyenne = somme / n;

	double mediane;
	if(n%2==0){
		mediane = (temps[n/2 - 1] + temps[n/2]) / 2.0;
	}else{
		mediane = temps[n/2];
	}

	double somme_carres = 0.0;
	for(double t : temps){
		const double ecart = t - moyenne;
		somme_carres += ecart * ecart;
	}

	const double ecart_type = std::sqrt(somme_carres / n);

	Result r;
	r.nom = nom;
	r.min = temps.front();
	r.mediane = mediane;
	r.mean = moyenne;
	r.max = temps.back();
	r.stddev = ecart_type;
	r.ops_par_sec = 1'000'000'000.0 / mediane;
	r.samples = samples;
	r.iterations = iterations;
	return r;
}

inline void print_table(const std::vector<Result>& results){
	std::cout << std::left << std::setw(24) << "name"
		<< std::right
		<< std::setw(12) << "min(ns)"
		<< std::setw(12) << "mediane(ns)"
		<< std::setw(12) << "mean(ns)"
		<< std::setw(12) << "max(ns)"
		<< std::setw(12) << "stddev"
		<< std::setw(18) << "ops/s"
		<< "\n";
	std::cout << std::string(24 + 12 * 5 + 18, '-') << "\n";
	std::cout << std::fixed << std::setprecision(2);
	for(const Result& r : results){
		std::cout << std::left << std::setw(24) << r.nom
                  << std::right
                  << std::setw(12) << r.min
                  << std::setw(12) << r.mediane
                  << std::setw(12) << r.mean
                  << std::setw(12) << r.max
                  << std::setw(12) << r.stddev
                  << std::setw(18) << r.ops_par_sec
                  << "\n";
	}
}
}



