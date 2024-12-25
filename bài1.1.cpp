#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <cctype>
#include <stdexcept>
using namespace std;
bool isOperator(char c) {
    return c == '^' || c == '~' || c == '!' || c == '-';
}
bool isValidExpression(const string& expression) {
    stack<char> brackets;
    for (size_t i = 0; i < expression.size(); ++i) {
        char c = expression[i];
        if (c == '(' ){
            brackets.push(c);
        } else if (c == ')') {
            if (brackets.empty() || brackets.top() != '(' ){
                return false;
            }
            brackets.pop();
        } else if (!isalpha(c) && !isOperator(c) && c != ' ' && c != '(' && c != ')') {
            return false;
        }
    }
    return brackets.empty();
}

// Function to evaluate the expression
bool evaluateExpression(const string& expression, const map<char, bool>& values) {
    stack<bool> operands;
    stack<char> operators;

    for (size_t i = 0; i < expression.size(); ++i) {
        char c = expression[i];

        if (c == ' ') {
            continue;
        } else if (isalpha(c)) {
            if (values.find(c) == values.end()) {
                throw invalid_argument("Variable value not provided: " + string(1,c));
            }
            operands.push(values.at(c));
        } else if (c == '(' ){
            operators.push(c);
        } else if (c == ')') {
            while (!operators.empty() && operators.top() != '(' ){
                char op = operators.top();
                operators.pop();

                if (op == '!') {
                    bool val = operands.top(); operands.pop();
                    operands.push(!val);
                } else {
                    bool right = operands.top(); operands.pop();
                    bool left = operands.top(); operands.pop();

                    if (op == '^') operands.push(left && right);
                    else if (op == '~') operands.push(left || right);
                    else if (op == '-') operands.push(!left || right);
                }
            }
            operators.pop(); // Pop '('
        } else if (isOperator(c)) {
            while (!operators.empty() && operators.top() != '(') {
                char op = operators.top();
                if ((op == '!') || (op != '*' && c != '!')) break;
                operators.pop();

                if (op == '!') {
                    bool val = operands.top(); operands.pop();
                    operands.push(!val);
                } else {
                    bool right = operands.top(); operands.pop();
                    bool left = operands.top(); operands.pop();

                    if (op == '^') operands.push(left && right);
                    else if (op == '~') operands.push(left || right);
                    else if (op == '-') operands.push(!left || right);
                }
            }
            operators.push(c);
        }
    }

    while (!operators.empty()) {
        char op = operators.top(); operators.pop();

        if (op == '!') {
            bool val = operands.top(); operands.pop();
            operands.push(!val);
        } else {
            bool right = operands.top(); operands.pop();
            bool left = operands.top(); operands.pop();

            if (op == '^') operands.push(left && right);
            else if (op == '~') operands.push(left || right);
            else if (op == '-') operands.push(!left || right);
        }
    }

    return operands.top();
}

int main() {
    string expression;
    cout << "Enter a logical expression: ";
    getline(cin, expression);

    if (!isValidExpression(expression)) {
        cout << "Invalid logical expression." << endl;
        return 1;
    }

    map<char, bool> values;
    cout << "Enter values for variables (e.g., A 1 for True, A 0 for False):" << endl;
    char var;
    bool val;
    while (cin >> var >> val) {
        values[var] = val;
        if (cin.peek() == '\n') break;
    }

    try {
        bool result = evaluateExpression(expression, values);
        cout << "The result of the expression is: " << (result ? "True" : "False") << endl;
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
