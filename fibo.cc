#include "fibo.h"

#include <vector>
#include <cassert>
#include <algorithm>

namespace {

  // Doprowadza wektor do postaci unormowanej
  void toRegulated(std::vector<bool>& v) {
    size_t length = v.size();

    for (size_t i = 2; i < length; i++) {
      if (v[i] == 0 && v[i-1] == 1 && v[i-2] == 1) {
        v[i] = 1;
        v[i-1] = 0;
        v[i-2] = 0;
      }
    }

    if (v.size() >= 2 && v[length-1] == 1 && v[length-2] == 1) {
      v[length-1] = 0;
      v[length-2] = 0;
      v.push_back(1);
      length++;
    }

    if (length >= 3) {
      for (size_t i = length - 1; i >= 2; i--) {
        if (v[i] == 0 && v[i - 1] == 1 && v[i - 2] == 1) {
          v[i] = 1;
          v[i - 1] = 0;
          v[i - 2] = 0;
        }
      }
    }
  }

  // Wynikiem jest zaktualizowany wektor odpowiadający takiemu, w którym
  // na pozycji i jest 2. Po wykonaniu funkcji, v[0...i] jest nadal unormowany.
  void fix2(std::vector<bool>& v, size_t i) {
    v[i] = false;

    if (i + 1 >= v.size()) {
      v.push_back(1);
    } else {
      v[i + 1] = true;
    }

    if (i >= 2) {
      if (v[i - 2] == false) {
        v[i - 2] = true;

        if (i > 2 and v[i - 3] == true) {
          v[i - 3] = false;
          v[i - 2] = false;
          v[i - 1] = true;
        }

      } else {
        fix2(v, i - 2);
      }

    } else if (i == 1) {
      if (v[i - 1] == false) {
        v[i - 1] = true;
      } else {
        fix2(v, i - 1);
      }
    }
  }

  // Funkcje vector- Add/And/Or/Xor/Shift modyfikują pierwszy argument.

  void vectorAdd(std::vector<bool>& v1, const std::vector<bool>& v2) {
    for (size_t i = v1.size(); i < v2.size(); i++) {
      v1.push_back(0);
    }

    size_t i = v2.size();

    while (true) {
      i--;

      if (v2[i] == true) {
        if (v1[i] == false) {
          v1[i] = true;
        } else {
          fix2(v1, i);
        }
      }

      if (i == 0) {
        break;
      }
    }

    toRegulated(v1);
  }

  void vectorAnd(std::vector<bool>& v1, const std::vector<bool>& v2) {
    size_t length = std::min(v1.size(), v2.size());
    size_t i = 0;

    while (i < length) {
      v1[i] = ((v1[i] == 1 && v2[i] == 1));
      i++;
    }
    while (i < v1.size()) {
      v1[i] = 0;
      i++;
    }

    i--;
    while (v1[i] == 0 && i > 0) {
      v1.pop_back();
      i--;
    }

    toRegulated(v1);
  }

  void vectorOr(std::vector<bool>& v1, const std::vector<bool>& v2) {
    size_t length = std::min(v1.size(), v2.size());
    size_t i = 0;

    while (i < length) {
      v1[i] = ((v1[i] == 1 || v2[i] == 1));
      i++;
    }

    while (i < v2.size()) {
      v1.push_back((v2[i] == 1));
      i++;
    }

    toRegulated(v1);
  }

  void vectorXor(std::vector<bool>& v1, const std::vector<bool>& v2) {
    size_t length = std::min(v1.size(), v2.size());
    size_t i = 0;

    while (i < length) {
      v1[i] = (((v1[i] == 1 && v2[i] == 0) || (v1[i] == 0 && v2[i] == 1)));
      i++;
    }

    while (i < v2.size()) {
      v1.push_back((v2[i] == 1));
      i++;
    }

    i = std::max(v1.size(), v2.size()) - 1;
    while (v1[i] == 0 && i > 0) {
      v1.pop_back();
      i--;
    }

    toRegulated(v1);
  }

  void vectorShift(std::vector<bool>& v1, int n) {
    std::vector<bool> v;

    for (int i=0; i<n; i++) {
      v.push_back(0);
    }

    for (size_t i=0; i<v1.size(); i++) {
      v.push_back(v1[i]);
    }

    v1 = std::move(v);
    toRegulated(v1);
  }

  std::vector<bool> generateFibits(unsigned long long n) {
    std::vector<bool> v;
    unsigned long long f1=2, f2=1, old;

    while (f1 < n) {
      old = f1;
      f1 = f1 + f2;
      f2 = old;
    }

    if (f1 > n) {
      old = f1;
      f1 = f2;
      f2 = old - f2;
    }

    while (n > 0 || f2 > 0) {
      if (f1 <= n) {
        v.push_back(1);
        n -= f1;
      } else {
        v.push_back(0);
      }

      old = f1;
      f1 = f2;
      f2 = old - f2;
    }

    std::reverse(v.begin(), v.end());

    return v;
  }

  std::vector<bool> generateFibits(const std::string& str) {
    assert((str.size() > 1 and str[0] == '1') or (str.size() == 1 and str[0] == '0'));

    for (size_t i = 1; i < str.size(); i++) {
      assert(str[i] == '0' or str[i] == '1');
    }

    std::vector<bool> v;

    size_t i = str.size();
    while (true) {
      i--;
      v.push_back((str[i] == '1'));

      if (i == 0) {
        break;
      }
    }

    toRegulated(v);
    return v;
  }

