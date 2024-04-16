/*
    Authored By : Aniruddha Date
    Date Created: 25 January 2024
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits.h>

using namespace std;
using ld = double;

#define LEFT_LIMIT -1000000.0
#define RIGHT_LIMIT 1000000.0
#define NOT_FLAG INT_MAX
const ld epsilon = 0.00001;

enum
{
    LEFT_SIDE = -1,
    RIGHT_SIDE = 1
}typedef direction;

const int PRECISION = 0.001;

class Node
{
    private:
    ld coef;
    ld power;

    public:
    Node(ld coef, ld power) : coef(coef), power(power) {}

    Node operator+(const Node &other) const
    {
        if (power == other.power)
        {
            return Node(coef + other.coef, power);
        }
        else
        {
            throw runtime_error("Attempting to merge nodes with different powers.");
        }
    }

    ld getCoef() { return coef; }
    ld getPower() { return power; }
    void setCoef(ld c) { coef = c; return; }
    void setPower(ld p) { power = p; return; }

    // Overloaded operators to compare nodes based on power
    bool operator<(const Node &other) const
    {
        return power < other.power;
    }

    bool operator>(const Node &other) const
    {
        return power > other.power;
    }

    bool operator==(const Node &other) const
    {
        return power == other.power;
    }

    Node operator+=(const Node &other) const
    {
        Node temp(coef + other.coef, power);
        return temp;
    }

} typedef Node;

class Polynomial
{
    private:
    vector<Node> Terms;

    public:

    // CONSTRUCTORS

    Polynomial(){}

    Polynomial(const Polynomial& poly)
    {
        for(const auto& term : poly.Terms)
        {
            Terms.push_back(term);
        }
    }

    Polynomial(bool flag)
    {
        int Size;
        while (true)
        {
            cout << "Enter Number of Terms : ";
            cin >> Size;
            if (Size > 0 && Size < 21) { break; }
            else
            {
                system("cls"); // Assuming you're on Windows, otherwise you might use `system("clear")` for Unix-like systems
                cout << "Please Enter 1 <= n <= 20\n";
            }
        }

        for (int i = 0; i < Size; ++i)
        {
            ld coef, power;
            cout << "Enter coefficient for term " << i + 1 << ": ";
            cin >> coef;
            cout << "Enter power for term " << i + 1 << ": ";
            cin >> power;
            Node temp(coef, power);
            Terms.push_back(temp);
        }

        sort(Terms.begin(), Terms.end(), [](const Node &a, const Node &b)
             { return a > b; });

        int index = 0;
        while (index < Terms.size() - 1)
        {
            if (Terms[index] == Terms[index + 1])
            {
                Terms[index] = Terms[index] + Terms[index + 1];
                Terms.erase(Terms.begin() + index + 1);
            }
            else
            {
                index++;
            }
        }

        for (int i = 0; i < Terms.size(); ++i)
        {
            cout << Terms[i].getCoef() << "x^" << Terms[i].getPower() << " ";
        }
        cout << "\n";
    }

    // Overloaded '+' operator for addition
    Polynomial operator+(const Polynomial &other) const
    {
        Polynomial result;
        // Merge Terms of both polynomials
        result.Terms = Terms;
        for (const auto &term : other.Terms)
        {
            bool found = false;
            for (auto &resTerm : result.Terms)
            {
                if (term == resTerm)
                {
                    resTerm += term;
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                result.Terms.push_back(term);
            }
        }
        return result;
    }

    void print_poly();

    ld f_x(ld point);
    Polynomial d_fx();
    ld dfx_x(ld point);

    ld find_edge_interval(ld start, direction unknown);

    ld newton_raphson(ld point);
    ld bin_search(ld point1, ld point2);

    friend vector<ld> roots(Polynomial poly);

} typedef Polynomial;

ld Polynomial :: f_x(ld point)
{
    ld value = (ld) 0.0;
    for(auto term : (*this).Terms)
    {
        value += (term.getCoef() * pow(point, term.getPower()));
    }
    return value;
}

Polynomial Polynomial :: d_fx()
{
    Polynomial Copy_Polynomial((*this));
    if((*this).Terms[0].getCoef() == 0) return Copy_Polynomial;

    for(auto& term : Copy_Polynomial.Terms)
    {
        if(term.getPower() > 0)
        {
            term.setCoef(term.getCoef() * term.getPower());
            term.setPower(term.getPower() - 1);
        }
        else
        {
            term.setCoef(0);
            break;
        }
    }
    Copy_Polynomial.Terms.pop_back();
    return Copy_Polynomial;
}

ld Polynomial :: dfx_x(ld point)
{
    Polynomial temp = (*this).d_fx();
    return temp.f_x(point);
}

void Polynomial :: print_poly()
{
    for(auto& term : (*this).Terms)
    {
        cout << term.getCoef() << "x^" << term.getPower() << " ";
    }
    cout << "\n";
}

ld Polynomial :: find_edge_interval(ld start, direction unknown)
{
    ld point = start;
    ld shift = unknown * epsilon;
    while(f_x(start) * f_x(start + shift) > 0)
    {
        shift *= 2;
        if(shift > RIGHT_LIMIT || shift < LEFT_LIMIT) return NOT_FLAG;
    }
    cout << "shift = " << shift << "\n";
    point += shift;
    cout << "point = " << point << "\n";

    return point;
}

ld Polynomial :: newton_raphson(ld point)
{
    ld prev = point;
    ld next = INT_MIN;
    while(abs(prev - next) > epsilon)
    {
        next = prev - f_x(prev) / dfx_x(prev);
        prev = next;
    }
    return next;
}

ld Polynomial :: bin_search(ld point1, ld point2)
{
    while(abs(point1 - point2) > epsilon)
    {
        ld mid = (point1 + point2) / 2;
        if(abs(f_x(mid)) < epsilon) return mid;

        if(f_x(point1) * f_x(mid) < 0) point2 = mid;
        else point1 = mid;
    }
    return point1;
}

void print_vec(vector<ld> vec);

vector<ld> roots(Polynomial poly)
{
    cout << "poly term size = " << poly.Terms.size() << "\n";
    if (poly.Terms.size() == 0) return {};
    if (poly.Terms.size() == 1) return {0};

    vector<ld> ans;
    Polynomial temp = poly.d_fx();
    cout << "\t\ttemp size = " << temp.Terms.size() << "\n";
    vector<ld> starting_points = roots(temp);

    cout << "Starting points from temp roots: ";
    print_vec(starting_points);

    ld l = starting_points.front();
    ld r = starting_points.back();

    if (poly.f_x(l) == 0)
    ans.push_back(l);

    else
    {
        ld lx = poly.find_edge_interval(l, LEFT_SIDE);
        cout << "\trx = " << lx << " notflag = " << NOT_FLAG << "\n";
        if (lx != NOT_FLAG)
        {
            starting_points.insert(starting_points.begin(), lx);
            cout << "lx added to starting_points.\n";
        }
        else
        {
            cout << "lx is equal to NOT_FLAG, not added to starting_points.\n";
        }
        cout << "Updated starting points rx: ";
    }

    if (poly.f_x(r) == 0)
    ans.push_back(r);
    else
    {
        ld rx = poly.find_edge_interval(r, RIGHT_SIDE);
        cout << "\trx = " << rx << " notflag = " << NOT_FLAG << "\n";
        if (rx != NOT_FLAG)
        {
            starting_points.push_back(rx);
            cout << "rx added to starting_points.\n";
        }
        else
        {
            cout << "rx is equal to NOT_FLAG, not added to starting_points.\n";
        }
        cout << "Updated starting points rx: ";
    }
    print_vec(starting_points);

    for(int i = 0; i < starting_points.size() - 1; i++)
    {
        if(poly.f_x(starting_points[i]) * poly.f_x(starting_points[i+1]) > 0)
        {
            continue;
        }
        ld value = poly.bin_search(starting_points[i], starting_points[i+1]);
        ans.push_back(value);
    }
    return ans;
}

vector<ld> filter_roots(vector<ld> poly_roots, Polynomial p1)
{
    vector<ld> filtered_roots;
    for(int i = 0; i < poly_roots.size(); i++)
    {
        if(abs(p1.f_x(poly_roots[i])) > epsilon)
        { 
            cout << "Root " << i + 1 << " with value " << p1.f_x(poly_roots[i]) << " skipped.\n";
        }
        else
        {
            filtered_roots.push_back(poly_roots[i]);
        }
    }
    return filtered_roots;
}

void print_root_fx(vector<ld> poly_roots, Polynomial p1)
{
    for(int i = 0; i < poly_roots.size(); i++)
    {
        cout << i + 1 << ") " << "x = " << poly_roots[i] << " f(x) = " << p1.f_x(poly_roots[i]) << "\n";
    }
}

void print_vec(vector<ld> vec) {for(int i = 0; i < vec.size(); i++) cout << vec[i] << "\n";}

int main()
{
    Polynomial p1(true);
    // cout << "\t hi bro \n";
    // cout << p1.f_x(5.0) << "\n\n";
    vector<ld> poly_roots = roots(p1);
    // cout << "Roots before filtering:\n";
    // print_vec(poly_roots);
    // print_root_fx(poly_roots, p1);
 
    // poly_roots = filter_roots(poly_roots, p1);
    cout << "______________________________________________\n";
    
    cout << "\n\nRoots after filtering:\n";
    print_root_fx(poly_roots,p1);
    print_vec(poly_roots);
    
    return 0;
}
