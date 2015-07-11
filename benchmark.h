#ifndef BENCHMARK_H
#define BENCHMARK_H 

#include <ctime>
#include <string>

#include "ui.h"

#define INIT_BENCHMARK std::clock_t begin = std::clock();
#define MEASURE_AND_REPORT(title) std::clock_t end = std::clock();\
	Window::printMessage(title, "Latency: " + std::to_string(double(end - begin) / CLOCKS_PER_SEC));\
	UI::getchar();

#endif /* BENCHMARK_H */
