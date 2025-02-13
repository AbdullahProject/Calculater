#include <iostream>
#include <sstream>
#include <stack>
#include <cctype>
#include <cmath>

using namespace std;

// Function to determine precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Function to perform arithmetic operations
double applyOperation(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return (b != 0) ? a / b : NAN;
        default: return 0;
    }
}

// Function to evaluate an expression given as a string
double evaluateExpression(const string &expression, bool &valid) {
    stack<double> values;
    stack<char> ops;
    stringstream ss(expression);
    char ch;
    valid = true;
    
    while (ss >> ch) {
        if (isdigit(ch) || ch == '.') {
            ss.putback(ch);
            double value;
            ss >> value;
            values.push(value);
        } else if (ch == '(') {
            ops.push(ch);
        } else if (ch == ')') {
            if (ops.empty()) {
                valid = false;
                return 0;
            }
            while (!ops.empty() && ops.top() != '(') {
                double b = values.top(); values.pop();
                if (values.empty()) {
                    valid = false;
                    return 0;
                }
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperation(a, b, op));
            }
            if (!ops.empty()) ops.pop(); // Remove '('
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            while (!ops.empty() && precedence(ops.top()) >= precedence(ch)) {
                double b = values.top(); values.pop();
                if (values.empty()) {
                    valid = false;
                    return 0;
                }
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperation(a, b, op));
            }
            ops.push(ch);
        } else {
            valid = false;
            return 0;
        }
    }
    
    while (!ops.empty()) {
        if (values.size() < 2) {
            valid = false;
            return 0;
        }
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOperation(a, b, op));
    }
    
    return values.top();
}

int main() {
    char choice;
    while (true) {
        string expression;
        bool valid;
        
        cout << "Enter an expression (or press E to exit): ";
        getline(cin, expression);
        
        if (expression == "E" || expression == "e") {
            break;
        }
        
        double result = evaluateExpression(expression, valid);
        if (!valid || isnan(result)) {
            cout << "Invalid expression. Please enter a valid expression or press E to exit." << endl;
            continue;
        }
        
        cout << "Result: " << result << endl;
    }
    
    return 0;
}

