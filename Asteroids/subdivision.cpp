#include "geometry.h"

double ALPHA[] = {
	1.13333, -0.358974, -0.333333, 0.129032, 0.945783,
	2.0, 3.19889, 4.47885, 5.79946, 7.13634,
	8.47535, 9.80865, 11.1322, 12.4441, 13.7439,
	15.0317, 16.3082, 17.574, 18.83, 20.0769
};

double BETA[] = {
	0.46875, 1.21875, 1.125, 0.96875, 0.840932, 0.75, 0.686349,
	0.641085, 0.60813, 0.583555, 0.564816, 0.55024, 0.5387,
	0.529419, 0.52185, 0.515601, 0.510385, 0.505987, 0.502247, 0.49904
};

double alpha(GLuint n) {
	if (n <= 20) return ALPHA[n-1];
	
	double b = beta(n);
	
	return n * (1 - b) / b; 
}

double beta(GLuint n) {
	if (n <= 20) return BETA[n-1];
	return (5.0/4.0 - (3+2*cos(2*M_PI / n)) * (3+2*cos(2*M_PI / n)) / 32);
}