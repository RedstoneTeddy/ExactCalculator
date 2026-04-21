#ifndef CALCULATION_ERROR_HPP
#define CALCULATION_ERROR_HPP

#include <exception>
#include <string>


enum class ErrorType {
    DivisionByZero, // 1/0
    VariableNotDefined,
    DomainError, // e.g., sqrt(-1) or log(-1) or acos(100)
    SyntaxError
};



class CalculationError : public std::exception {
protected:
    std::string message;
    ErrorType type;
public:
    CalculationError(const std::string& msg, ErrorType errType) : message(msg), type(errType) {}

    const char* what() const noexcept override {
        return message.c_str();
    }

    ErrorType getErrorType() const {
        return type;
    }
};



#endif // CALCULATION_ERROR_HPP