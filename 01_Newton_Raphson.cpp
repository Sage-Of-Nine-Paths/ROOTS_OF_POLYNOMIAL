/*
    Authored By : Aniruddha Date
    Date Created: 5 January 2024
*/

// give input in form: 1x^2 - 9x^0

#include<bits/stdc++.h>
#include<windows.h>
using namespace std;

#define blank  ' '
#define e  2.71828
#define pi 3.14159

#define loop_exit _exit(); continue;

enum choices
{
    go,                 // 0
    option_1,           // find f(x0)
    option_2,           // find f'(x0)
    option_3,           // roots
    option_4,           // minimum, maximum (approx)
    option_5,           // definite integral over range(approx)
    option_6,           // plot graph
    change_polynomial,  // change the polynomial
    stop                // exit the program
};

// linked list to take input polynomial

// node containing coefficiant, power, next pointer
struct node
{
    long double coefficient;
    long double power;
    node* next;

    // constructors
    node(long double coefficient = 0, long double power = 1): coefficient(coefficient), power(power), next(nullptr) {}
    node(): coefficient(1), power(0), next(nullptr) {}
    
}typedef node;

void _exit()
{
    cout<<"\nPress Enter to exit ...";
    cin.ignore();
    cin.get();
    system("cls");
}

// input function to take polynomial as input directly
void take_input(node *&head)
{
    node *ptr = head;

    string s;
    cout << "Enter the polynomial expression: ";
    getline(cin, s);
    const int n = s.length();

    double sign = 1.0;

    for (int i = 0; i < n; ++i)
    {
        if (s[i] == blank)
            continue;

        else if (s[i] == '+')
            sign = 1.0;
        else if (s[i] == '-')
            sign = -1.0;

        bool _coef = true;
        double temp_coef = 0.0;
        double temp_pow = 0.0;

        while (i < n)
        {
            if (s[i] == '+' || s[i] == '-')
            {
                sign = (s[i] == '+') ? 1.0 : -1.0;
                ++i;
                continue;
            }

            if (s[i] >= '0' && s[i] <= '9' && _coef == true)
            {
                temp_coef = 10 * temp_coef + (s[i] - '0');
                ++i;
                continue;
            }
            else if (s[i] == '^')
            {
                _coef = false;
                ++i;
                while (s[i] == blank)
                {
                    ++i;
                    continue;
                }
                while (s[i] >= '0' && s[i] <= '9' && i < n)
                {
                    temp_pow = 10 * temp_pow + (s[i] - '0');
                    ++i;
                }
            }
            else if (s[i] == blank || (s[i] >= 'a' && s[i] <= 'z'))
            {
                ++i;
                continue;
            }

            break;
        }
        // cout << "\t" << sign * temp_coef << "\n";
        // cout << "\t" << temp_pow  << "\n";

        if (ptr == nullptr)
        {
            head = new node(sign * temp_coef, temp_pow);
            ptr = head;
        }
        else
        {
            ptr->next = new node(sign * temp_coef, temp_pow);
            ptr = ptr->next;
        }
    }
}

// this returns the differenciation value of function at given point
double differenciation(node* head, double at, double h = 0.001)
{
    if(head == nullptr)
    {
        cout << "\terror in differneciation\n";
        return 0;
    }

    double r1 = 0;
    double r2 = 0;
    node* ptr = head;
    while(ptr != nullptr)
    {
        double temp;
        temp = ptr->coefficient * pow(at, ptr->power);
        r1 += temp;
        ptr = ptr->next;
    }
    // cout << "\tr1 = " << r1 << "\n";

    at += h;
    ptr = head;

    while(ptr != nullptr)
    {
        double temp;
        temp = ptr->coefficient * pow(at, ptr->power);
        r2 += temp;
        ptr = ptr->next;
    }
    // cout << "\tr2 = " << r2 << "\n";

    double result = (r2 - r1) / h;
    

    return result;
}

// returns the value of function at given point
double value_at(node* head, double at)
{
    if(head == nullptr) return 0;

    node* ptr = head;

    double res = 0.0;

    while(ptr != nullptr)
    {
        res += ptr->coefficient * pow(at, ptr->power);
        ptr = ptr->next;
    }

    return res;
}

// returns the root value upto given steps accuracy
vector<double> roots(node* head, int steps = 5)
{
    if(head == nullptr) return {};

    double h = 0.5;
    bool plus;

    double curr_val = value_at(head, -100);
    if(curr_val > 0) plus = true;
    else plus = false;

    vector<double> initial_points;

    for(double i=-100; i<100; i+=h)
    {
        curr_val = value_at(head,i);
        if(curr_val < 0 && plus == true)
        {
            initial_points.push_back(i-h);
            plus = false;
        }
        else if(curr_val > 0 && plus == false)
        {
            initial_points.push_back(i-h);
            plus = true;
        }
    }

    const int size_of_vector = initial_points.size();

    vector<double> _roots;

    for(int i=0; i<size_of_vector; ++i)
    {
        node* ptr = head;
        double output = initial_points[i];

        int t = steps;
        while(t--)
        {
            double diff = differenciation(head, output, 0.001);
            // cout << "\tsteps = " << steps << "\t& diff = " << diff << "\t";

            if(diff <= 0.0005 && diff >= -0.0005) break;

            output = output - value_at(head,output)/diff;
            // cout << " \toutput = " << output << "\n";
        }
        _roots.push_back(output);
    }
    return _roots;
}

