/*
    Authored By : Aniruddha Date
    Date Created: 10 February 2024
*/

#include <iostream>
#include <complex>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// Function to calculate the value of a polynomial at a given point
complex<double> polynomial_value(const vector<pair<double, int>> &terms, const complex<double> &x)
{
    complex<double> result = 0.0;
    for (const auto &term : terms)
    {
        result += term.first * pow(x, term.second);
    }
    return result;
}

// Durand-Kerner method to find roots of polynomial
vector<complex<double>> durand_kerner(const vector<pair<double, int>> &terms, double tolerance = 1e-10, int max_iterations = 1000)
{
    int n = terms.size(); // Number of terms in the polynomial
    vector<complex<double>> roots(n - 1);

    // Initial guess for the roots (usually equally spaced around a circle)
    double angle_increment = 2.0 * M_PI / n;
    for (int i = 0; i < n - 1; ++i)
    {
        roots[i] = polar(1.0, angle_increment * i);
    }

    // Iteratively refine the roots
    for (int iter = 0; iter < max_iterations; ++iter)
    {
        vector<complex<double>> new_roots(n - 1);

        // Calculate the next iteration of each root
        for (int i = 0; i < n - 1; ++i)
        {
            complex<double> root = roots[i];
            complex<double> numerator = polynomial_value(terms, root);
            complex<double> denominator = 1.0;
            for (int j = 0; j < n - 1; ++j)
            {
                if (i != j)
                {
                    denominator *= (root - roots[j]);
                }
            }
            new_roots[i] = root - numerator / denominator;
        }

        // Check for convergence
        bool converged = true;
        for (int i = 0; i < n - 1; ++i)
        {
            if (abs(new_roots[i] - roots[i]) > tolerance)
            {
                converged = false;
                break;
            }
        }

        // If converged, return the roots
        if (converged)
        {
            return new_roots;
        }

        // Update roots for the next iteration
        roots = new_roots;
    }

    cerr << "Warning: Maximum number of iterations reached without convergence." << endl;
    return roots;
}

int main()
{
    // Input the coefficients and powers of x
    int n;
    cout << "Enter the highest power of x in the polynomial: ";
    cin >> n;

    vector<pair<double, int>> terms(n + 1, {0.0, 0});
    cout << "Enter the coefficients of the polynomial: ";
    for (int i = n; i >= 0; --i)
    {
        cout << "coef of x^" << i << " = ";
        cin >> terms[i].first;
        terms[i].second = i;
    }

    // Calculate the roots
    vector<complex<double>> roots = durand_kerner(terms);

    // Output the roots
    cout << "Roots of the polynomial: ";
    for (const auto &root : roots)
    {
        cout << root << "\n";
    }
    cout << endl;

    return 0;
}
