#include<vector>
#include<algorithm>
#include<cassert>
#include<cstring>

#include "fibo.h"

void Fibo::normalize(){
	if(fibBits.size() < 2)
		return;

	if(fibBits[0] == true && fibBits[1] == true){
		reverse(fibBits.begin(), fibBits.end());
		fibBits.push_back(false);
		reverse(fibBits.begin(), fibBits.end());
	}

	for(unsigned i = 0; i < fibBits.size()-2; i++){
		if(fibBits[i+1] == true && fibBits[i+2] == true){
			fibBits[i] = true;
			fibBits[i+1] = false;
			fibBits[i+2] = false;
		}
	}

	for(unsigned i = fibBits.size()-1 ; i>=1; i--){
		if(fibBits[i] == true && fibBits[i-1] == true){
			fibBits[i-1] = false;
			fibBits[i] = false;
			if(i>1)
				fibBits[i-2] = true;
			else{
				reverse(fibBits.begin(), fibBits.end());
				fibBits.push_back(true);
				reverse(fibBits.begin(), fibBits.end());
			}
		}
	}
}

void Fibo::compress(){
	reverse(fibBits.begin(), fibBits.end());
	int size = fibBits.size()-1;
	while(size >= 0 && fibBits[size] == false){
		fibBits.pop_back();
		size--;
	}

	if(fibBits.size() == 0)
		fibBits.push_back(false);

	reverse(fibBits.begin(), fibBits.end());
}

void Fibo::compensateSizes(int &size1, int size2){
	if(size1 < size2){
		reverse(fibBits.begin(), fibBits.end());
		while(size1 < size2){
			fibBits.push_back(false);
			size1++;
		}
		reverse(fibBits.begin(), fibBits.end());
	}
}

Fibo::Fibo() {
	fibBits.push_back(false);
}

Fibo::Fibo(unsigned long long n){
	unsigned long long Fibonacci[FIBULLINC];
	Fibonacci[0] = 1;
	Fibonacci[1] = 2;
	for(int i = 2 ; i < FIBULLINC; i++){
		Fibonacci[i] = Fibonacci[i-1] + Fibonacci[i-2];
	}
	bool add = false;
	for(int i = FIBULL; i>=0; i-- ){
		if(Fibonacci[i] <= n){
			fibBits.push_back(true);
			n-=Fibonacci[i];
			add = true;
		}
		else if(add){
			fibBits.push_back(false);
		}
	}


	normalize();
	compress();
}

Fibo::Fibo(short n) : Fibo((unsigned long long) (n >= 0 ? n : 0)){
	assert(n >= 0);
}

Fibo::Fibo(long n) : Fibo((unsigned long long) (n >= 0 ? n : 0)){
	assert(n >= 0);
}

Fibo::Fibo(int n) : Fibo((unsigned long long) (n >= 0 ? n : 0)){
	assert(n >= 0);
}

Fibo::Fibo(long long n) : Fibo((unsigned long long) (n >= 0 ? n : 0)){
	assert(n >= 0);
}

Fibo::Fibo(unsigned short n) : Fibo((unsigned long long) n){}

Fibo::Fibo(unsigned long n) : Fibo((unsigned long long) n){}

Fibo::Fibo(unsigned n) : Fibo((unsigned long long) n){}

Fibo::Fibo(std::string s){
	assert(s.size() > 0);
	assert(s.size() == 1 || s[0] != '0');
	std::size_t legal = std::strspn(s.c_str(), "01");
	assert(legal == s.size());

	bool add = false;

	for(unsigned i = 0; i < s.size(); i++){
		if(s[i] == '0' && add){
			fibBits.push_back(false);
		}
		else if(s[i] == '1'){
			fibBits.push_back(true);
			add = true;
		}
	}

	normalize();
	compress();
}

Fibo::Fibo(const char* c) : Fibo((std::string) (c == nullptr ? "0" : c)){
	assert(c);
}

Fibo::Fibo(const Fibo &f){
	std::vector<bool> f2;
	for(unsigned i = 0; i < f.fibBits.size(); i++){
		f2.push_back(f.fibBits[i]);
	}

	fibBits = f2;
}

Fibo& Fibo::operator= (const Fibo &f2){
	if(this == &f2)
		return *this;

	this->fibBits.clear();
	for(unsigned i = 0; i < f2.fibBits.size(); i++)
		fibBits.push_back(f2.fibBits[i]);

	return *this;
}

