#include "Parser.hpp"
#include <stdexcept>

Parser::Parser(const std::string& input) : lexer(input) {
    advance();
}

void Parser::advance() {
    currentToken = lexer.getNextToken();
}

void Parser::eat(TokenType type) {
    if (currentToken.type == type) {
        advance();
    } else {
        throw std::runtime_error("Unexpected token: " + currentToken.value);
    }
}

std::unique_ptr<Expr> Parser::parse() {
    return parseExpression();
}

// Expression -> Term { ('+' | '-') Term }
std::unique_ptr<Expr> Parser::parseExpression() {
    auto node = parseTerm();

    while (currentToken.type == TokenType::PLUS ||
           currentToken.type == TokenType::MINUS) {
        Token op = currentToken;
        if (op.type == TokenType::PLUS) eat(TokenType::PLUS);
        else eat(TokenType::MINUS);

        auto right = parseTerm();
        node = std::make_unique<BinaryExpr>(op.value, std::move(node), std::move(right));
    }

    return node;
}

// Term -> Factor { ('*' | '/') Factor }
std::unique_ptr<Expr> Parser::parseTerm() {
    auto node = parseFactor();

    while (currentToken.type == TokenType::MUL ||
           currentToken.type == TokenType::DIV) {
        Token op = currentToken;
        if (op.type == TokenType::MUL) eat(TokenType::MUL);
        else eat(TokenType::DIV);

        auto right = parseFactor();
        node = std::make_unique<BinaryExpr>(op.value, std::move(node), std::move(right));
    }

    return node;
}

// Factor -> NUMBER | '(' Expression ')'
std::unique_ptr<Expr> Parser::parseFactor() {
    Token tok = currentToken;

    if (tok.type == TokenType::NUMBER) {
        eat(TokenType::NUMBER);
        return std::make_unique<NumberExpr>(std::stoi(tok.value));
    } else if (tok.type == TokenType::LPAREN) {
        eat(TokenType::LPAREN);
        auto node = parseExpression();
        eat(TokenType::RPAREN);
        return node;
    }

    throw std::runtime_error("Unexpected token in factor: " + tok.value);
}
