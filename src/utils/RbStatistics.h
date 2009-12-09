/*!
* \file
* This file contains commonly used statistics functions that are used
* in REvBayes. The probability density (pdf), log of the probability
* density (lnPdf), cumulative probability (cdf), and quantiles of
* common probability distributions.
*
* \brief Namespace containing statistical functions
*
* (c) Copyright 2009- under GPL version 3
* \date Last modified: $Date$
 * \author Sebastian Hoehna (Department of Mathematics, Stockholm University)
 * \author John Huelsenbeck (Department of Integrative Biology, University of California, Berkeley)
 * \author Fredrik Ronquist (Department of Entomology, Swedish Museum of Natural History)
* \license GPL version 3
*
* $Id$
*/
#ifndef RbStatistics_H
#define RbStatistics_H

#include <vector>

class RbUnrootedTopology;
class RandomNumberGenerator;

namespace RbStatistics {

#pragma mark Beta Distribution

	namespace Beta {
	
	double pdf(double a, double b, double x);                                  /*!< Beta(a,b) probability density */
	double lnPdf(double a, double b, double x);                                /*!< Beta(a,b) log_e probability density */
	double cdf(double a, double b, double x);                                  /*!< Beta(a,b) cumulative probability */
	double quantile(double a, double b, double p);                             /*!< Beta(a,b) quantile */
	double rv(double a, double b, RandomNumberGenerator* rng);                 /*!< Beta(a,b) random variable */
	}
	
#pragma mark Chi-Square Distribution

	namespace ChiSquare {
	
	double pdf(double v, double x);                                            /*!< Chi-Square(v) probability density */
	double lnPdf(double v, double x);                                          /*!< Chi-Square(v) log_e probability density */
	double cdf(double v, double x);                                            /*!< Chi-Square(v) cumulative probability */
	double quantile(double v, double p);                                       /*!< Chi-Square(v) quantile */
	double rv(double v, RandomNumberGenerator* rng);                           /*!< Chi-Square(v) random variable */
	}

#pragma mark Dirichlet Distribution

	namespace Dirichlet {
	double pdf(const std::vector<double>& a, const std::vector<double>& z);    /*!< Dirichlet(a[]) probability density */
	double lnPdf(const std::vector<double>& a, const std::vector<double>& z);  /*!< Dirichlet(a[]) log_e probability density */
	double rv(const std::vector<double>& a, const std::vector<double>& z, 
	          RandomNumberGenerator* rng);                                     /*!< Dirichlet(a[]) log_e random variable */
	}

#pragma mark Exponential Distribution

	namespace Exponential {
	
	double pdf(double lambda, double x);                                       /*!< Exponential(lambda) probability density */
	double lnPdf(double lambda, double x);                                     /*!< Exponential(lambda) log_e probability density */
	double cdf(double lambda, double x);                                       /*!< Exponential(lambda) cumulative probability */
	double quantile(double lambda, double p);                                  /*!< Exponential(lambda) quantile */
	double rv(double lambda, RandomNumberGenerator* rng);                      /*!< Exponential(lambda) random variable */
	}

#pragma mark Gamma Distribution

	namespace Gamma {
	
	double pdf(double a, double b, double x);                                  /*!< Gamma(a,b) probability density */
	double lnPdf(double a, double b, double x);                                /*!< Log of the Gamma(a,b) probability density */  
	double cdf(double a, double b, double x);                                  /*!< Gamma(a,b) cumulative probability */
	double quantile(double a, double b, double p);                             /*!< Gamma(a,b) quantile */
	double rv(double a, double b, RandomNumberGenerator* rng);                 /*!< Gamma(a,b) random variable */
	}

#pragma mark LogNormal Distribution

	namespace LogNormal {
	
	double pdf(double mu, double sigma, double x);                             /*!< LogNormal(mu,sigma) probability density */
	double lnPdf(double mu, double sigma, double x);                           /*!< Log of the LogNormal(mu,sigma) probability density */
	double cdf(double mu, double sigma, double x);                             /*!< LogNormal(mu,sigma) cumulative probability */
	double quantile(double mu, double sigma, double p);                        /*!< LogNormal(mu,sigma) quantile */
	double rv(double mu, double sigma, RandomNumberGenerator* rng);            /*!< LogNormal(mu,sigma) random variable */
	}

#pragma mark Poisson Distribution

	namespace Poisson {
	
	double pdf(double lambda, int x);                                          /*!< Poisson(lambda) probability */
	double lnPdf(double lambda, int x);                                        /*!< Log of the Poisson(lambda) probability */
	double cdf(double lambda, int x);                                          /*!< Poisson(lambda) cumulative probability */
	double quantile(double lambda, double p);                                  /*!< Poisson(lambda) quantile */
	double rv(double lambda, RandomNumberGenerator* rng);                      /*!< Poisson(lambda) random variable */
	}

#pragma mark Normal Distribution

	namespace Normal {
	
	double pdf(double mu, double sigma, double x);                             /*!< Normal(mu,sigma) probability density */
	double lnPdf(double mu, double sigma, double x);                           /*!< Log of the Normal(mu,sigma) probability density */
	double cdf(double mu, double sigma, double x);                             /*!< Normal(mu,sigma) cumulative probability */
	double quantile(double p);                                                 /*!< Normal(0,1) quantile */
	double quantile(double mu, double sigma, double p);                        /*!< Normal(mu,sigma) quantile */
	double rv(double mu, double sigma, RandomNumberGenerator* rng);            /*!< Normal(mu,sigma) random variable */
	}

#pragma mark Uniform(0,1) Distribution

	namespace Uniform01 {
	
	double pdf(double x);                                                      /*!< Uniform(0,1) probability density */
	double lnPdf(double x);                                                    /*!< Log of the Uniform(0,1) probability density */
	double cdf(double x);                                                      /*!< Uniform(0,1) cumulative probability */
	inline double quantile(double p);                                          /*!< Uniform(0,1) quantile */
	double rv(RandomNumberGenerator* rng);                                     /*!< Uniform(0,1) random variable */
	}
	
#pragma mark Uniform(a,b) Distribution

	namespace Uniform {
	
	double pdf(double a, double b, double x);                                  /*!< Uniform(a,b) probability density */
	double lnPdf(double a, double b, double x);                                /*!< Log of the Uniform(a,b) probability density */
	double cdf(double a, double b, double x);                                  /*!< Uniform(a,b) cumulative probability */
	double quantile(double a, double b, double p);                             /*!< Uniform(a,b) quantile */
	double rv(double a, double b, RandomNumberGenerator* rng);                 /*!< Uniform(a,b) random variable */
	}
	
#pragma mark UniformUnrootedTopology

	namespace UniformUnrootedTopology {
	
	double pdf(int n, RbUnrootedTopology* x);                                  /*!< Uniform(a,b) probability density */
	double lnPdf(int n, RbUnrootedTopology* x);                                /*!< Log of the Uniform(a,b) probability density */
	RbUnrootedTopology* rv(int n, RandomNumberGenerator* rng);
	}

#pragma mark Helper Functions

	namespace Helper {
	
	double rndGamma(double s, RandomNumberGenerator& rng);
	double rndGamma1(double s, RandomNumberGenerator& rng);
	double rndGamma2(double s, RandomNumberGenerator& rng);
	int    poissonLow(double lambda, RandomNumberGenerator& rng);
	int    poissonInver(double lambda, RandomNumberGenerator& rng);
	int    poissonRatioUniforms(double lambda, RandomNumberGenerator& rng);
	}

}

#endif
