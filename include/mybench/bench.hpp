#pragma once
#include <iostream>
#include <chrono>
#include <string>
#include <vector>

namespace mybench {

	//Fn = nimporte quoi, (une fonction lambda...).
	
template <typename T>
inline void DoNotOptimize(const T& valeur){
	asm volatile("" : : "r,m"(valeur) : "memory");
}

template <typename Fn>
void bench(const std::string& nom, Fn fonction, int samples = 50, int iterations = 1000, int warmup = 100){

	for(int i = 0; i < warmup; i++){
		fonction();
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
			std::chrono::duration_cast<std::chrono::nanoseconds>(fin -debut).count();
		temps.push_back(static_cast<double>(duree) / iterations);

	}

	double min = temps[0];
	double max = temps[0];
	double somme = 0.0;
	for(double t : temps){
		if(t < min)
			min = t;
		if(t>max)
			max = t;
		somme+=t;
	}

	const double moyenne = somme / temps.size();

	std::cout << nom << " : "
		<< "min : " << min << " ns, "
		<< "moy : " << moyenne << " ns, "
		<< "max : " << max << " ns, "
		<< " (" << samples << " echantillons X " << iterations << " iterations)\n";
}

}