// returns area of function within given range
double _area(node* head, double lower_limit, double upper_limit)
{
    double steps = 1000;
    double h = (upper_limit - lower_limit) / steps;
    double integral = 0.0;
    for(double i = lower_limit; i < upper_limit; i+=h)
    {
        integral += value_at(head, i) * h;
    }
    return integral;
}

void plot_graph()
{
    // yet to be completed
}

vector<double> min_max_val(node* head, double start, double end)
{
    double minimum_val = 0;
    double maximum_val = 0;
    double h = (end - start) / 1000.0;

    for(double i=start; i <= end; i += h)
    {
        double value_at_given_point = value_at(head,i);
        minimum_val = min(minimum_val, value_at_given_point);
        maximum_val = max(maximum_val, value_at_given_point);
        cout << "value at = "<<value_at_given_point<< " ";
        cout<<"i = "<< i <<" min = "<<minimum_val<<" max = "<<maximum_val<<"\n";
    }
    
    return {minimum_val, maximum_val};
}

// menu
void menu()
{  
    node* head = nullptr;
    take_input(head);

    int choice;
    while(choice)
    {
        cout << "\n";
        cout << "Press 1 to find value of f(x)  at given point\n";
        cout << "Press 2 to find value of f'(x) at given point\n";
        cout << "Press 3 to find roots of given f(x)\n";
        cout << "Press 4 to find minimum and maximum value of polynomial in -100 to +100\n";
        cout << "Press 5 to find definite integral in given range(approx) \n";
        cout << "Press 6 to plot the graph of given equation\n";
        cout << "Press 7 to change polynomial\n";
        cout << "Press 8 to exit the program\n";

        cin >> choice;


        // find value of f(x) at given point
        if(choice == option_1)
        {
            double point;

            cout << "Enter point for which you want f(x) :: ";
            cin >> point;

            cout << value_at(head, point) << "\n";
            
            loop_exit
        }

        // find value of f'(x) at given point
        else if(choice == option_2)
        {
            double point;

            cout << "Enter point for which you want to find f'(x) :: ";
            cin >> point;

            cout << differenciation(head, point, 0.001) << "\n";
            
            loop_exit
        }

        // find roots of given polynomial
        else if(choice == option_3)
        {

            cout << "Roots of Given Polynomial are : \n";

            vector<double> _roots = roots(head,5);
            for(int i=0; i<_roots.size(); ++i)
            cout << i+1. << ")  " << _roots[i] << "\n";
            cout << "\n";
            loop_exit

        }

        // find minimum and maximum value in given range
        else if(choice == option_4)
        {
            there:
            double range_start;
            double range_end;

            cout << "Enter minimum range :: ";
            cin >> range_start;
            cout << "Enter maximum range :: ";
            cin >> range_end;

            if(range_start >= range_end)
            {
                cout << "Enter valid range\n";
                goto there;
            }

            vector<double> min_max = min_max_val(head, range_start, range_end);

            cout << "Approx minimum value = " << min_max[0] << "\n";
            cout << "Approx maximum value = " << min_max[1] << "\n";
            
            loop_exit
            
        }

        // find area (definite integral) of function in given range
        else if(choice == option_5)
        {
            here:
            double lower_limit;
            double upper_limit;

            cout << "Enter lower limit :: ";
            cin >> lower_limit;

            cout << "Enter upper limit :: ";
            cin >> upper_limit;

            if(lower_limit == upper_limit)
            {
                cout << "definite intigral of given polynomial in given range is :"
                << 0 << "\n";

                loop_exit
                
            }

            else if(upper_limit < lower_limit)
            {
                cout << "Upper limit must be greater than lower limit\n";
                goto here;
            }

            double integral = _area(head,lower_limit,upper_limit);
            cout << "definite integral of given polynomial in given range is :"
            << integral << "\n";

            loop_exit
            
        }

        // plot graph of polynomial
        else if(choice == option_6)
        {
            plot_graph();

            loop_exit
        }
        
        // change the polynomial
        else if(choice == change_polynomial)
        {
            cout << "freeing nodes of current polynomial ...\n";
            node* ptr = head;
            while(ptr != nullptr)
            {
                node* prev = ptr;
                free(ptr);
                ptr = prev->next;
            }
            cout << " \n\n Enter new polynomial :: ";
            take_input(head);
            printf("Press Enter to continue ...");

            loop_exit;

        }

        // exit the program
        else if(choice == stop)
        {
            node* ptr = head;
            cout << "\nfreeing the nodes ...\n";
            while(ptr != nullptr)
            {
                node* prev = ptr;
                free(ptr);
                ptr = prev->next;
            }
            cout << "Exiting the Program...\n\n";
            return;
        }

        // default case
        else
        {
            cout << "Enter valid choice\n";
            
            loop_exit
        }
    }
    
    return;
}

// main
int main()
{
    // calling the menu to dynamically engage with program
    menu();
    
    return 0;
}
