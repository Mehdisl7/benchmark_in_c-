#include <thebench/timer.hpp>

int main(){

	long somme = 0;
	{
		thebench::Timer t("somme d'un milion de caractere");	
		for(long i = 0; i <= 1'000'000; ++i)
			somme+=i;
	}

	std::cout << "resultat = " << somme << std::endl;
}