Fibo& Fibo::operator+= (const Fibo &f2){
	int suma[4] = {0, 0, 0, 0};
	int size1 = fibBits.size(), size2 = f2.fibBits.size();
	compensateSizes(size1, size2);

	size1++;
	int diff = size1 - size2;
	reverse(fibBits.begin(), fibBits.end());
	fibBits.push_back(false);
	reverse(fibBits.begin(), fibBits.end());
	fibBits.push_back(false);
	fibBits.push_back(false);

	for(unsigned i = 0; i < fibBits.size(); i++){
		if(size2 < size1 || i >= fibBits.size() - 2){
			suma[3] = fibBits[i];
			size2++;
		}
		else{
			suma[3] = fibBits[i] + f2.fibBits[i-diff];
		}

		if(i>=3){
			if(suma[0] == 0 && suma[1] == 2 && suma[2] == 0){
				suma[0] = 1;
				suma[1] = 0;
				suma[2] = 0;
				suma[3]+= 1;
			}
			if(suma[0] == 0 && suma[1] == 3 && suma[2] == 0){
				suma[0] = 1;
				suma[1] = 1;
				suma[3]+= 1;
			}
			if(suma[0] == 0 && suma[1] == 2 && suma[2] == 1){
				suma[0] = 1;
				suma[1] = 1;
				suma[2] = 0;
			}
			if(suma[0] == 0 && suma[1] == 1 && suma[2] == 2){
				suma[0] = 1;
				suma[1] = 0;
				suma[2] = 1;
			}
			fibBits[i-3] = suma[0];
			fibBits[i-2] = suma[1];
			fibBits[i-1] = suma[2];
			fibBits[i] = suma[3];
		}

		suma[0] = suma[1];
		suma[1] = suma[2];
		suma[2] = suma[3];
	}

	fibBits.pop_back();
	if(fibBits[fibBits.size()-1] == true)
		fibBits[fibBits.size()-2] = true;

	fibBits.pop_back();

	normalize();
	compress();
	return *this;
}

Fibo& Fibo::operator<<= (unsigned long long n){
	for(unsigned i = 0; i < n; i++){
		fibBits.push_back(false);
	}

	return *this;

}

Fibo& Fibo::operator^= ( const Fibo &f){
	int size1 = fibBits.size(), size2 = f.fibBits.size();
	compensateSizes(size1, size2);

	int diff = size1-size2;
	for(int i = 0; i < size1; i++){
		if(size1 > size2){
			size2++;
		}
		else{
			fibBits[i] = fibBits[i]^f.fibBits[i-diff];
		}
	}

	compress();
	normalize();

	return *this;
}

Fibo& Fibo::operator|= (const Fibo &f){
	int size1 = fibBits.size(), size2 = f.fibBits.size();
	compensateSizes(size1, size2);

	int diff = size1-size2;
	for(int i = 0; i < size1; i++){
		if(size1 > size2){
			size2++;
		}
		else{
			fibBits[i] = fibBits[i]|f.fibBits[i-diff];
		}
	}

	compress();
	normalize();

	return *this;
}

Fibo& Fibo::operator&= (const Fibo &f){
	int size1 = fibBits.size(), size2 = f.fibBits.size();
	compensateSizes(size1, size2);

	int diff = size1-size2;
	for(int i = 0; i < size1; i++){
		if(size1 > size2){
			fibBits[i] = false;
			size2++;
		}
		else{
			fibBits[i] = fibBits[i]&f.fibBits[i-diff];
		}
	}

	compress();
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Fibo& f){
	for(unsigned i = 0; i < f.fibBits.size(); i++){
		os<<f.fibBits[i];
	}
	return os;
}

Fibo const operator+ (const Fibo &f, const Fibo &other){
	Fibo result = Fibo(f);
	result += other;
	return result;
}

Fibo const operator& (const Fibo &f, const Fibo &other){
	Fibo result = Fibo(f);
	result &= other;
	return result;
}

Fibo const operator^ (const Fibo &f, const Fibo &other){
	Fibo result = Fibo(f);
	result ^= other;
	return result;
}

Fibo const operator<< (const Fibo &f, unsigned long long other){
	Fibo result = Fibo(f);
	result <<= other;
	return result;
}

Fibo const operator| (const Fibo &f, const Fibo &other){
	Fibo result = Fibo(f);
	result |= other;
	return result;
}


unsigned Fibo::length() const{
	return fibBits.size();
}

bool operator<(const Fibo& f1, const Fibo& f2){
	if(f2.fibBits.size() > f1.fibBits.size())
		return true;
	if(f1.fibBits.size() > f2.fibBits.size())
		return false;

	return f1.fibBits < f2.fibBits;
}

bool operator==(const Fibo& f1, const Fibo& f2){
	return f1.fibBits == f2.fibBits;
}

bool operator<=(const Fibo &f1, const Fibo& f2){
	return (f1 < f2) || (f1 == f2);
}

bool operator>(const Fibo& f1, const Fibo& f2){
	return !(f1 <= f2);
}

bool operator>=(const Fibo& f1, const Fibo& f2){
	return !(f1 < f2);
}

bool operator!=(const Fibo& f1, const Fibo& f2){
	return !(f1 == f2);
}

const Fibo Zero(){
	return Fibo("0");
}

const Fibo One(){
	return Fibo("1");
}




