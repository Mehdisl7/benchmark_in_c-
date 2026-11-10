#pragma once
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

namespace mybench {

	//Fn = nimporte quoi, (une fonction lambda...).
	
template <typename T>
inline void DoNotOptimize(const T& valeur){
	asm volatile("" : : "r,m"(valeur) : "memory");
}

template <typename Fn>
void bench(const std::string& nom, Fn fonction, int samples = 50, double cible_ns = 1'000'000.0, int warmup = 100){

	for(int i = 0; i < warmup; i++){
		fonction();
	}

	int iterations = 1;
	while(true){
		const auto debut = std::chrono::steady_clock::now();
		for(int i = 0; i < iterations; ++i){
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
		for(int i = 0; i < iterations; i++){
			fonction(); //c'est ici qu'on va executer le code afin de le mesurer.	    
		}

		const auto fin = std::chrono::steady_clock::now();
		const auto duree=
			std::chrono::duration_cast<std::chrono::nanoseconds>(fin - debut).count();
		temps.push_back(duree / iterations);

	}

	std::sort(temps.begin(), temps.end());
	const size_t n = temps.size();

	const double min = temps.front();
	const double max = temps.back();

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

	std::cout << nom << " :\n"
		<< "min : " << min << " ns\n"
		<< "median " << mediane << " ns\n"
		<< "moy : " << moyenne << " ns\n "
		<< "max : " << max << " ns\n"
		<< "stddev " << ecart_type << " ns\n"
		<< "   (" << samples << " samples x " << iterations << " iterations auto)\n";

}

}



