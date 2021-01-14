// $Id: ubigint.cpp,v 1.10 2020-10-11 12:25:22-07 - - $

#include <cctype>
#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

#include "ubigint.h"
#include "debug.h"

ubigint::ubigint(unsigned long that) {
    while (that > 0) {
        ubig_value.push_back(that % 10);
        that = that / 10;
    }

    DEBUGF('~', this << " -> " << that)
}

ubigint::ubigint(const string& that) {
    for (char digit : that) {
        ubig_value.push_back(digit - '0');
    }

    reverse(ubig_value.begin(), ubig_value.end());
}

ubigint ubigint::operator+ (const ubigint& that) const {
    ubigint bigInt;

    unsigned int loopLength;
    unsigned int pointer = 0;
    int addValue = 0;
    int carry = 0;
    int bigger = 0;

    if (this->ubig_value.size() > that.ubig_value.size()) {
        loopLength = that.ubig_value.size();
        bigger = 1;
    }
    else if (this->ubig_value.size() < that.ubig_value.size()) {
        loopLength = this->ubig_value.size();
        bigger = 2;
    }
    else {
        loopLength = that.ubig_value.size();
        bigger = 0;
    }

    if (ubig_value.size() == 0) {
        for (unsigned int i = 0; i < that.ubig_value.size(); i++) {
            bigInt.ubig_value.push_back(that.ubig_value.at(i));
        }
        return bigInt;
    }
    else if (that.ubig_value.size() == 0) {
        for (unsigned int i = 0; i < ubig_value.size(); i++) {
            bigInt.ubig_value.push_back(ubig_value.at(i));
        }
        return bigInt;
    }

    while (pointer < loopLength || carry > 0) {
        if (pointer < ubig_value.size()) {
            addValue = addValue + ubig_value.at(pointer);
        }
        if (pointer < that.ubig_value.size()) {
            addValue = addValue + that.ubig_value.at(pointer);
        }
        addValue = addValue + carry;
        carry = 0;

        if (addValue >= 10) {
            carry = 1;
            addValue = addValue % 10;
        }


        bigInt.ubig_value.push_back(addValue);
        addValue = 0;
        pointer++;
    }

    if (bigger == 1) {
        while (pointer < ubig_value.size()) {
            bigInt.ubig_value.push_back(ubig_value.at(pointer));
            pointer++;
        }
    }
    else if (bigger == 2) {
        while (pointer < that.ubig_value.size()) {
            bigInt.ubig_value.push_back(that.ubig_value.at(pointer));
            pointer++;
        }
    }

    while (bigInt.ubig_value.size() > 0 
        && bigInt.ubig_value.back() == 0) {
        bigInt.ubig_value.pop_back();
    }
    return bigInt;
}

ubigint ubigint::operator- (const ubigint& that) const {
    if (*this < that) {
        throw domain_error("ubigint::operator-(a<b)");
    }

    ubigint bigInt;

    unsigned int pointer = 0;
    int borrowNum = 0;
    int tempUvalue = 0;

    while (pointer < that.ubig_value.size()) {
        tempUvalue = ubig_value.at(pointer) - borrowNum;
        borrowNum = 0;

        if (tempUvalue < that.ubig_value.at(pointer)) {
            tempUvalue = tempUvalue + 10;
            borrowNum = 1;
        }

        bigInt.ubig_value.push_back(tempUvalue 
            - that.ubig_value.at(pointer));
        pointer++;
    }

    while (pointer < ubig_value.size()) {
        if (borrowNum > 0) {
            bigInt.ubig_value.push_back(ubig_value.at(pointer) - 1);
            borrowNum = 0;
        }
        else {
            bigInt.ubig_value.push_back(ubig_value.at(pointer));
        }
        pointer++;
    }

    while (bigInt.ubig_value.size() > 0 
        && bigInt.ubig_value.back() == 0) {
        bigInt.ubig_value.pop_back();
    }

    return bigInt;
}

