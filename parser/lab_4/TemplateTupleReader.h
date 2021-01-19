//
// Created by Anna on 07.01.2021.
//

#ifndef OOP_FIT_C_TEMPLATETUPLEREADER_H
#define OOP_FIT_C_TEMPLATETUPLEREADER_H

#include <iostream>
#include <tuple>
#include <sstream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <type_traits>

#include "CSVParserException.h"

namespace TupleOperators {

    static const unsigned int NEEDED_NUMBER_SKIPPED_SYMBOLS_FOR_SCREENING = 1;

    class TupleReaderParams {
    private:
        unsigned int column_;
        const char screenedSymbol_;
        const char delimiter_;
    public:
        TupleReaderParams(const unsigned int column, const char screenedSymbol, const char delimiter) :
                column_(column), screenedSymbol_(screenedSymbol), delimiter_(delimiter) {}

        [[nodiscard]] unsigned int getColumn() const {
            return column_;
        }

        [[nodiscard]] char getScreenedSymbol() const {
            return screenedSymbol_;
        }

        [[nodiscard]] char getDelimiter() const {
            return delimiter_;
        }

        void increaseColumn() {
            ++column_;
        }
    };

    void throwExceptionIfEndOfFileMetEarlierThanShouldBe(std::istringstream &stream, const unsigned int column) {
        if (stream.eof()) {
            throw CSVParser::CSVParserException("less data was encountered than expected.", column,
                                                CSVParser::ExceptionType::DataUnderflow);
        }
    }

    void throwExceptionIfStreamFailedAfterReadingInputData(std::istringstream &stream,
                                                           const unsigned int column) {
        if (stream.fail()) {
            throw CSVParser::CSVParserException("", column, CSVParser::ExceptionType::InvalidData);
        }
    }

    void throwExceptionIfExtraDataPresent(std::istringstream &stream, const unsigned int column) {
        if (!stream.eof()) {
            throw CSVParser::CSVParserException("more data was encountered than expected. This line will be skipped...",
                                                column, CSVParser::ExceptionType::DataOverflow);
        }
    }

    void skipSymbols(std::istringstream &stream, size_t skipSymbolsNumber) {
        for (int skipSymbolsCounter = 0; skipSymbolsCounter < skipSymbolsNumber; skipSymbolsCounter++) {
            stream.get();
        }
    }

    bool isNextStreamSymbolEqualScreened(std::istringstream &stream, const char screenedSymbol) {
        return stream.peek() == screenedSymbol;
    }

    bool isThereAnyFurtherScreenedSymbols(std::istringstream &stream, const char screenedSymbol) {
        if (!stream.eof()) {
            skipSymbols(stream, NEEDED_NUMBER_SKIPPED_SYMBOLS_FOR_SCREENING);
            if (!stream.eof() && stream.peek() == screenedSymbol) {
                return true;
            } else {
                stream.unget();
            }
        }

        return false;
    }

    template<typename T>
    void readFullValueContainingScreenedSymbol(T &data, std::istringstream &stream, TupleReaderParams &readerParams) {}

    void readFullValueContainingScreenedSymbol(std::string &data, std::istringstream &stream,
                                               TupleReaderParams &readerParams) {
        if (stream.eof()) {
            return;
        }

        skipSymbols(stream, NEEDED_NUMBER_SKIPPED_SYMBOLS_FOR_SCREENING);
        while (isNextStreamSymbolEqualScreened(stream, readerParams.getScreenedSymbol())) {
            throwExceptionIfEndOfFileMetEarlierThanShouldBe(stream, readerParams.getColumn());
            size_t lastSymbolStringIndex = data.size() - 1;
            data[lastSymbolStringIndex] = readerParams.getDelimiter();
            std::string extraData;
            skipSymbols(stream, NEEDED_NUMBER_SKIPPED_SYMBOLS_FOR_SCREENING);
            stream >> extraData;
            throwExceptionIfStreamFailedAfterReadingInputData(stream, readerParams.getColumn());
            data += extraData;
            if (!isThereAnyFurtherScreenedSymbols(stream, readerParams.getScreenedSymbol())) {
                break;
            }
        }
    }

    template<typename First>
    std::tuple<First>
    readNextTuple(std::istringstream &stream, TupleReaderParams &readerParams) {
        throwExceptionIfEndOfFileMetEarlierThanShouldBe(stream, readerParams.getColumn());
        First inputData;
        stream >> inputData;
        throwExceptionIfStreamFailedAfterReadingInputData(stream, readerParams.getColumn());
        readFullValueContainingScreenedSymbol(inputData, stream, readerParams);
        throwExceptionIfExtraDataPresent(stream, readerParams.getColumn());
        readerParams.increaseColumn();
        return std::make_tuple(inputData);
    }

    template<typename First, typename Second, typename ...Args>
    std::tuple<First, Second, Args...>
    readNextTuple(std::istringstream &stream, TupleReaderParams &readerParams) {
        throwExceptionIfEndOfFileMetEarlierThanShouldBe(stream, readerParams.getColumn());
        First inputData;
        stream >> inputData;
        throwExceptionIfStreamFailedAfterReadingInputData(stream, readerParams.getColumn());
        readFullValueContainingScreenedSymbol(inputData, stream, readerParams);
        readerParams.increaseColumn();
        return std::tuple_cat(std::make_tuple(inputData),
                              readNextTuple<Second, Args...>(stream, readerParams));
    }

    template<typename ...Args>
    std::tuple<Args...>
    getCsvFileCurrentRowsTuple(std::istringstream &stream, const char screenedSymbol, const char delimiter) {
        unsigned int currentColumn = 0;
        TupleReaderParams readerParams(currentColumn, screenedSymbol, delimiter);
        return readNextTuple<Args...>(stream, readerParams);
    }
}


#endif //OOP_FIT_C_TEMPLATETUPLEREADER_H
