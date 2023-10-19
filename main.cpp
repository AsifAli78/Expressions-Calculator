#include "expevaluator.h"
#include <iostream>
#include <string>

int main() {
    expEvaluator expEval;

    std::cout << "PySUB Interpreter 1.0 on Windows (October 2023)" << std::endl;
    std::cout << "Enter program lines or read(<filename>.py) at command line interface. Type 'help' for more information or 'quit' to exit." << std::endl;

    while (true) {
        std::string input;
        std::cout << ">>> ";
        std::getline(std::cin, input);

        if (input == "quit") {
            break;
        }

        if (input == "help") {
            std::cout << "Help information here..." << std::endl;
            continue;
        }

        try {
            int result = expEval.evaluateExpression(input);
            std::cout << result << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}

