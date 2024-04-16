#include <iostream>
#include <thread>
#include <map>
#include <string>
#include <chrono>
using namespace std::chrono_literals;

//void function1() {
//	for (int i = 0; i < 200; ++i)
//	{
//		std::cout << "+";
//	}
//}
//
//void function2() {
//	for (int i = 0; i < 200; ++i)
//	{
//		std::cout << "-";
//	}
//}

void refreshForecast(std::map<std::string, int> forecastMap) {
	while (true) {
		for (auto &item : forecastMap) {
			item.second++;
			std::cout << item.first << " - " << item.second << std::endl;
		}
		std::this_thread::sleep_for(2000ms);
	}
}

int main() {
	//std::thread worker1(function1);
	//std::thread worker2(function2);

	std::map<std::string, int> forecastMap = {
		{"new york", 15},
		{"mumbai", 20},
		{"berlin", 18}
	};

	std::thread bgworker(refreshForecast, forecastMap);

	system("pause>nul");

	return 0;
}