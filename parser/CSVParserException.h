#ifndef LAB_4_CSVPARSEREXCEPTION_H
#define LAB_4_CSVPARSEREXCEPTION_H

#include <exception>
#include <string>

namespace CSVParser {

    enum class ExceptionType {
        InvalidData,
        DataOverflow,
        DataUnderflow
    };

    class CSVParserException : public std::exception {

    private:
        ExceptionType errorType_;
        std::string errorMessage_;
        unsigned int currentErrorColumn_ = 0;
    public:
        CSVParserException() = default;

        explicit CSVParserException(const std::string &errorMessage, const unsigned int column, ExceptionType type) {
            errorMessage_ = errorMessage;
            currentErrorColumn_ = column;
            errorType_ = type;
        }

        [[nodiscard]] const char *what() const noexcept override {
            return errorMessage_.c_str();
        }

        [[nodiscard]] unsigned int getErrorColumn() const noexcept {
            return currentErrorColumn_;
        }

        [[nodiscard]] ExceptionType getErrorType() const noexcept {
            return errorType_;
        }
    };

}

#endif //LAB_4_CSVPARSEREXCEPTION_H
