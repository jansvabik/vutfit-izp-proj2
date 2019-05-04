/*
 * IZP, proj2
 * Author: Jan Svabik, xsvabi00
 * Created: 2018-11-02
 * Last update: 2018-11-22
 * Version: 1.2
 */

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * CHECKING FUNCTIONS DECLARATION
 */
int checkArguments(int argc, char* argv[]);
int isAllDigits(char* s);

/*
 * COMPUTING FUNCTIONS DECLARATION
 */
void handleLogarithm(double x, unsigned int n);
void handlePower(double x, double y, unsigned int n);

double taylor_log(double x, unsigned int n);
double cfrac_log(double x, unsigned int n);

double taylor_pow(double x, double y, unsigned int n);
double taylorcf_pow(double x, double y, unsigned int n);

double do_pow(double log, double x, double y, unsigned int n);

/*
 * MAIN
 */
int main(int argc, char* argv[]) {
    // check arguments
    if (!checkArguments(argc, argv))
        return EXIT_SUCCESS;

    if (!strcmp(argv[1], "--log"))
        handleLogarithm(strtod(argv[2], NULL), strtol(argv[3], NULL, 10));
    else
        handlePower(strtod(argv[2], NULL), strtod(argv[3], NULL), strtol(argv[4], NULL, 10));
    
    return EXIT_SUCCESS;
}

void handleLogarithm(double x, unsigned int n) {
    printf("       log(%.5g) = %.12g\n", x, log(x));
    printf(" cfrac_log(%.5g) = %.12g\n", x, cfrac_log(x, n));
    printf("taylor_log(%.5g) = %.12g\n", x, taylor_log(x, n));
}

void handlePower(double x, double y, unsigned int n) {
    printf("         pow(%.5g,%.5g) = %.12g\n", x, y, pow(x, y));
    printf("  taylor_pow(%.5g,%.5g) = %.12g\n", x, y, taylor_pow(x, y, n));
    printf("taylorcf_pow(%.5g,%.5g) = %.12g\n", x, y, taylorcf_pow(x, y, n));
}

/*
 * COMPUTING FUNCTIONS
 */

// continued fractin logarithm
double cfrac_log(double x, unsigned int n) {
    if (x < 0)
        return NAN;

    if (x == 0)
        return -INFINITY;

    if (x == INFINITY)
        return INFINITY; 

    double fin = 1;
    double nx = (x - 1) / (x + 1);
    double nxp = nx*nx;

    for (unsigned int i = n; i > 0; i--)
        fin = (2*i - 1) - (i*i*nxp)/fin;

    return 2*nx/fin;
}

// taylor logarithm
double taylor_log(double x, unsigned int n) {
    if (x < 0)
        return NAN;

    if (x == 0)
        return -INFINITY;

    if (x == INFINITY)
        return INFINITY; 

    double fin = 0;
    
    if (x > 1) {
        double xmxdx = (x - 1) / x;
        double lastpow = xmxdx;
        for (unsigned int i = 1; i <= n; i++) {
            fin += lastpow / i;
            lastpow *= xmxdx;
        }
    }
    else {
        double nx = 1-x;
        double lastpow = nx;
        for (unsigned int i = 1; i <= n; i++) {
            fin -= lastpow / i;
            lastpow *= nx;
        }
    }

    return fin;
}

// taylor - power
double taylor_pow(double x, double y, unsigned int n) {
    // add support for negative numbers
    double logx = x;
    if (x < 0)
        logx = -x;

    // calculate the power
    return do_pow(taylor_log(logx, n), x, y, n);
}

// continued fraction - power
double taylorcf_pow(double x, double y, unsigned int n) {
    // add support for negative numbers
    double logx = x;
    if (x < 0)
        logx = -x;

    // calculate the power
    return do_pow(cfrac_log(logx, n), x, y, n);
}

// calculate the power
double do_pow(double log, double x, double y, unsigned int n) {
    double fin = 1;
    
    long double ypow = y;
    double taylogpow = log;
    long double fact = 1.0;

    for (unsigned int i = 1; i <= n; i++) {
        fin += ypow * taylogpow / fact;
        ypow *= y;
        taylogpow *= log;
        fact *= i+1;
    }

    // do the negation if x was negative
    if (x < 0)
        fin = -fin;

    // return result
    return fin;
}

/*
 * CHECKING FUNCTIONS
 */
// check program arguments
int checkArguments(int argc, char* argv[]) {
    // check arguments number and logic
    if (
        (argc < 4 || argc > 5) ||
        (argc == 4 && strcmp(argv[1], "--log")) ||
        (argc == 5 && strcmp(argv[1], "--pow"))
    ) {
        printf("Program usage: \nLog. func.: --log X N (X = number; N = iterations)\nPow. funct.: --pow X Y N (X = num.; Y = power; N = iter.)\n");
        return 0;
    }

    // check that N argument is integer
    if (
        (!strcmp(argv[1], "--log") && !isAllDigits(argv[3])) ||
        (!strcmp(argv[1], "--pow") && !isAllDigits(argv[4]))        
    ) {
        printf("N (number of iterations) must be an integer greater than 0.");
        return 0;
    }

    return 1;
}

// is every char of string digit?
int isAllDigits(char* s) {
    for (unsigned int i = 0; s[i] != '\0'; i++)
        if (!isdigit(s[i]))
            return 0;

    return 1;
}