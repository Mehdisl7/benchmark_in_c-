#pragma once

#include <iostream>
#include <chrono>
#include <utility>
#include <string>

namespace mybench{

class Timer{
public:
	explicit Timer(std::string nom)
		: nom_(std::move(nom)),
		debut_(std::chrono::steady_clock::now()) {}

	~Timer() {
		const auto fin = std::chrono::steady_clock::now();
		const auto duree = std::chrono::duration_cast<std::chrono::microseconds>(fin - debut_).count();
		std::cout << nom_ << " : " << duree << " us\n";
	}
private:
	std::string nom_;
	std::chrono::steady_clock::time_point debut_;
};
}				

