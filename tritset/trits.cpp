#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#define K (sizeof(unsigned int)*4)


typedef unsigned int uint;


/*
enum Trit{False, Unknown, True};
ЕЕЕ.
//резерв пам€ти дл€ хранени€ 1000 тритов
TritSet set(1000);
// length of internal array
size_t allocLength = set.capacity();
assert(allocLength >= 1000*2 / 8 / sizeof(uint) );
// 1000*2 - min bits count
// 1000*2 / 8 - min bytes count
// 1000*2 / 8 / sizeof(uint) - min uint[] size

//не выдел€ет никакой пам€ти
set[1000Т000Т000] = Unknown;
assert(allocLength == set.capacity());

// false, but no exception or memory allocation
if(set[2000Т000Т0`00]==True){}
assert(allocLength == set.capacity());

//выделение пам€ти
set[1000Т000Т000] = True;
assert(allocLength < set.capacity());
*/

enum class Trit {

    Unknown,
    False,
    True
};

char tritName(Trit t) {

    if (t == Trit::Unknown)return 'U';
    if (t == Trit::False)return 'F';
    if (t == Trit::True)return 'T';

    return '?';
}

class TritRef {

    uint* dataptr;
    uint offset;


public:
    //constructor just initializing members
    TritRef(uint* _dataptr, uint _offset) : dataptr(_dataptr), offset(_offset) {}



    //conversion to Trit
    operator Trit() const {
        //just like in the getter, but the bitmask is stored now locally
        
        uint a = *dataptr;

        a >>= offset * 2;
        a &= 3;

        return static_cast<Trit>(a);
    }

    //sets one single bit represented by mask to b
    TritRef& operator=(Trit b) {
        
        uint m = 3;
        uint a = static_cast<int>(b);
        m <<= offset * 2;
        *dataptr &= ~m;
        a <<= offset * 2;
        *dataptr |= a;

        return *this;
    }

    //TODO copy ctor., operator==, operator<
};

class TritSet {
public:
    std::vector<uint> data;
    uint ini_cap;
    


    TritSet(uint _cap) :ini_cap(_cap){
        uint real_cap = _cap / K;
        if (_cap % K != 0) {
            real_cap++;
        }
        data.reserve(real_cap);     
        for (int i = 0; i < real_cap; ++i) {
            data.push_back(0);
        }
    }

    size_t capacity() const {
        return data.capacity();
    }

    Trit get(size_t indexOfTrit) {

        if (indexOfTrit >= K * data.capacity()) {

            return Trit::Unknown;
        }

        size_t indexOfBlock = indexOfTrit / K;

        size_t offset = indexOfTrit % K;

        uint a = data[indexOfBlock];

        //[1001][1100][1011][1111] - theory
        //[1111][1011][1100][1001] - practice (x86)

        size_t mask = 3;

        mask <<= 2 * offset;
        // 0000000000000000011 -> 0011000000000000000
        //                        1101101010101010101 

        size_t b = a & mask;

        b >>= 2 * offset;

        if (b == 3) {
            std::cerr << "Bad trit found!" << std::endl;
            b--;
        }
       
        return static_cast<Trit>(b);
     }
    
    Trit operator[](size_t indexOfTrit) const{
        if (indexOfTrit >= K * data.capacity()) {

            return Trit::Unknown;
        }

        size_t indexOfBlock = indexOfTrit / K;

        size_t offset = indexOfTrit % K;

        uint a = data[indexOfBlock];

        //[1001][1100][1011][1111] - theory
        //[1111][1011][1100][1001] - practice (x86)

        size_t mask = 3;

        mask <<= 2 * offset;
        // 0000000000000000011 -> 0011000000000000000
        //                        1101101010101010101 

        size_t b = a & mask;

        b >>= 2 * offset;

        if (b == 3) {
            std::cerr << "Bad trit found!" << std::endl;
            b--;
        }

        return static_cast<Trit>(b);
    }
    void set(size_t indexOfTrit, Trit trit) {

        if (indexOfTrit >= K * data.capacity()) {

            if (trit == Trit::Unknown) {

                // nothing
                return;
            }
            else {
                uint new_cap = indexOfTrit/K;
                if (indexOfTrit % K != 0) {
                    new_cap++;
                }

                // [aaaaaaaaaa]
                data.reserve(new_cap);
                // [aaaaaaaaaa.............................................]
                while (data.size() < data.capacity()) {
                    data.push_back(0);
                }
                // [aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa]
            }
        }
        
        // enough capacity and size

        size_t indexOfBlock = indexOfTrit / K;

        size_t offset = indexOfTrit % K;

        uint a = data[indexOfBlock];
        std::cout << "Old a = " << a << std::endl;

        //[1001][1100][1011][1111] - theory
        //[1111][1011][1100][1001] - practice (x86)

        size_t mask = 3;
        mask <<= 2 * offset;
        mask = (~mask);

        a &= mask;
        std::cout << "Inter a = " << a << std::endl;
        mask = static_cast<uint>(trit);
        
        mask <<= 2 * offset;

        a |= mask;
        std::cout << "New a = " << a << std::endl;
        data[indexOfBlock] = a;
    }

