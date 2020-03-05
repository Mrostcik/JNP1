/*
 * Fibo.h
 *
 *  Created on: 19 lis 2019
 *      Author: szymon
 */

#ifndef FIBO_H_
#define FIBO_H_
#include<iostream>
#include<array>
#include<vector>
#define FIBULL 91
#define FIBULLINC 92

class Fibo {
private:
	std::vector<bool> fibBits;
	void normalize();
	void compress();
	void compensateSizes(int &size1, int size2);
public:
	Fibo();
	explicit Fibo(std::string s);
	explicit Fibo(const char* c);
	Fibo(unsigned long long n);
	Fibo(short n);
	Fibo(long n);
	Fibo(int n);
	Fibo(long long n);
	Fibo(unsigned short n);
	Fibo(unsigned long n);
	Fibo(unsigned int n);
	Fibo(bool b) = delete;
	Fibo(char c) = delete;
	Fibo(const Fibo& f);
	Fibo& operator= (const Fibo &f2);
	Fibo& operator+= (const Fibo &f2);
	Fibo& operator<<= (unsigned long long n);
	Fibo& operator^= (const Fibo &f);
	Fibo& operator|= (const Fibo &f);
	Fibo& operator&= (const Fibo &f);
	unsigned length() const;

	friend bool operator<(const Fibo& f1, const Fibo& f2);
	friend bool operator<=(const Fibo& f1, const Fibo& f2);
	friend bool operator>(const Fibo& f1, const Fibo& f2);
	friend bool operator>=(const Fibo& f1, const Fibo& f2);
	friend bool operator!=(const Fibo& f1, const Fibo& f2);
	friend bool operator==(const Fibo& f1, const Fibo& f2);
	friend std::ostream& operator<<(std::ostream& os, const Fibo& f);
	friend const Fibo operator| (const Fibo &f, const Fibo &other);
	friend const Fibo operator<< (const Fibo &f, unsigned long long);
	friend const Fibo operator^ (const Fibo &f, const Fibo &other);
	friend const Fibo operator& (const Fibo &f, const Fibo &other);
	friend const Fibo operator+ (const Fibo &f, const Fibo &other);
};

const Fibo Zero();
const Fibo One();

#endif /* FIBO_H_ */
