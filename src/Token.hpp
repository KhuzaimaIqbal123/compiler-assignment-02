#ifndef TOKEN_HPP
#define TOKEN_HPP
#include <string>

using namespace std;

enum class TokenType {
    NUMBER,
    PLUS,
    MINUS,
    MUL,
    DIV,
    LPAREN,
    RPAREN,
    END
};

struct Token {
    TokenType type;
    std::string value;
};

#endif
