#pragma once
#include <iostream>
#include <chrono>
#include <string>

namespace mybench {

	//Fn = nimporte quoi, (une fonction lambda...).
	
	template <typename T>
	inline void DoNotOptimize(const T& valeur){
		asm volatile("" : : "r,m"(valeur) : "memory");
	}

	template <typename Fn>
	void bench(const std::string& nom, Fn fonction, int iterations = 1000, int warmup = 100){

		for(int i = 0; i < warmup; i++){
			fonction();
		}

		const auto debut = std::chrono::steady_clock::now();
		for(int i = 0; i < iterations; i++){
			fonction(); //c'est ici qu'on va executer le code afin de le mesurer.
				    
		}

		const auto fin = std::chrono::steady_clock::now();
		const auto total = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - debut).count();
		const double moyenne = static_cast<double>(total) / iterations;

		std::cout << nom << " : " << moyenne << "ns/iteration"
			<< " (" << iterations << " repetitions)\n";
	}

}

