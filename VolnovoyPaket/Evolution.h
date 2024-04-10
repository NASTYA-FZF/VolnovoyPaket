#pragma once
#include <vector>
#include <complex>

typedef std::complex<double> compl;

class Evolution
{
public:
	const int num_psi = 1024;
private:
	int points;
	double step_po, tau;
	std::vector<double> po;
	
public:
	std::vector<std::vector<compl>> for_fft;
	std::vector<std::vector<compl>> my_psi = std::vector<std::vector<compl>>(num_psi);

	Evolution() { }

	Evolution(double a, double sigma, double step_t, double R0, int num_point, double left);

	Evolution operator=(const Evolution& right);

	std::vector<double> GetPo();

	std::vector<compl> step_evolution(int num_evolution);

	void furie_my_psy();
};

//¡œ‘
void fft(std::vector<compl>& a, bool invert);