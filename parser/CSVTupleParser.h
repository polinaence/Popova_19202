#ifndef LAB_4_CSVTUPLEPARSER_H
#define LAB_4_CSVTUPLEPARSER_H

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <locale>
#include <algorithm>
#include <stdexcept>

#include "TemplateTuplePrinter.h"
#include "TemplateTupleReader.h"
#include "CSVParserException.h"


namespace CSVParser {

    static const char DEFAULT_CSV_DELIMITER = ';';
    static const char DEFAULT_CSV_SCREENED_SYMBOL = '\"';
    static const int DEFAULT_LINES_TO_SKIP_NUMBER = 0;

    template<typename ...Args>
    class CSVTupleParser {
    private:
        std::ifstream &csvInputStream_;
        char csvDelimiter_ = CSVParser::DEFAULT_CSV_DELIMITER;
        char csvScreenedSymbol_ = CSVParser::DEFAULT_CSV_SCREENED_SYMBOL;
        unsigned int csvLinesToSkipNumber_ = CSVParser::DEFAULT_LINES_TO_SKIP_NUMBER;

        void skipLines() {
            std::string temporaryString;
            unsigned int currentLineNumber = csvLinesToSkipNumber_;
            while (currentLineNumber != 0) {
                std::getline(csvInputStream_, temporaryString);
                --currentLineNumber;
            }
        }

        size_t getStartingRowsForIterationNumber() {
            return csvLinesToSkipNumber_ + 1;
        }

    public:
        CSVTupleParser(std::ifstream &inputStream, const size_t skipLinesNumber,
                       const char delimiter, const char screenedSymbol) :
                csvInputStream_(inputStream),
                csvDelimiter_(delimiter),
                csvLinesToSkipNumber_(skipLinesNumber),
                csvScreenedSymbol_(screenedSymbol) {
            skipLines();
        };

        class iterator : public std::iterator<std::input_iterator_tag, std::tuple<Args...>> {
        private:
            std::ifstream &itInputStream_;
            std::tuple<Args...> *itTuple_ = nullptr;
            unsigned int itCurrentRow_;
            std::string itCurrentLine_;
            char itDelimiter_;
            char itScreenedSymbol_;
            std::streampos itPosition_;

            void setStringStreamLocateParams(std::istringstream &stream) {
                std::locale loc(std::locale::classic(), new CSVParserCType(itDelimiter_));
                stream.imbue(loc);
            }

            class CSVParserCType : public std::ctype<char> {
            private:
                mask performedTable[table_size]{};

            public:
                explicit CSVParserCType(char delimiter, size_t refs = 0)
                        : std::ctype<char>(&performedTable[0], false, refs) {
                    std::copy_n(classic_table(), table_size, performedTable);
                    performedTable[' '] = lower;
                    performedTable[delimiter] = space;
                }
            };

            void readNextTuple() {
                try {
                    readNextFileRowToCurrentLine();
                    saveStreamPosition();
                    std::istringstream stringStream(itCurrentLine_);
                    setStringStreamLocateParams(stringStream);
                    *itTuple_ = TupleOperators::getCsvFileCurrentRowsTuple<Args...>(stringStream, itScreenedSymbol_,
                                                                                    itDelimiter_);
                    ++itCurrentRow_;
                } catch (CSVParserException &exception) {
                    switch (exception.getErrorType()) {
                        case ExceptionType::InvalidData:
                            printAboutInvalidDataFromException(exception);
                            throwRuntimeExceptionToStopProgram();
                            break;
                        case ExceptionType::DataUnderflow:
                            printAboutDataUnderflowFromException(exception);
                            throwRuntimeExceptionToStopProgram();
                            break;
                        case ExceptionType::DataOverflow:
                            printWarningAboutDataOverflow(exception);
                            continueIterations();
                            break;
                    }
                }
            }

            void throwRuntimeExceptionToStopProgram() {
                throw std::runtime_error("");
            }

            void printAboutInvalidDataFromException(const CSVParserException &exception) {
                std::cerr << "CSVTupleParser Iterator: in row " << itCurrentRow_
                          << ", in column " << exception.getErrorColumn()
                          << " an inappropriate type was encountered!" << std::endl;
            }

            void printAboutDataUnderflowFromException(const CSVParserException &exception) {
                std::cerr << "CSVTupleParser Iterator: in row " << itCurrentRow_
                          << " " << exception.what() << std::endl;
            }

            void printWarningAboutDataOverflow(const CSVParserException &exception) {
                std::cerr << "CSVTupleParser Iterator: in row " << itCurrentRow_
                          << " " << exception.what() << std::endl;
            }

            void readNextFileRowToCurrentLine() {
                itInputStream_.clear();
                itInputStream_.seekg(itPosition_);
                itInputStream_ >> itCurrentLine_;
                itPosition_ = itInputStream_.tellg();
            }

            void saveStreamPosition() {
                itPosition_ = itInputStream_.tellg();
            }

            void continueIterations() {
                ++itCurrentRow_;
                ++(*this);
            }

            [[nodiscard]] bool isCsvInputStreamReachedEndOfFile() const {
                return itPosition_ == EOF;
            }

        public:
            friend class CSVTupleParser<Args...>;

            iterator(std::ifstream &itInputStream, const unsigned int row, const char delimiter,
                     const char screenedSymbol, const std::streampos streamPosition) :
                    itInputStream_(itInputStream),
                    itCurrentRow_(row),
                    itDelimiter_(delimiter),
                    itScreenedSymbol_(screenedSymbol),
                    itPosition_(streamPosition) {

                itTuple_ = new std::tuple<Args...>;
                if (!isCsvInputStreamReachedEndOfFile()) {
                    readNextTuple();
                }
            }

            ~iterator() {
                delete itTuple_;
            }

            iterator &operator++() {
                if (!isCsvInputStreamReachedEndOfFile()) {
                    readNextTuple();
                }
                return *this;
            }

            bool operator==(const iterator &other) const {
                if (other.isCsvInputStreamReachedEndOfFile() && this->isCsvInputStreamReachedEndOfFile()) {
                    return true;
                }
                return itCurrentLine_ == other.itCurrentLine_;
            }

            bool operator!=(const iterator &other) const {
                return !(*this == other);
            }

            typename iterator::reference operator*() const {
                return *itTuple_;
            }
        };

        iterator begin() {
            return iterator(csvInputStream_, getStartingRowsForIterationNumber(), csvDelimiter_, csvScreenedSymbol_,
                            csvInputStream_.tellg());
        }

        iterator end() {
            return iterator(csvInputStream_, UINT_MAX, csvDelimiter_, csvScreenedSymbol_, EOF);
        }
    };


}


#endif //LAB_4_CSVTUPLEPARSER_H
