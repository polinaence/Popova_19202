#ifndef OOP_FIT_C_TEMPLATETUPLEPRINTER_H
#define OOP_FIT_C_TEMPLATETUPLEPRINTER_H

#include <cstdio>
#include <tuple>
#include <iostream>

namespace TupleOperators {

    static const char LEFT_BORDER_SYMBOL = '[';
    static const char RIGHT_BORDER_SYMBOL = ']';
    static const char TOKENS_DELIMITER = ' ';

    template<typename Ch, typename Tr, class Tuple, size_t N>
    class TuplePrinter {
    public:
        /*
         * Recursive printing std::tuple with N elements
         */
        static void print(std::basic_ostream<Ch, Tr> &out, const Tuple &t) {
            TuplePrinter<Ch, Tr, Tuple, N - 1>::print(out, t);
            out << TOKENS_DELIMITER << std::get<N - 1>(t);
        }
    };

    /*
     * Class-stopping print template std::tuple recursion
     */
    template<typename Ch, typename Tr, class Tuple>
    class TuplePrinter<Ch, Tr, Tuple, 1> {
    public:
        static void print(std::basic_ostream<Ch, Tr> &out, const Tuple &t) {
            out << std::get<0>(t);
        }
    };

    template<typename Ch, typename Tr, typename... Args>
    auto &operator<<(std::basic_ostream<Ch, Tr> &out, const std::tuple<Args...> &tuple) {
        out << LEFT_BORDER_SYMBOL;
        TuplePrinter<Ch, Tr, decltype(tuple), sizeof...(Args)>::print(out, tuple);
        out << RIGHT_BORDER_SYMBOL;

        return out;
    }

}


#endif //OOP_FIT_C_TEMPLATETUPLEPRINTER_H