  bool isSmaller(const std::vector<bool>& v1, const std::vector<bool>& v2) {
    if (v1.size() < v2.size()) {
      return true;
    }

    if (v1.size() > v2.size()) {
      return false;
    }

    int length = v1.size();

    for (int i = length - 1; i >= 0; i--) {
      if (v1[i] == 0 and v2[i] == 1) {
        return true;
      }

      if (v1[i] == 1 and v2[i] == 0) {
        return false;
      }
    }

    return false;
  }

  char const* valid(char const* arg){
    if (arg == NULL) {
      return "";
    } else {
      return arg;
    }
  }

  std::vector<bool> validateSigned(long long n){
    assert(n >= 0);
    return generateFibits((unsigned long long)n);
  }

}

// Konstruktor bezargumentowy - tworzy liczbę 0
Fibo::Fibo() {
  fibits.push_back(0);
}

Fibo::Fibo(const std::string& str) : fibits(generateFibits(str)) {}

// Konstruktor napisu w postaci "..." wywołuje konstruktor ze stringa
Fibo::Fibo(const char* str) : Fibo(std::string(valid(str))) {}


Fibo::Fibo(short n) : fibits(validateSigned((long long)n)) {}
Fibo::Fibo(int n) : fibits(validateSigned((long long)n)) {}
Fibo::Fibo(long long n) : fibits(validateSigned(n)) {}

Fibo::Fibo(unsigned int n) : Fibo((unsigned long long)n) {}
Fibo::Fibo(unsigned long n) : Fibo((unsigned long long)n) {}
Fibo::Fibo(unsigned long long n) : fibits(generateFibits(n)) {}

// Konstruktor kopiujący
Fibo::Fibo(const Fibo& other) : fibits(other.fibits) {}

// Konstruktor przenoszący
Fibo::Fibo(Fibo&& other) : fibits(std::move(other.fibits)) {}

// Przypisanie kopiujące
Fibo& Fibo::operator=(const Fibo& other) {
  if (this != &other) fibits = other.fibits;
  return *this;
}

// Przypisanie przenoszące
Fibo& Fibo::operator=(Fibo&& other) {
  fibits = std::move(other.fibits);
  return *this;
}

// Operatory modyfikujące:

Fibo& Fibo::operator+=(const Fibo& other) {
  if (this == &other) {
    const std::vector<bool> copy = other.fibits;
    vectorAdd(fibits, copy);
  } else {
    vectorAdd(fibits, other.fibits);
  }

  return *this;
}

Fibo& Fibo::operator&=(const Fibo& other) {
  if (this == &other) {
    const std::vector<bool> copy = other.fibits;
    vectorAnd(fibits, copy);
  } else {
    vectorAnd(fibits, other.fibits);
  }

  return *this;
}

Fibo& Fibo::operator|=(const Fibo& other) {
  if (this == &other) {
    const std::vector<bool> copy = other.fibits;
    vectorOr(fibits, copy);
  } else {
    vectorOr(fibits, other.fibits);
  }

  return *this;
}

Fibo& Fibo::operator^=(const Fibo& other) {
  if (this == &other) {
    const std::vector<bool> copy = other.fibits;
    vectorXor(fibits, copy);
  } else {
    vectorXor(fibits, other.fibits);
  }

  return *this;
}

Fibo& Fibo::operator<<=(int n) {
  vectorShift(fibits, n);
  return *this;
}

// Operator zwraca nowy obiekt równy *this << n
const Fibo Fibo::operator<<(int n) const {
  Fibo copy = *this;
  return copy <<= n;
}

size_t Fibo::length() const {
    return fibits.size();
}

// Operatory wykonujące działanie / porównanie na obiektach i zwracające nowy obiekt / bool.
// Nie są metodami, ponieważ element po lewej stronie działania nie musi być obiektem Fibo.

const Fibo operator+(Fibo first, const Fibo& second) {
  return first += second;
}

const Fibo operator&(Fibo first, const Fibo& second) {
  return first &= second;
}

const Fibo operator|(Fibo first, const Fibo& second) {
  return first |= second;
}

const Fibo operator^(Fibo first, const Fibo& second) {
  return first ^= second;
}


bool operator==(const Fibo& first, const Fibo& second) {
  return first.fibits == second.fibits;
}

bool operator<(const Fibo& first, const Fibo& second) {
  return isSmaller(first.fibits, second.fibits);
}

bool operator>(const Fibo& first, const Fibo& second) {
  return !(first < second) and !(first == second);
}

bool operator<=(const Fibo& first, const Fibo& second) {
  return first < second or first == second;
}

bool operator>=(const Fibo& first, const Fibo& second) {
  return first > second or first == second;
}

bool operator!=(const Fibo& first, const Fibo& second) {
  return !(first == second);
}


std::ostream& operator<<(std::ostream& os, const Fibo& that) {
  for (size_t i = 0; i < that.length(); i++) {
    if (that.fibits[that.length() - i - 1]) {
      os << '1';
    } else {
      os << '0';
    }
  }

  return os;
}


const Fibo Zero() {
  return Fibo(0);
}

const Fibo One() {
  return Fibo(1);
}
