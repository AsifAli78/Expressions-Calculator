#include "expevaluator.h"
#include <stack>
#include <stdexcept>
#include <cctype>
#include <iostream>

expEvaluator::expEvaluator() {
    // Constructor, if needed
}

int expEvaluator::getOperatorPrecedence(char op) {
    // Define operator precedence for both arithmetic and logical operators
    if (op == '+' || op == '-') {
        return 1;
    }
    else if (op == '*' || op == '/' || op == '%') {
        return 2;
    }
    else if (op == '<' || op == '>' || op == '<=' || op == '>=' || op == '!=' || op == '==') {
        return 3;
    }
    else if (op == 'and') {
        return 0;
    }
    else if (op == 'or') {
        return 0;
    }
    else if (op == 'not') {
        return 0;
    }
    return 0; // Default precedence
}

void expEvaluator::applyAndPush(std::stack<Operand>& operandStack, std::stack<char>& operatorStack) {
    if (operatorStack.empty()) {
        return; // Nothing to apply
    }

    char oper = operatorStack.top();
    operatorStack.pop();

    if (oper == 'and' || oper == 'or' || oper == 'not') {
        // Handle logical operators
        if (operandStack.size() < 2) {
            throw std::runtime_error("Invalid expression");
        }

        Operand op2 = operandStack.top();
        operandStack.pop();
        Operand op1 = operandStack.top();
        operandStack.pop();

        bool result = false;

        if (oper == 'and') {
            result = op1.booleanValue && op2.booleanValue;
        }
        else if (oper == 'or') {
            result = op1.booleanValue || op2.booleanValue;
        }
        else if (oper == 'not') {
            result = !op2.booleanValue;
        }

        operandStack.push(Operand(result));
    }
    else {
        // Handle arithmetic operators
        if (operandStack.size() < 2) {
            throw std::runtime_error("Invalid expression");
        }

        Operand op2 = operandStack.top();
        operandStack.pop();
        Operand op1 = operandStack.top();
        operandStack.pop();

        int result = 0;

        if (oper == '+') {
            result = op1.intValue + op2.intValue;
        }
        else if (oper == '-') {
            result = op1.intValue - op2.intValue;
        }
        else if (oper == '*') {
            result = op1.intValue * op2.intValue;
        }
        else if (oper == '/') {
            if (op2.intValue == 0) {
                throw std::runtime_error("Division by zero");
            }
            result = op1.intValue / op2.intValue;
        }
        else if (oper == '%') {
            if (op2.intValue == 0) {
                throw std::runtime_error("Modulo by zero");
            }
            result = op1.intValue % op2.intValue;
        }
        operandStack.push(Operand(result));
    }
}

Operand expEvaluator::applyOperator(Operand op1, Operand op2, char oper) {
    // Handle the application of logical operators
    if (oper == 'and') {
        return Operand(op1.booleanValue && op2.booleanValue);
    }
    else if (oper == 'or') {
        return Operand(op1.booleanValue || op2.booleanValue);
    }
    else if (oper == 'not') {
        return Operand(!op2.booleanValue);
    }

    // Handle the application of arithmetic operators
    int num1 = op1.intValue;
    int num2 = op2.intValue;

    switch (oper) {
        case '+':
            return Operand(num1 + num2);
        case '-':
            return Operand(num1 - num2);
        case '*':
            return Operand(num1 * num2);
        case '/':
            if (num2 == 0) {
                throw std::runtime_error("Division by zero");
            }
            return Operand(num1 / num2);
        case '%':
            if (num2 == 0) {
                throw std::runtime_error("Modulo by zero");
            }
            return Operand(num1 % num2);
        case '<':
            return Operand(num1 < num2);
        case '>':
            return Operand(num1 > num2);
        case '<=':
            return Operand(num1 <= num2);
        case '>=':
            return Operand(num1 >= num2);
        case '==':
            return Operand(num1 == num2);
        case '!=':
            return Operand(num1 != num2);
        default:
            throw std::runtime_error("Invalid operator");
    }
}

bool expEvaluator::isOperator(char c) {
    // Check if a character is a valid operator
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
            c == '<' || c == '>' || c == '=' || c == '!' || c == 'a' || c == 'o' || c == 'n');
}

bool expEvaluator::isOperand(char c) {
    // Check if a character is a valid operand
    return std::isalnum(c);
}

bool expEvaluator::isWhitespace(char c) {
    // Check if a character is whitespace
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

bool expEvaluator::evaluateExpression(const std::string& expression) {
    std::stack<Operand> operandStack;
    std::stack<char> operatorStack;

    for (size_t i = 0; i < expression.size(); ++i) {
        if (isWhitespace(expression[i])) {
            continue; // Skip whitespace
        }
        else if (expression[i] == '(') {
            operatorStack.push(expression[i]);
        }
        else if (expression[i] == ')') {
            while (!operatorStack.empty() && operatorStack.top() != '(') {
                applyAndPush(operandStack, operatorStack);
            }
            if (!operatorStack.empty()) {
                operatorStack.pop(); // Remove the '('
            } else {
                throw std::runtime_error("Unbalanced parentheses");
            }
        }
        else if (isOperator(expression[i])) {
            char currOperator = expression[i];
            while (!operatorStack.empty() && operatorStack.top() != '(' && getOperatorPrecedence(operatorStack.top()) >= getOperatorPrecedence(currOperator)) {
                applyAndPush(operandStack, operatorStack);
            }
            operatorStack.push(currOperator);
        }
        else {
            // Read operands (numbers or variables)
            size_t j = i;
            while (j < expression.size() && (isOperand(expression[j]) || expression[j] == '_')) {
                j++;
            }
            std::string operand = expression.substr(i, j - i);
            i = j - 1; // Move i to the last character of the operand
            if (std::isdigit(operand[0]) || (operand[0] == '-' && operand.size() > 1 && std::isdigit(operand[1]))) {
                int num = std::stoi(operand);
                operandStack.push(Operand(num));
            }
            else if (operand == "true" || operand == "false") {
                bool value = (operand == "true");
                operandStack.push(Operand(value));
            }
            else {
                throw std::runtime_error("Invalid operand: " + operand);
            }
        }
    }

    while (!operatorStack.empty()) {
        if (operatorStack.top() == '(') {
            throw std::runtime_error("Unbalanced parentheses");
        }
        applyAndPush(operandStack, operatorStack);
    }

    if (operandStack.size() == 1 && operandStack.top().isBoolean()) {
        return operandStack.top().booleanValue;
    }
    else if (operandStack.size() == 1 && operandStack.top().isInteger()) {
        throw std::runtime_error("Expected a boolean result");
    }
    throw std::runtime_error("Invalid expression");
}

void expEvaluator::clearSymbolTable() {
    symbolTable.clear();
}

void expEvaluator::setSymbolValue(const std::string& variable, int value) {
    symbolTable[variable] = Operand(value);
}

void expEvaluator::setSymbolValue(const std::string& variable, bool value) {
    symbolTable[variable] = Operand(value);
}

bool expEvaluator::isBooleanExpression(const std::string& expression) {
    for (char c : expression) {
        if (c == '<' || c == '>' || c == '=' || c == '!' || c == 'a' || c == 'o' || c == 'n') {
            return true;
        }
    }
    return false;
}
