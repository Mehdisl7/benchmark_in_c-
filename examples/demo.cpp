#include <mybench/bench.hpp>

#include <numeric>
#include <iostream>
#include <vector>

int main(){

	long resultat = 0;

	mybench::bench("somme de 10K entiers", [&] {
		std::vector<int> v(10000);
		std::iota(v.begin(), v.end(), 0);
		resultat+=std::accumulate(v.begin(), v.end(), 0L);
	});

	std::cout << "resultat final" << resultat << "\n";

	return 0;
}




