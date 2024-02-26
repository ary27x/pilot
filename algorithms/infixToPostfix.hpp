#ifndef __INFIX_TO_POSTFIX__
#define __INFIX_TO_POSTFIX__

#include "../headers/parser.hpp"
#include <stack>
#include <unordered_map>
#include <memory>
#include <vector>

// Enhanced NodeType with enum class for stronger type safety.
enum class NodeType {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    INT,
    VARIABLE
};

// Operator precedence mapping for determining stack operations.
std::unordered_map<NodeType, int> operatorPrecedence = {
    {NodeType::OP_ADD, 0},
    {NodeType::OP_SUB, 0},
    {NodeType::OP_MUL, 1},
    {NodeType::OP_DIV, 1},
    {NodeType::OP_MOD, 1},
};

// Class to convert infix expressions to postfix using AST nodes.
class InfixToPostfixConverter {
public:
    // Converts a vector of AST nodes from infix to postfix notation.
    std::vector<std::shared_ptr<AST_NODE>> convert(const std::vector<std::shared_ptr<AST_NODE>>& operations) {
        std::vector<std::shared_ptr<AST_NODE>> resultant;
        std::stack<std::shared_ptr<AST_NODE>> operatorStack;

        for (const auto& current : operations) {
            // Skip processing if the current node is null.
            if (!current) continue;

            // Directly add operands to the resultant vector.
            if (current->type == NodeType::INT || current->type == NodeType::VARIABLE) {
                resultant.push_back(current);
            } else {
                // Process operators based on their precedence.
                processOperators(current, resultant, operatorStack);
            }
        }

        // Empty the stack, adding remaining operators to the resultant vector.
        while (!operatorStack.empty()) {
            resultant.push_back(operatorStack.top());
            operatorStack.pop();
        }

        return resultant;
    }

private:
    // Handles the logic of operator precedence and stacking.
    void processOperators(const std::shared_ptr<AST_NODE>& current, std::vector<std::shared_ptr<AST_NODE>>& resultant, std::stack<std::shared_ptr<AST_NODE>>& operatorStack) {
        while (true) {
            if (operatorStack.empty() || operatorPrecedence.at(current->type) > operatorPrecedence.at(operatorStack.top()->type)) {
                operatorStack.push(current);
                break;
            } else {
                resultant.push_back(operatorStack.top());
                operatorStack.pop();
            }
        }
    }
};

#endif // __INFIX_TO_POSTFIX__
