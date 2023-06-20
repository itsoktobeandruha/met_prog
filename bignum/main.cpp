#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <optional>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <string>

using namespace std;

//fixed: 1. убрал перегрузку = ; 2. const добавлен везде, где нужен (наверное) ; 3. normalize
//		 4. to_string()
class bignum {
	vector<int> d; 
	const int digit_size = 3; 

	private:
		void normalize(); //fixed
		void normalize_vec(std::vector<int>& digits) const;
		bignum get_block(int offset, int length); //FIXED//отступ + длина этого блока
		bignum _plus(const bignum& arg) const;
		bignum _minus(const bignum&& arg) const;

	public:
		bignum(uint64_t n = 0);
		bignum(const string& numstring);
		bignum(const vector<int> digit); //FIXED//из вектора если дан вектор {0,1,2] digit = 210 (right to left) // 2 реализовать//done?????
		string to_string() const;
		bignum operator*(const bignum& arg) const;
		size_t size() const; 
		int operator[](int idx) const;
		int& operator[](int idx);
		friend ostream& operator <<(ostream& out, bignum& arg);
		friend bignum fastmult(const bignum& arg1, const bignum& arg2);
	
};

ostream& operator<<(ostream& out, bignum& arg)
{
	arg.normalize();
	out << arg.to_string() << endl;
	return out;

}

void bignum::normalize()
{
	normalize_vec(d);
	uint32_t i = d.size() - 1;
	while (i > 0 && d[i] == 0) {
		d.pop_back();
		i--;
	}
}

void bignum::normalize_vec(std::vector<int>& digits) const
{
	uint32_t base = static_cast<int>(std::pow(10, digit_size));

	for (auto iter = digits.begin(); iter != digits.end(); iter++) {

		if (*iter >= 0) 
		{
			int temp = *iter / base;
			*iter %= base;
			if (iter == digits.end() - 1) {
				if (temp) 
					digits.push_back(temp);
				break;
			}
			else {
				*(iter + 1) += temp;
			}
		}
		else 
		{
			int temp = *iter / base;
			*iter %= base;
			iter += base;
			*(iter + 1) -= temp;
		}
	}
}

bignum bignum::get_block(int offset, int length) //fixed
{
	return bignum(std::vector<int>(d.begin() + offset, d.begin() + offset + length));
}

bignum bignum::_plus(const bignum& arg) const
{
	bignum b = *this;
	for (int i = 0; i < b.size(); i++) {
		b[i] += arg[i];
	}
	return b;
}

bignum bignum::_minus(const bignum&& arg) const
{
	bignum b = *this;
	for (int i = 0; i < d.size(); i++) {
		b[i] -= arg[i];
	}
	return b;
}

bignum::bignum(uint64_t n) //вычисляем base теперь один раз fixed
{
	int base = static_cast<uint32_t>(pow(10, digit_size));
	while (n > 0) {
		uint32_t value = n % base;
		n /= base;
		d.push_back(value);
	}
}

bignum::bignum(const string& numstring) //проверить на апострофы и пробелы
{
	string s(digit_size, '0');
	int k = 0;
	for (int i = numstring.size() - 1; i >= 0; i--)
	{
		if (isdigit(numstring[i]))
		{
			s[digit_size - k - 1] = numstring[i];
			k++;
			if (k == digit_size || i == 0)
			{
				d.push_back(atoi(s.c_str()));
				s = string(digit_size, '0');
				k = 0;
			}
		}
	}
}

bignum::bignum(const vector<int> digit)
{
	d = digit;
}

string bignum::to_string()const
{
	string num_string;
	int d_size = digit_size;
	for_each(d.rbegin(), d.rend(), [&num_string, d_size](const auto& elem) {
			std::string elem_str = std::to_string(elem);
			num_string += std::string(d_size - elem_str.size(), '0') + elem_str + ' ';
		});
	return num_string;
}

bignum bignum::operator*(const bignum& arg) const
{
	std::vector<int> digits(2 * std::max(this->size(), arg.size()), 0);
	for (auto i = 0; i < d.size(); i++)
		for (auto j = 0; j < arg.size(); j++) {
			digits[i + j] += d[i] * arg[j];
			normalize_vec(digits);
		}
	bignum res(digits);
	res.normalize();
	return res;
}

size_t bignum::size() const
{
	return d.size();
}

int bignum::operator[](int idx) const
{
	return d[idx];
}

int& bignum::operator[](int idx)
{
	return d[idx];
}


bignum fastmult(const bignum& arg1, const bignum& arg2) {
	if ((arg1.size() < 10) || (arg2.size() < 10)) { //если число короткое то применять наивное умножение
		return arg1 * arg2;
	}// else - Karacuba
	bignum n1 = arg1;
	bignum n2 = arg2;
	int dlt = arg1.size() - arg2.size();
	int null_add = (int)abs(dlt);
	if (arg1.size() < arg2.size()) {
		n1.d.insert(n1.d.begin(), null_add, 0);
	}
	else { if (arg1.size() > arg2.size()) { n2.d.insert(n2.d.begin(), null_add, 0); } }
	if (n1.size() % 2 == 1) {
		null_add++;
		null_add++;
		n1.d.insert(n1.d.begin(), 1, 0);
		n2.d.insert(n2.d.begin(), 1, 0);
	}

	int size = n1.size() / 2;
	bignum a = n1.get_block(0, size);
	bignum b = n1.get_block(size, size);
	bignum c = n2.get_block(0, size);
	bignum d = n2.get_block(size, size);

	bignum e = b._plus(a);
	e.normalize();
	bignum f = d._plus(c);
	f.normalize();

	bignum Y = fastmult(a, c);
	bignum X = fastmult(b, d);
	bignum Z = fastmult(e, f);

	bignum tmp = Z._minus(X)._minus(Y);
	tmp.normalize();

	tmp.d.insert(tmp.d.begin(), size, 0);
	X.d.insert(X.d.begin(), 2 * size, 0);
	bignum res = X._plus(tmp)._plus(Y);
	if (null_add) res.d.erase(res.d.begin(), res.d.begin() + null_add);
	res.normalize();


	return res;
}

int main()
{
	bignum number(215213120130);
	bignum number2("32123434212345");
	vector<int> vector = {1,256,30000};
	bignum number3(vector);
	cout << number;
	cout << number2;
	cout << number3;

	return 0;
}