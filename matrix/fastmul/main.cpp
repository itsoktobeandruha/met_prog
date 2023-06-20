#include "Matrix.h"
#include <chrono>
#include <time.h>

int main()
{
	Matrix a(2048,2048);
	Matrix b(2048,2048);
	clock_t start, end;
	start = clock();
	fast_mul(a, b);
	end = clock();
	std::cout << (end - start) / 1000 << std::endl;
	return 0;
}