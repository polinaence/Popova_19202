#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <tuple>
#include <algorithm>
#include <iterator>
#include <vector>
#include <stdexcept>

template<class TupType, size_t... I>
void printany(const TupType& _tup, std::index_sequence<I...>, std::ostream& out)
{
    std::cout << "(";

    (..., (out << (I == 0 ? "" : ", ") << std::get<I>(_tup)));
    std::cout << ")\n";
}


template<class...T>
std::ostream& operator<<(std::ostream& out, const std::tuple<T...>& tup) {

    printany(tup, std::make_index_sequence<sizeof...(T)>(), out);
    return out;
}

template<typename I, typename S>
class CSVParser {

    std::ifstream& data;
    int skip = 0;
    const char delim;
    const char screenSymbol;
    const bool screenModeEnabled;

public:

 CSVParser(std::ifstream& fd, int sk, bool sce = false, char dlm = ',', char scs = '"') :data(fd), skip(sk), screenModeEnabled(sce), delim(dlm), screenSymbol(scs) {}

    class iterator : public std::iterator <
        std::input_iterator_tag,   // iterator_category
        std::tuple<I, S>,                      // value_type
        long,                      // difference_type
        const std::tuple<I, S>*,               // pointer
        const std::tuple<I, S>&
    >// reference
    {
        //long num = FROM;
        std::ifstream& refdata;
        std::tuple<I, S>* p = nullptr;


        std::vector<std::string> vault;

        int posInsideVec = -1;

        CSVParser* pmaster = nullptr;

        int currentRow = 0;
        int currentCol = 0;

    public:


        explicit iterator(std::ifstream& d, CSVParser* pm) : refdata(d), pmaster(pm) {
            S str;
            std::getline(refdata, str);

            vault = splitByDelim(str);

            p = new std::tuple<I, S>(1, vault[0]);
            posInsideVec = 0;
        }
        explicit iterator(std::tuple<I, S>* ptr, std::ifstream& d, CSVParser* pm) : refdata(d), pmaster(pm) {
            p = ptr;
        }

        std::vector<std::string> splitByDelim(const std::string& s) {

            std::vector<std::string> cells;

            int lastPos = -1;
            for (int i = 0; i < s.size(); ++i) {

                // check around

                if (s[i] != pmaster->delim)continue;

                if (pmaster->screenModeEnabled && i > 0 && i < s.size() - 1 && s[i - 1] == pmaster->screenSymbol && s[i + 1] == pmaster->screenSymbol) {

                    continue;
                }

                // если экранирование только с одной стороны (левой), считаем за ошибку
                if (pmaster->screenModeEnabled && i > 0 && i < s.size() - 1 && s[i - 1] == pmaster->screenSymbol && s[i + 1] != pmaster->screenSymbol) {

                    throw std::exception((std::string("Wrong screen detected: Row = ") + std::to_string(currentRow + 1) + " Col: " + std::to_string(currentCol + 1)).c_str());
                }


                // substring
                cells.push_back(s.substr(lastPos + 1, i - lastPos - 1));
                lastPos = i;

            }

            cells.push_back(s.substr(lastPos + 1, s.size() - lastPos + 1));


            return cells;
        }



        //iterator& operator++() { num = TO >= FROM ? num + 1 : num - 1; return *this; }
        iterator& operator++() {

            //int curIndex = std::get<0>(*p);           


            if (posInsideVec >= vault.size() - 1) {

                S str;


                if (refdata.good() == false) {

                    throw std::exception("Problem with file!");

                }

                while (!refdata.eof()) {

                    std::getline(refdata, str);
                    if (str.size() > 0) {
                        break;
                    }
                }
                //std::cout << "s size = " << str.size() << std::endl;

                if (str.size() == 0 && refdata.eof()) { delete p; p = nullptr; return *this; }

                currentRow++;
                vault = splitByDelim(str);
                std::get<0>(*p)++;
                std::get<1>(*p) = vault[0];
                posInsideVec = 0;
                currentCol = 0;

            }
            else {
                std::get<0>(*p)++;
                std::get<1>(*p) = vault[++posInsideVec];
            }

            return *this;

        }
        //iterator operator++(int) { iterator retval = *this; ++(*this); return retval; }
        bool operator==(iterator other) const { return p == other.p; }
        bool operator!=(iterator other) const { return p != other.p; }
        std::tuple<I, S> operator*() const { return *p; }
    };
    iterator begin() { return iterator(data, this); }
    iterator end() { std::cout << "end() call" << std::endl;  return iterator(nullptr, data, this); }

};


int main()
{
    //auto b = std::make_tuple('a', 1, "OMG", 0.55f);

    //std::cout << b;

    std::ifstream file("data2.csv");

    const char* a = "hello, \n handsome!";
    std::cout << a << std::endl;
    CSVParser<int, std::string> parser(file, 0, true, ';');

    try {
        for (std::tuple<int, std::string> rs : parser) {
            std::cout << rs << std::endl;


            system("pause");
        }
    }
    catch (std::exception& ex) {

        std::cout << "Exception caught: " << ex.what() << std::endl;

    }




    system("pause");
    return 0;

}