    void shrink() {

        int k = data.capacity()-1;

        while (data[k] == 0) {
            k--;
        }

        if (k == -1) {

            // all Unknown

            data.clear();
            data.shrink_to_fit();
            data.reserve(ini_cap);
            std::fill(data.begin(), data.end(), 0);            
        }
        else {

            data.resize(k + 1);
            data.shrink_to_fit();
        }
    }

    TritRef operator[](size_t i) {
    

        if (i>= K * data.capacity()) {

                uint new_cap = (i+1) / K;
                if ((i+1) % K != 0) {
                    new_cap++;
                }

                // [aaaaaaaaaa]
                data.reserve(new_cap);
                // [aaaaaaaaaa.............................................]
                while (data.size() < data.capacity()) {
                    data.push_back(0);
                }
                // [aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa]
            
        }


        return TritRef(&data.at(i/K), i%K);   
    
    }

    friend std::ostream& operator<<(std::ostream&, TritSet&);

    static Trit log_and(Trit a, Trit b) {
        if (a == Trit::False || b == Trit::False) {
            return Trit::False;
        }

        if (a == Trit::Unknown || b == Trit::Unknown) {
            return Trit::Unknown;
        }

        return Trit::True;
    }

    static Trit log_or(Trit a, Trit b) {

        if (a == Trit::True || b == Trit::True) {
            return Trit::True;
        }

        if (a == Trit::Unknown || b == Trit::Unknown) {
            return Trit::Unknown;
        }

        return Trit::False;
    }

    static Trit log_not(Trit a) {

        if (a == Trit::True) return Trit::False;
        if (a == Trit::False) return Trit::True;
        return a;
    }


    TritSet operator&(const TritSet& b) const {

        uint c_cap = (data.capacity() < b.capacity()) ? data.capacity() : b.data.capacity();

        TritSet c(c_cap*K);

        for (int i = 0; i < c_cap*K; ++i) {

            Trit first = (*this)[i];
            Trit second = b[i];
            

            c[i] = log_and((*this)[i], b[i]);
        }

        return c;
    }

    TritSet operator|(const TritSet& b) const {

        uint c_cap = (data.capacity() < b.capacity()) ? data.capacity() : b.data.capacity();

        TritSet c(c_cap * K);

        for (int i = 0; i < c_cap * K; ++i) {

            Trit first = (*this)[i];
            Trit second = b[i];
            

            c[i] = log_or((*this)[i], b[i]);
        }

        return c;
    }

    TritSet operator!() const {

        TritSet result(data.capacity() * K);
        for (int i = 0; i < data.capacity() * K; ++i) {
            Trit t = (*this)[i];
            t = log_not(t);
            result[i] = t;
        }
        return result;
    }

    size_t cardinality(Trit t) const {

        size_t result = 0;

        for (size_t i = 0; i < data.capacity()*K; ++i) {

            if (t == (*this)[i]) {
                result++;
            }
        }

        return result;
    }


    std::unordered_map<Trit, int> cardinality()const {

        std::unordered_map<Trit, int> theMap;

        for (size_t i = 0; i < data.capacity() * K; ++i) {

            Trit t = (*this)[i];

            if (theMap.find(t) == theMap.end()) {
                theMap[t] = 0;
            }
            else {
                ++theMap[t];
            }
        }

        return theMap;
    }

};

std::ostream& operator<<(std::ostream& out, TritSet& ts) {

    out << "[";
    for (int i = 0; i < ts.capacity() * K; i++) {
        out << tritName(ts[i]);
    }
    out << "]" << std::endl;
    return out;
}



int main()
{
    uint ini_size = 100;

    TritSet A(ini_size);

    for (int i = 0; i < ini_size; ++i) {

        std::cout << tritName(A[i]);
    }
    std::cout << std::endl;

    //A.set(50, Trit::True);
    A[50] = Trit::True;

    std::cout << A;

    //A.set(51, Trit::False);
    A[51] = Trit::False;

    for (int i = 0; i < A.data.size(); ++i) {
        std::cout << A.data[i] << std::endl;
    }
    std::cout << std::endl;

    std::cout << A;

    std::cout << std::endl;

    for (int i = 0; i < ini_size; ++i)
        A[i] = Trit::True;
    
    std::cout << A;

    for (int i = 0; i < ini_size; ++i) {
        A[i] = Trit::False;
    }

    std::cout << A;

    for (int i = 0; i < ini_size; ++i) {

        if (i % 3 == 0) {
            A[i] = Trit::Unknown;
        }
        else if (i % 3 == 1) {
            A[i] = Trit::False;
        }
        else {
            A[i] = Trit::True;
        }
    }

    std::cout << A;

    TritSet B(50);

    TritSet C = A & B;


    std::cout << C;
    std::cout << C.data.size();

    TritSet D = A | B;

    std::cout << D;

    TritSet A2 = !A;

    std::cout << A2;

    auto m = A2.cardinality();
    for (auto elem : m) {
        std::cout << tritName(elem.first) << " -> " << elem.second << std::endl;
    }

    A[0] = Trit::False;

    auto m2 = A.cardinality();
    for (auto elem : m2) {
        std::cout << tritName(elem.first) << " -> " << elem.second << std::endl;
    }

    system("pause");
    return 0;
}
