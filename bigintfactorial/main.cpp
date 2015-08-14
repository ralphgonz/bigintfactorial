#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

class BigInteger {
	vector<long> parts;
	// each part can hold a number up to 9999, so product is less than 2^31
	static const long NUM_PLACES = 4;
	static const long MAX_PART = pow(10, NUM_PLACES);

public:
	BigInteger(const long);
	BigInteger(const BigInteger&);
	void addPart(const long value, const long partNo);
	void allocateZeros(const long partNo);
    friend ostream& operator<<(ostream&, const BigInteger&);
    friend BigInteger operator*(const BigInteger&, const BigInteger&);
};

BigInteger::BigInteger(long n) {
	parts.push_back(n);
}

BigInteger::BigInteger(const BigInteger& b) {
	for (auto i=b.parts.begin() ; i != b.parts.end() ; ++i) {
		parts.push_back(*i);
	}
}

void BigInteger::addPart(long value, long partNo) {
	allocateZeros(partNo);
	value += parts.at(partNo);
	// possibly carry overage
	long carry = value / MAX_PART; // truncate
	long remainder = value - carry * MAX_PART;
	parts.at(partNo) = remainder;
	if (carry > 0) {
		addPart(carry, partNo + 1);
	}
}

void BigInteger::allocateZeros(const long partNo) {
	if (partNo >= (long)parts.size()) {
		parts.resize(partNo + 1);
	}
	for (long i=parts.size() ; i<=partNo ; ++i) {
		parts.at(i) = 0;
	}
}

BigInteger operator* (const BigInteger& a, const BigInteger& b) {
	// multiple parts independently, then add them up, carrying into higher-level parts
	BigInteger product(0);
	for (long i=0 ; i<(long)a.parts.size() ; ++i) {
		for (long j=0 ; j<(long)b.parts.size() ; ++j) {
			product.addPart(a.parts[i] * b.parts[j], i + j);
		}
	}

	return product;
}
ostream &operator<<(ostream &os, BigInteger const &b) {
	// Don't fill first part:
	auto i = b.parts.rbegin();
	if (i != b.parts.rend()) {
		os << *i;
		++i;
		for ( ; i != b.parts.rend() ; ++i) {
			os << std::fixed << std::setw(BigInteger::NUM_PLACES) << std::setfill('0') << *i;
		}
	}
    return os;
}

BigInteger factorial(long n) {
    return BigInteger(n < 2 ? 1 : n * factorial(n-1));
}

int main() {
    int n;
    cin >> n;
    BigInteger result = factorial(n);
    cout << result << endl;
    return 0;
}
