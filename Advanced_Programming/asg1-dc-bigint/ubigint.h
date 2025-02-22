// $Id: ubigint.h,v 1.5 2020-10-11 12:25:22-07 - - $

#ifndef __UBIGINT_H__
#define __UBIGINT_H__

#include <exception>
#include <iostream>
#include <vector>
#include <limits>
#include <utility>
using namespace std;

#include "debug.h"
#include "relops.h"

class ubigint {
    friend ostream& operator<< (ostream&, const ubigint&);
    private:

        using udigit_t = unsigned char;
        using ubigvalue_t = vector<udigit_t>;
        ubigvalue_t ubig_value;

    public:
        void multiply_by_2();
        void divide_by_2();

        ubigint() = default;
        ubigint(unsigned long);
        ubigint(const string&);

        ubigint operator+ (const ubigint&) const;
        ubigint operator- (const ubigint&) const;
        ubigint operator* (const ubigint&) const;
        ubigint operator/ (const ubigint&) const;
        ubigint operator% (const ubigint&) const;

        bool operator== (const ubigint&) const;
        bool operator<  (const ubigint&) const;
};

#endif

