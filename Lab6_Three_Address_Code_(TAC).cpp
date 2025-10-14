#include <bits/stdc++.h>
using namespace std;

int tempCount = 1;

struct TAC {
    string result, op, arg1, arg2;
};

vector<TAC> code;

// Create new temporary variable
string newTemp() {
    return "T" + to_string(tempCount++);
}

// Operator precedence
int precedence(char op) {
    if (op == '^')
        return 3;
    if (op == '*' || op == '/' || op == '%')
        return 2;
    if (op == '+' || op == '-')
        return 1;
    if (op == '=')
        return 0;
    return -1;
}

// Apply operator to generate TAC
string applyOp(string a, string b, char op) {
    string t = newTemp();
    TAC instr;
    instr.result = t;
    instr.op = string(1, op);
    instr.arg1 = a;
    instr.arg2 = b;
    code.push_back(instr);
    return t;
}

// Handle unary minus
string applyUnary(string a) {
    string t = newTemp();
    TAC instr;
    instr.result = t;
    instr.op = "uminus";
    instr.arg1 = a;
    instr.arg2 = "";
    code.push_back(instr);
    return t;
}

// Handle unary sqrt
string applyUnarySqrt(string a) {
    string t = newTemp();
    TAC instr;
    instr.result = t;
    instr.op = "sqrt";
    instr.arg1 = a;
    instr.arg2 = "";
    code.push_back(instr);
    return t;
}

// Check if operator
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

// Convert expression to TAC using Shunting Yard logic
string generateTAC(string expr) {
    stack<char> ops;
    stack<string> vals;
    for (int i = 0; i < expr.size(); i++) {
        if (expr[i] == ' ')
            continue;
        // '('
        if (expr[i] == '(') {
            ops.push('(');
        }
        // ')'
        else if (expr[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                string b = vals.top();
                vals.pop();
                string a = vals.top();
                vals.pop();
                char op = ops.top();
                ops.pop();
                vals.push(applyOp(a, b, op));
            }
            ops.pop(); // remove '('
        }
        // Operator
        else if (isOperator(expr[i])) {
            // Handle unary minus
            // Handle unary minus (works for both variables and bracketed
            // expressions)
            if (expr[i] == '-' &&
                (i == 0 || expr[i - 1] == '(' || isOperator(expr[i - 1]))) {
                i++;
                string inner = "";
                // Case 1: if next is '(' then capture full subexpression
                if (expr[i] == '(') {
                    int open = 1;
                    i++;
                    while (i < expr.size() && open > 0) {
                        if (expr[i] == '(')
                            open++;
                        else if (expr[i] == ')')
                            open--;
                        if (open > 0)
                            inner += expr[i];
                        i++;
                    }
                    i--; // step back after closing parenthesis
                    string tInner = generateTAC(inner);
                    vals.push(applyUnary(tInner));
                }
                // Case 2: single variable or number
                else {
                    string operand = "";
                    while (i < expr.size() &&
                           (isalnum(expr[i]) || expr[i] == '_'))
                        operand += expr[i++];
                    i--;
                    vals.push(applyUnary(operand));
                }
                continue;
            }
            while (!ops.empty() &&
                   precedence(ops.top()) >= precedence(expr[i])) {
                string b = vals.top();
                vals.pop();
                string a = vals.top();
                vals.pop();
                char op = ops.top();
                ops.pop();
                vals.push(applyOp(a, b, op));
            }
            ops.push(expr[i]);
        }
        // Operand
        else if (isalnum(expr[i])) {
            string operand = "";
            while (i < expr.size() && (isalnum(expr[i]) || expr[i] == '_'))
                operand += expr[i++];
            //      i--;
            // Check for 'sqrt(' function call
            if (operand == "sqrt" && i < expr.size() && expr[i] == '(') {
                i++; // Consume '('
                string inner = "";
                int open = 1;
                // Extract the subexpression inside sqrt()
                while (i < expr.size() && open > 0) {
                    if (expr[i] == '(')
                        open++;
                    else if (expr[i] == ')')
                        open--;
                    if (open > 0)
                        inner += expr[i];
                    i++;
                }
                // i is now past the final ')'
                // Recurse to generate TAC for the inner expression (argument)
                string tInner = generateTAC(inner);
                // Apply the sqrt function operator
                vals.push(applyUnarySqrt(tInner));
                i--;
            } else {
                // Regular operand (variable or number)
                i--; // MUST step back for the main loop's i++ to land on the
                     // next token
                vals.push(operand);
            }
        }
    }
    while (!ops.empty()) {
        string b = vals.top();
        vals.pop();
        string a = vals.top();
        vals.pop();
        char op = ops.top();
        ops.pop();
        vals.push(applyOp(a, b, op));
    }
    return vals.top();
}

int main() {
    string expr;
    cout << "Enter an expression: ";
    getline(cin, expr);
    string result = generateTAC(expr);
    cout << "\nThree Address Code:\n";
    for (int i = 0; i < code.size(); i++) {
        cout << "(" << i + 1 << ") " << code[i].result << " = ";
        if (code[i].op == "uminus")
            cout << "uminus " << code[i].arg1;
        else if (code[i].op == "sqrt")
            cout << code[i].op << " (" << code[i].arg1 << ") ";
        else
            cout << code[i].arg1 << " " << code[i].op << " " << code[i].arg2;
        cout << endl;
    }
    cout << "(" << code.size() + 1 << ") " << "Result = " << result << endl;
    return 0;
}