ubigint ubigint::operator* (const ubigint& that) const {
    ubigint bigInt;

    for (unsigned int i = 0; i < ubig_value.size(); i++) {
        ubigint temp;
        int multValue = 0;
        int carry = 0;
        for (unsigned int j = 0; j < that.ubig_value.size(); j++) {
            multValue = (ubig_value.at(i) 
                * that.ubig_value.at(j)) + carry;
            carry = 0;
            if (multValue >= 10) {
                carry = multValue / 10;
                multValue = multValue % 10;
            }

            temp.ubig_value.push_back(multValue);
        }

        if (carry > 0) {
            temp.ubig_value.push_back(carry);
            carry = 0;
        }

        unsigned int offset = 0;
        while (offset < i) {
            temp.ubig_value.insert(temp.ubig_value.begin(), 0);
            offset++;
        }

        bigInt = bigInt + temp;
        temp.ubig_value.clear();
    }

    while (bigInt.ubig_value.size() > 0 
        && bigInt.ubig_value.back() == 0) {
        bigInt.ubig_value.pop_back();
    }

    return bigInt;
}

void ubigint::multiply_by_2() {
   int carry = 0;
   for (unsigned int i = 0; i < ubig_value.size(); i++) {
       int multValue = (ubig_value.at(i) * 2) + carry;
       carry = 0;

       if (multValue >= 10) {
           carry = multValue / 10;
           multValue = multValue % 10;
       }

       ubig_value.at(i) = multValue;
   }

   if (carry > 0) {
       ubig_value.push_back(carry);
   }

   while (ubig_value.size() > 0 && ubig_value.back() == 0) {
       ubig_value.pop_back();
   }
}

void ubigint::divide_by_2() {
    int remainder = 0;

    for (int i = ubig_value.size() - 1; i >= 0; i--) {
        int digit = ubig_value.at(i);    

        digit = (digit + remainder) / 2;

        remainder = (ubig_value.at(i) % 2) * 10;

        ubig_value.at(i) = digit;
    }

    while (ubig_value.size() > 0 && ubig_value.back() == 0) {
        ubig_value.pop_back();
    }
}

struct quo_rem { ubigint quotient; ubigint remainder; };
quo_rem udivide(const ubigint& dividend, const ubigint& divisor_) {
    static const ubigint zero(0);
    ubigint divisor = divisor_;
    if (divisor == zero) throw domain_error("ubigint::divide: by 0");

    ubigint power_of_2 = ubigint(1);
    ubigint quotient = ubigint(0);
    ubigint remainder = dividend;


    while (divisor < dividend) {
        divisor.multiply_by_2();
        power_of_2.multiply_by_2();
    }

    while (power_of_2 > zero) {
        if ((divisor < remainder) || (divisor == remainder)) {
            remainder = remainder - divisor;
            quotient = quotient + power_of_2;
        }
        divisor.divide_by_2();
        power_of_2.divide_by_2();
    }

    return { .quotient = quotient, .remainder = remainder };
}

ubigint ubigint::operator/ (const ubigint& that) const {
    return udivide(*this, that).quotient;
}

ubigint ubigint::operator% (const ubigint& that) const {
    return udivide(*this, that).remainder;
}

bool ubigint::operator== (const ubigint& that) const {
    if (ubig_value.size() != that.ubig_value.size()) {
        return 0;
    }
    else {
        for (unsigned int i = 0; i < ubig_value.size(); i++) {
            if (ubig_value.at(i) != that.ubig_value.at(i)) {
                return 0;
            }
        }
        return 1;
    }
}

bool ubigint::operator< (const ubigint& that) const {
    if (*this == that) {
        return 0;
    }
    if (ubig_value.size() < that.ubig_value.size()) {
        return 1;
    }
    if (ubig_value.size() == that.ubig_value.size()) {
        if (ubig_value.size() == 0) {
            return 0;
        }
        for (int i = ubig_value.size() - 1; i >= 0; i--) {
            if (ubig_value.at(i) > that.ubig_value.at(i)) {
                return 0;
            }
            if (that.ubig_value.at(i) > ubig_value.at(i)) {
                return 1;
            }
        }
        return 1;
    }
    return 0;
}

ostream& operator<< (ostream& out, const ubigint& that) {
    int outLength = 70;
    if (that.ubig_value.size() > 0) {
        int charCount = 0;
        for (int i = that.ubig_value.size() - 1; i >= 0; i--) {
            if (charCount % (outLength - 1) == 0 && charCount != 0) {
                out << "\\" << endl;
            }
            out << static_cast<char>(that.ubig_value.at(i) + '0');
            charCount++;
        }
    }
    else {
        out << '0';
    }

    return out;
}

