#ifndef FIBO_H
#define FIBO_H

#include <string>
#include <ostream>
#include <vector>

class Fibo {
  public:
    Fibo(bool) = delete;
    Fibo(char) = delete;

    Fibo();

    explicit Fibo(const std::string& str);
    explicit Fibo(const char* str);

    Fibo(int n);
    Fibo(unsigned int n);
    Fibo(unsigned long n);
    Fibo(unsigned long long n);

    Fibo(const Fibo& other);
    Fibo(Fibo&& other);

    Fibo& operator=(const Fibo& other);
    Fibo& operator=(Fibo&& other);

    Fibo& operator+=(const Fibo& other);
    Fibo& operator&=(const Fibo& other);
    Fibo& operator|=(const Fibo& other);
    Fibo& operator^=(const Fibo& other);
    Fibo& operator<<=(int n);
    const Fibo operator<<(int n) const;

    size_t length() const;
    const std::vector<bool>& getFibits() const;

  private:
    std::vector<bool> fibits;

  friend bool operator==(const Fibo& first, const Fibo& second);
  friend bool operator<(const Fibo& first, const Fibo& second);
  friend std::ostream& operator<<(std::ostream& os, const Fibo& that);
};

const Fibo operator+(Fibo first, const Fibo& second);
const Fibo operator&(Fibo first, const Fibo& second);
const Fibo operator|(Fibo first, const Fibo& second);
const Fibo operator^(Fibo first, const Fibo& second);

bool operator==(const Fibo& first, const Fibo& second);
bool operator<(const Fibo& first, const Fibo& second);
bool operator>(const Fibo& first, const Fibo& second);
bool operator!=(const Fibo& first, const Fibo& second);
bool operator<=(const Fibo& first, const Fibo& second);
bool operator>=(const Fibo& first, const Fibo& second);

std::ostream& operator<<(std::ostream& os, const Fibo& that);

const Fibo Zero();
const Fibo One();

#endif
