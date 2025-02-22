// $Id: bigint.cpp,v 1.3 2020-10-11 12:47:51-07 - - $

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "bigint.h"
#include "debug.h"
#include "relops.h"

bigint::bigint (long that): uvalue (that), is_negative (that < 0) {
   DEBUGF ('~', this << " -> " << uvalue)
}

bigint::bigint (const ubigint& uvalue, bool is_negative):
                uvalue(uvalue), is_negative(is_negative) {
}

bigint::bigint (const string& that) {
   is_negative = that.size() > 0 and that[0] == '_';
   uvalue = ubigint (that.substr (is_negative ? 1 : 0));
}

bigint bigint::operator+ () const {
    return *this;
}

bigint bigint::operator- () const {
    return {uvalue, not is_negative};
}

bigint bigint::operator+ (const bigint& that) const {
    bigint result;
    if (this->is_negative == that.is_negative) {
        result.uvalue = uvalue + that.uvalue;
        result.is_negative = this->is_negative;
    }
    else {
        if (this->uvalue > that.uvalue) {
            result.uvalue = this->uvalue - that.uvalue;
            result.is_negative = this->is_negative;
        }
        else if (this->uvalue < that.uvalue) {
            result.uvalue = that.uvalue - this->uvalue;
            result.is_negative = that.is_negative;
        }
        else {
            result.uvalue = this->uvalue - that.uvalue;
            result.is_negative = false;
        }
    }
    return result;
}

bigint bigint::operator- (const bigint& that) const {
    bigint result;
    if (this->is_negative == that.is_negative) {
        if (this->uvalue > that.uvalue) {
            result.uvalue = this->uvalue - that.uvalue;
            result.is_negative = this->is_negative;
        }
        else if (this->uvalue < that.uvalue) {
            result.uvalue = that.uvalue - this->uvalue;
            result.is_negative = !(that.is_negative);
        }
        else if (this->uvalue == that.uvalue) {
            result.uvalue = that.uvalue - this->uvalue;
            result.is_negative = false;
        }
    }
    else {
        result.uvalue = uvalue + that.uvalue;
        result.is_negative = this->is_negative;
    }
    return result;
}


bigint bigint::operator* (const bigint& that) const {
    bigint result;
    if (this->is_negative == that.is_negative) {
        result.uvalue = uvalue * that.uvalue;
        result.is_negative = false;
    }
    else {
        result.uvalue = uvalue * that.uvalue;
        result.is_negative = true;
    }
    return result;
}

bigint bigint::operator/ (const bigint& that) const {
    bigint result;
    if (this->is_negative == that.is_negative) {
        result.uvalue = uvalue / that.uvalue;
        result.is_negative = false;
    }
    else {
        result.uvalue = uvalue / that.uvalue;
        result.is_negative = true;
    }
    return result;
}

bigint bigint::operator% (const bigint& that) const {
    bigint result;
    if (this->is_negative == that.is_negative) {
        result.uvalue = uvalue % that.uvalue;
        result.is_negative = false;
    }
    else {
        result.uvalue = uvalue % that.uvalue;
        result.is_negative = true;
    }
    return result;
}

bool bigint::operator== (const bigint& that) const {
    if (is_negative != that.is_negative) {
        return false;
    }
    else {
        return uvalue == that.uvalue;
    }
}

bool bigint::operator< (const bigint& that) const {
    if (is_negative != that.is_negative) {
        return is_negative;
    }
    if (is_negative) {
        return !(uvalue < that.uvalue) and !(uvalue == that.uvalue);
    }
    return uvalue < that.uvalue;
}

ostream& operator<< (ostream& out, const bigint& that) {
    return out << (that.is_negative ? "-" : "") << that.uvalue;
}

