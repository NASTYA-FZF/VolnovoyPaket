#include "pch.h"
#define _USE_MATH_DEFINES
#include "Evolution.h"
#include <cmath>
using namespace std;

Evolution::Evolution(double a, double sigma, double step_t, double R0, int num_point, double left)
{
	points = num_point;
	
	tau = step_t;
	vector<compl> psi0(points + 1);

	psi0[0] = psi0[points] = 0.;
	//psi0[points] = 0.;
	

	double left_po = left;
	double norma = 0., norma2 = 0.;
	step_po = (R0 - left_po) / points;
	po.push_back(left_po);

	for (int i = 1; i < points; i++)
	{
		po.push_back(left_po + i * step_po);
		psi0[i] = compl(exp(-(po[i] - a) * (po[i] - a) / (4 * sigma * sigma)), 0.);
		norma += psi0[i].real();
	}
	po.push_back(left_po + points * step_po);
	for (int i = 1; i < points; i++)
	{
		psi0[i] = compl(exp(-(po[i] - a) * (po[i] - a) / (4 * sigma * sigma)) / norma, 0.);
		norma2 += psi0[i].real();
	}
	my_psi[0] = psi0;
}

Evolution Evolution::operator=(const Evolution& right)
{
	points = right.points;
	step_po = right.step_po;
	tau = right.tau;
	my_psi = right.my_psi;
	po = right.po;
	return *this;
}

std::vector<double> Evolution::GetPo()
{
	return po;
}

std::vector<compl> Evolution::step_evolution(int num_evolution)
{
	vector<compl> cur_psi(points + 1);

	//tau /= 6.5e-16;
	//step_po /= 1e-18;

	//for (int i = 0; i < po.size(); i++) po[i] /= 1e-18;
	
	compl A, B, D;
	
	//compl C(2., 2. * tau / (step_po * step_po));
	compl C(8., 2. * tau / (step_po * step_po));

	vector<compl> last_psi = my_psi[num_evolution - 1];

	vector<compl> alpha(points - 1), betta(points - 1);
	alpha[0] = betta[0] = compl(0., 0.);
	
	for (int i = 1; i < points - 1; i++)
	{
		A = compl(0., tau * (1 / (2 * po[i]) - 1 / step_po) / step_po);
		B = compl(0., -tau * (1 / (2 * po[i]) + 1 / step_po) / step_po);

		//A = B = compl(0., -tau * (1 / (2 * po[i]) + 1 / step_po) / step_po);

		//A = B = compl(0., -tau * (1 / (2 * po[i]) - 1 / step_po) / step_po);

		//B = A = compl(0., tau * (1 / (2 * po[i]) - 1 / step_po) / step_po);

		//D = (4. - C) * last_psi[i] - A * last_psi[i - 1] - B * last_psi[i + 1];
		D = (16. - C) * last_psi[i] - A * last_psi[i - 1] - B * last_psi[i + 1];

		/*alpha[i] = -B / (C + A * alpha[i - 1]);
		betta[i] = (D - A * betta[i - 1]) / (C + A * alpha[i - 1]);*/

		alpha[i] = -A / (C + B * alpha[i - 1]);
		betta[i] = (D - B * betta[i - 1]) / (C + B * alpha[i - 1]);
	}

	cur_psi[points] = compl(0., 0.);
	for (int i = points - 1; i > 0; i--)
	{
		cur_psi[i] = alpha[i - 1] * cur_psi[i + 1] + betta[i - 1];
	}
	my_psi[num_evolution] = cur_psi;

	return cur_psi;
}

void Evolution::furie_my_psy()
{
	for_fft = vector<vector<compl>>(points + 1);
	for (int j = 0; j < points + 1; j++)
	{
		for (int i = 0; i < num_psi; i++)
		{
			for_fft[j].push_back(my_psi[i][j]);
		}
	}

	for (int i = 0; i < points + 1; i++) fft(for_fft[i], true);
}

void fft(std::vector<compl>& a, bool invert)
{
	int n = (int)a.size();
	if (n == 1)  return;

	vector<compl> a0(n / 2), a1(n / 2);
	for (int i = 0, j = 0; i < n; i += 2, ++j) {
		a0[j] = a[i];
		a1[j] = a[i + 1];
	}
	fft(a0, invert);
	fft(a1, invert);

	double ang = 2 * M_PI / n * (invert ? -1 : 1);
	compl w(1), wn(cos(ang), sin(ang));
	for (int i = 0; i < n / 2; ++i) {
		a[i] = a0[i] + w * a1[i];
		a[i + n / 2] = a0[i] - w * a1[i];
		if (invert)
			a[i] /= 2, a[i + n / 2] /= 2;
		w *= wn;
	}
}
