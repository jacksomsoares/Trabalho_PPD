#include <iostream>
#include <sstream>
#include <omp.h>

using namespace std;

double s1,s2,s3,s4,s5;
double f1,f2,f3,f4,f5;

void sum(char* output, const long unsigned int d, const long unsigned int n) {
	long unsigned int digits[d + 11];
	s1 = omp_get_wtime();
	for (long unsigned int digit = 0; digit < d + 11; ++digit) {
		digits[digit] = 0;
	}
	f1 = omp_get_wtime();
	
	s2 = omp_get_wtime();
	#pragma omp parallel num_threads(12)
	{

	#pragma omp for reduction(+:digits[:d+11]) nowait
	for (long unsigned int i = 1; i <= n; ++i) {
		long unsigned int remainder = 1;
		for (long unsigned int digit = 0; digit < d + 11 && remainder; ++digit) {
			long unsigned int div = remainder / i;
			long unsigned int mod = remainder % i;
			digits[digit] += div;
			remainder = mod * 10;
		}
	}

	}
	f2 = omp_get_wtime();

	s3 = omp_get_wtime();
	for (long unsigned int i = d + 11 - 1; i > 0; --i) {
		digits[i - 1] += digits[i] / 10;
		digits[i] %= 10;
	}
	f3 = omp_get_wtime();
	if (digits[d + 1] >= 5) {
		++digits[d];
	}	

	s4 = omp_get_wtime();
	for (long unsigned int i = d; i > 0; --i) {
		digits[i - 1] += digits[i] / 10;
		digits[i] %= 10;
	}
	f4 = omp_get_wtime();

	stringstream stringstreamA;
	stringstreamA << digits[0] << ".";
	s5 = omp_get_wtime();
	for (long unsigned int i = 1; i <= d; ++i) {
		stringstreamA << digits[i];
	}
	f5 = omp_get_wtime();
	stringstreamA << '\0';
	string stringA = stringstreamA.str();
	stringA.copy(output, stringA.size());
}

int main() {

	long unsigned int d, n;

	cin >> d >> n;

	char output[d + 10]; // extra precision due to possible error

	sum(output, d, n);

	cout << output << endl;

	cout << "Tempo 1: " << f1 - s1 << endl;
	cout << "Tempo 2: " << f2 - s2 << endl;
	cout << "Tempo 3: " << f3 - s3 << endl;
	cout << "Tempo 4: " << f4 - s4 << endl;
	cout << "Tempo 5: " << f5 - s5 << endl;

	return 0;
}
