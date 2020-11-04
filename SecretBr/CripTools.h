#pragma once
#pragma warning(disable:4996)
// CriptTools.h
#ifndef CRIPT_TOOLS_H
#define CRIPT_TOOLS_H
#pragma warning(disable: 4996)
#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
#include <iomanip>
#include <bitset> 
#include <vector>
#include <algorithm>
#include <string>
#include <thread>
using namespace std;

/*variabile globale*/
/*============================== VARIABILE GLOBALE (pentru alfabet) ======================================*/
char caracter[100] = { 0 };
int N = 0;
/*============================== SFARSIT VARIABILE GLOBALE ======================================*/

/*============================== FUNCTII UTILE ======================================*/

/*calculul celui mai mare divizor comun*/
template<typename MyInt>
MyInt cmmdc(MyInt x, MyInt y) {
	MyInt rest;
	do {
		rest = x % y;
		x = y;
		y = rest;
	} while (rest != 0);
	return x;
}
/*extindem operatorul modulo (%) si pentru numere negative*/
template<typename MyInt>
int modulo(MyInt k, MyInt n) {
	if (k < 0)k = n - (-k) % n;
	if (k >= n) return k % n;
	return k;
}

/*calculeaza inversul unui numar in Zn. Va returna -1 daca numarul nu este inversabil*/
template<typename MyInt>
MyInt invers(MyInt a, MyInt n) {
	MyInt q, r, x0 = 1, x1 = 0, copy_n = n;
	a = modulo(a, n);
	while (n != 0)
	{
		r = n;
		q = a / n;
		n = a % n;
		a = r;

		r = x1;
		x1 = x0 - q * x1;
		x0 = r;
	}
	if (a == 1)
		return modulo(x0, copy_n);
	return -1;
}

/*calculeaza a la puterea b modulo c*/
template<typename MyInt>
MyInt a_la_b_mod_c(MyInt a, MyInt b, MyInt c) {
	int p = 1;
	a %= c;
	while (b > 0)
	{
		if (b % 2)
			p = (p * a) % c;
		a = (a * a) % c;
		b /= 2;
	}
	return p;
}

/*testeaza daca numarul este prim*/
template<typename MyInt>
int prim(MyInt x) {
	int i;
	if ((x == 1) || (x == 2)) return 1;
	if (x % 2 == 0) return 0;
	for (i = 3; i <= sqrt((double)x); i += 2)
		if (x % i == 0) return 0;
	return 1;
}

/*da un numar prim diferit de nr. situat intre min si max.*/
template<typename MyInt>
int da_prim(MyInt min, MyInt max, MyInt nr) {
	int nrIncercari = 10;
	MyInt k;
	if (min < 0 || max<0 || min>max)return-1;
	if (min == max)
		if (prim(min))
			return min;
		else
			return -1;
	for (int i = 0; i < nrIncercari; i++) {
		k = rand();
		k = min + k % (max - min);
		while (!prim(k) || k == nr)k++;
		if (k <= max)return k;
	}
	k = min;
	while (!prim(k) || k == nr)k++;
	if (k <= max)return k;
	return -1;
}

/*calculeaza valoarea polinomului cu coeficientii coef in punctul x, modulo p.*/
template<typename MyInt>
int val_pol(MyInt* coef, int grad, MyInt x, MyInt p) {
	MyInt S = 0;
	for (int i = grad; i >= 0; i--)
		S = modulo((coef[i] + S * x), p);
	return S;
}

/*testeaza daca x este patrat perfect*/
template<typename MyInt>
long este_patrat_perfect(MyInt x) {
	MyInt temp = (MyInt)sqrt((double)x);
	if (temp * temp == x)return temp;
	return 0;
}

/*factorizeaza numarul n (determina p si q prime astfel incat n=pq)*/
template<typename MyInt>
void factorizare(MyInt n, MyInt& p, MyInt& q) {
	MyInt s_patrat, t;
	t = (MyInt)(sqrt((double)n) + 1);
	s_patrat = t * t - n;
	while (!este_patrat_perfect(s_patrat) && (t <= n)) {
		t++;
		s_patrat = t * t - n;
	}
	if (este_patrat_perfect(s_patrat) >= 0) {
		s_patrat = (MyInt)sqrt((double)s_patrat);
		p = t + s_patrat;
		q = t - s_patrat;
	}
	else {
		p = -1; q = -1;
	}
}

/*Calculeaza x astfel incat g^x=y mod p*/
template<typename MyInt>
MyInt log_d(MyInt g, MyInt y, MyInt p) {
	int i, j, * v, a, b;
	a = (MyInt)sqrt((double)p);
	v = new MyInt[a + 1];
	v[0] = 1;
	v[1] = a_la_b_mod_c(g, a, p);

	for (i = 2; i <= a; i++)
		v[i] = (v[i - 1] * v[1]) % p;
	b = modulo(y, p);
	for (i = 0; i < a; i++) {
		for (j = 0; j <= a; j++)
			if (b == v[j])return modulo(j * a - i, p - 1);
		b = (b * g) % p;
	}
	return 0;
}

/*citeste caracterele alfabetului precum si numarul de caractere din alfabet*/
void citeste_alfabet() {
	N = 0;
	ifstream in("alfabet.txt");
	if (!in.good())
		perror("Fisier inexistent");
	char c;
	while (in >> noskipws >> c) {
		caracter[N] = c;
		N++;
	}
	if (N == 0)
		cout << "Alfabetul dat are 0 caractere" << endl;
}

/*da codul(indexul) caracterului c in tabloul caracter */
int da_cod(char c) {
	for (int i = 0; i < N; i++)
		if (caracter[i] == c)return i;
	return -1;
}

/*da caracterul corespunzator codului dat*/
char da_caracter(int cod) {
	return caracter[modulo(cod, N)];
}

/*transforma in baza 10 un text scris cu alfabetul dat*/
int in_baza_10(string text) {
	int i = 0, rez = 0;
	while (text[i] != '\0')
		rez = rez * N + da_cod(text[i++]);
	return rez;
}

/*transforma un numar scris in baza 10 intr-un text scris cu alfabetul dat*/
string din_baza_10(int nr) {
	char* text;
	text = new char[100];
	int i = 0;
	while (nr > 0) {
		text[i++] = da_caracter(nr % N);
		nr /= N;
	}
	text[i] = '\0';
	return _strrev(text);
}

/*analiza frecventelor*/
vector<pair<int, string>> analiza_frecvente(int k = 1, const char* cale = "sursa.txt") {
	vector<pair<int, string>> blocuri;/*Un container format din blocuri. Un bloc va contine o pereche formata dintr-un numar si un grup de k caractere, reprezentand numarul de aparitii al grupului respectiv*/
	ifstream in(cale);
	if (!in.good())
		cout << "Fisier inexistent (pentru analiza frecventelor)" << endl;
	char c[100];
	while (!in.eof()) {
		in.read(c, k);
		c[k] = '\0';
		string s(c);
		bool mai_este = false;
		for (int i = 0; i < blocuri.size(); i++)
		{
			if (blocuri[i].second == s) {
				blocuri[i].first++;
				mai_este = true;
				break;
			}
		}
		if (!mai_este)
			blocuri.push_back(std::pair<int, string>(1, s));
	}

	sort(blocuri.begin(), blocuri.end(), greater<>());//pentru sortare descendenta
	return blocuri;
}
/*============================== SFARSIT FUNCTII UTILE ======================================*/



/*============================== TIPURI UTILIZATOR ======================================*/
/*MATRICE*/
template<typename T>
class Matrice {
protected:
	T** el;
	int lin, col;
public:
	/*constructor*/
	Matrice(int lin = 2, int col = 2);
	Matrice(const Matrice<T>&);
	/*destructor*/
	~Matrice();
	/*metode*/
	int NrLin() { return lin; }
	int NrCol() { return col; }
	Matrice<T> minor(int l, int c);/*calculeaza minorul corespunzator pentru liia l, coloana c*/
	T det();/*calculeaza valoarea determinantului matricei matrice*/
	Matrice<T> transp();/*calculeaza transpusa unei matrice*/
	Matrice<T> adj();/*calculeaza adjuncta unei matrice*/
	Matrice<T> inv();/*calculeaza inversa unei matrice*/
	Matrice<T> inv(int n);/*calculeaza inversa unei matrice in Zn*/
	/*supraincarcare operatori*/
	Matrice<T> operator+(const Matrice<T>&);/*calculeaza suma a doua matrice*/
	Matrice<T> operator-(const Matrice<T>&);/*calculeaza diferenta a doua matrice*/
	Matrice<T> operator*(const Matrice<T>&);/*calculeaza produsul a doua matrice*/
	Matrice<T> operator*(const T);/*calculeaza produsul unei matrice cu un scalar*/
	Matrice<T> operator-();/*schimba semnul elementelor*/
	Matrice<T> operator%(int n);/*calculeaza modulo pentru elemente*/


	Matrice<T>& operator=(const Matrice<T>&);
	T* operator[](int);
	template<typename T> friend istream& operator >> (istream& in, Matrice<T>&);
	template<typename T> friend ostream& operator << (ostream& out, const Matrice<T>&);
	template<typename T> friend class Vector;
};

template<typename T>
Matrice<T>::Matrice(int lin, int col) {
	this->lin = lin;
	this->col = col;
	el = new T * [lin];
	for (int i = 0; i < lin; i++) {
		el[i] = new T[col];
		for (int j = 0; j < col; j++)
			el[i][j] = 0;
	}
}
template<typename T>
Matrice<T>::Matrice(const Matrice& M) {
	lin = M.lin;
	col = M.col;
	el = new T * [M.lin];
	for (int i = 0; i < M.lin; i++) {
		el[i] = new T[M.col];
		for (int j = 0; j < M.col; j++)
			el[i][j] = M.el[i][j];
	}
}
template<typename T>
Matrice<T>::~Matrice() {
	for (int i = 0; i < lin; i++) {
		delete[] el[i];
	}
	delete[] el;
}
template<typename T>
Matrice<T> Matrice<T>::minor(int l, int c) {
	Matrice R(lin - 1, col - 1);
	for (int i = 0; i < lin - 1; i++) {
		for (int j = 0; j < col - 1; j++) {
			if (i < l) {
				if (j < c)
					R[i][j] = el[i][j];
				else
					R[i][j] = el[i][j + 1];
			}
			else {
				if (j < c)
					R[i][j] = el[i + 1][j];
				else
					R[i][j] = el[i + 1][j + 1];
			}
		}
	}
	return R;
}
template<typename T>
T Matrice<T>::det() {
	if (lin != col || lin <= 1) return el[0][0];
	T S = 0;
	for (int i = 0; i < lin; i++) {
		S += el[0][i] * (i % 2 ? -1 : 1) * minor(0, i).det();
	}
	return S;
}
template<typename T>
Matrice<T> Matrice<T>::transp() {
	Matrice R(col, lin);
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++)
			R[i][j] = el[j][i];
	return R;
}
template<typename T>
Matrice<T> Matrice<T>::adj() {
	Matrice<T> R(col, lin);
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++)
			R[i][j] = ((i + j) % 2 ? -1 : 1) * minor(j, i).det();
	return R;
}
template<typename T>
Matrice<T> Matrice<T>::inv() {
	Matrice<T> R(col, lin);
	T d = det();
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++)
			R[i][j] = ((i + j) % 2 ? -1 : 1) * minor(j, i).det() / d;
	return R;
}
template<typename T>
Matrice<T> Matrice<T>::inv(int n) {
	Matrice<T> R(col, lin);
	T d = invers(det(), n);
	if (d < 0 || lin == 1) {
		R[0][0] = d;
		return R;
	}
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++)
			R[i][j] = modulo(((i + j) % 2 ? -1 : 1) * minor(j, i).det() * d, n);
	return R;
}
template<typename T>
Matrice<T> Matrice<T>::operator+(const Matrice<T>& M) {
	Matrice R(lin, col);
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++)
			R[i][j] = el[i][j] + M.el[i][j];
	return R;
}
template<typename T>
Matrice<T> Matrice<T>::operator-(const Matrice<T>& M) {
	Matrice R(lin, col);
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++)
			R[i][j] = el[i][j] - M.el[i][j];
	return R;
}
template<typename T>
Matrice<T> Matrice<T>::operator*(const Matrice<T>& M) {
	Matrice R(lin, M.col);
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < M.col; j++) {
			R[i][j] = 0;
			for (int k = 0; k < col; k++)
				R[i][j] += el[i][k] * M.el[k][j];
		}
	return R;
}
template<typename T>
Matrice<T> Matrice<T>::operator*(const T t) {
	Matrice R(lin, col);
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++) {
			R[i][j] = t * el[i][j];
		}
	return R;
}
template<typename T>
Matrice<T> Matrice<T>::operator-() {
	Matrice R(lin, col);
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++) {
			R[i][j] = -el[i][j];
		}
	return R;
}
template<typename T>
T* Matrice<T>::operator[](int index) {
	if (index >= 0 && index < lin)
		return el[index];
	return 0;
}
template<typename T>
Matrice<T> Matrice<T>::operator%(int n) {
	Matrice<T> R(lin, col);
	for (int i = 0; i < lin; i++)
		for (int j = 0; j < col; j++)
			R[i][j] = modulo(el[i][j], n);
	return R;
}
template<typename T>
Matrice<T>& Matrice<T>::operator=(const Matrice<T>& M) {
	if (this != &M) {
		for (int i = 0; i < lin; i++) {
			delete[] el[i];
		}
		delete[] el;
		lin = M.lin;
		col = M.col;
		el = new T * [lin];
		for (int i = 0; i < lin; i++) {
			el[i] = new T[col];
			for (int j = 0; j < col; j++)
				el[i][j] = M.el[i][j];
		}
	}
	return *this;
}
template<typename T>
istream& operator >> (istream& in, Matrice<T>& M) {
	for (int i = 0; i < M.lin; i++) {
		delete[] M.el[i];
	}
	delete[] M.el;
	in >> M.lin >> M.col;
	M.el = new T * [M.lin];
	for (int i = 0; i < M.lin; i++) {
		M.el[i] = new T[M.col];
	}
	for (int i = 0; i < M.lin; i++) {
		for (int j = 0; j < M.col; j++)
			in >> M.el[i][j];
	}
	return in;
}
template<typename T>
ostream& operator << (ostream& out, const Matrice<T>& M) {
	for (int i = 0; i < M.lin; i++) {
		for (int j = 0; j < M.col; j++)
			cout << M.el[i][j] << " ";
		cout << endl;
	}
	return out;
}

/*VECTOR*/
template<typename T>
class Vector :public Matrice<T> {
public:
	Vector(int dim = 2);
	explicit Vector(const Matrice<T>&);
	Vector<T>& operator=(const Matrice<T>&);
	T& operator[](int);
	template<typename T> friend istream& operator >> (istream& in, Vector<T>&);
	template<typename T> friend ostream& operator << (ostream& out, const Vector<T>&);
};
template<typename T>
Vector<T>::Vector(int dim) :Matrice(dim, 1) {}
template<typename T>
Vector<T>::Vector(const Matrice<T>& M) : Matrice(M.lin, 1) {
	for (int i = 0; i < this->lin; i++) {
		this->el[i][0] = M.el[i][0];
	}
}
template<typename T>
Vector<T>& Vector<T>::operator=(const Matrice<T>& M) {
	if (this != &M) {
		for (int i = 0; i < this->lin; i++) {
			delete[] this->el[i];
		}
		delete[] this->el;
		this->lin = M.lin;
		this->col = 1;
		this->el = new T * [this->lin];
		for (int i = 0; i < this->lin; i++) {
			this->el[i] = new T[this->col];
			for (int j = 0; j < this->col; j++)
				this->el[i][j] = M.el[i][j];
		}
	}
	return *this;
}
template<typename T>
T& Vector<T>::operator[](int index) {
	return this->el[index][0];
}
template<typename T>
istream& operator >> (istream& in, Vector<T>& M) {
	for (int i = 0; i < M.lin; i++) {
		delete[] M.el[i];
	}
	delete[] M.el;
	in >> M.lin;
	M.col = 1;
	M.el = new T * [M.lin];
	for (int i = 0; i < M.lin; i++) {
		M.el[i] = new T[M.col];
	}
	for (int i = 0; i < M.lin; i++) {
		for (int j = 0; j < M.col; j++)
			in >> M.el[i][j];
	}
	return in;
}
template<typename T>
ostream& operator << (ostream& out, const Vector<T>& M) {
	for (int i = 0; i < M.lin; i++) {
		cout << M.el[i][0] << " ";
	}
	return out;
}

/*pentru lucrul cu numere mari*/
template<unsigned int N>
class BigInt {
private:
	bitset<N> bits;
public:
	static int nr;
	BigInt() : bits() { }
	BigInt(const BigInt& x) : bits(x.bits) { }
	BigInt(unsigned long x) {
		int n = 0;
		while (x) {
			bits[n++] = x & (unsigned long)1;
			x >>= 1;
		}
	}
	BigInt(int x) :BigInt((unsigned long)x) {}
	explicit BigInt(const bool b[], unsigned int dim) {
		bits.reset();
		if (dim > N)dim = N;
		for (int i = 0; i < N && i < dim; i++)bits[dim - 1 - i] = b[i];
	}
	explicit BigInt(const bitset<N>& x) : bits(x) { }
	BigInt(const char* x)
	{
		int indexBit = 0;
		char p[1 + N / 3];
		strncpy(p, x, N / 3);//luam sa incapa
		p[N / 3] = '\0';
		while (strlen(p)) {
			bits[indexBit++] = p[strlen(p) - 1] % 2;
			int temp = 0, aux = 0;
			for (int i = 0; i < strlen(p); i++) {
				aux = p[i] - '0' + temp * 10;
				p[i] = '0' + aux / 2;
				temp = aux % 2;
			}
			if (p[0] == '0')
				strcpy(p, p + 1);
		}
	}

	static void divide(BigInt x, BigInt y, BigInt& q, BigInt& r) {
		if (y == 0) throw std::domain_error("division by zero undefined");
		q = r = 0;
		if (x == 0) return;
		if (x == y) {
			q.bits[0] = 1;
			return;
		}
		r = x;
		if (x < y) return;

		// determinam bitul cel mai semnificativ in x si y
		unsigned int sig_x;
		for (int i = N - 1; i >= 0; i--) {
			sig_x = i;
			if (x[i]) break;
		}
		unsigned int sig_y;
		for (int i = N - 1; i >= 0; i--) {
			sig_y = i;
			if (y[i]) break;
		}
		// aliniem x si y
		unsigned int n = (sig_x - sig_y);
		y <<= n;
		n += 1;
		// algoritmul: deplasare si scadere 
		while (n--) {
			if (y <= r) {
				q.bits[n] = true;
				r -= y;
			}
			y >>= 1;
		}
	}
	static BigInt sqrt(BigInt a) {
		BigInt x0 = a, x1 = (a + 1) / 2;
		while (x1 < x0) {
			x0 = x1;
			x1 = (x1 + a / x1) / 2;
		}
		return x0;
	}
	static bool isSqrt(BigInt a) {
		BigInt x0 = a, x1 = (a + 1) / 2;
		while (x1 < x0) {
			x0 = x1;
			x1 = (x1 + a / x1) / 2;
		}
		return x0 * x0 == a;
	}
	static BigInt pow(BigInt a, BigInt b) {
		if (b == 0) return 1;
		BigInt temp = pow(a, b >> 1);
		if (b[0] == 0) return temp * temp;
		return temp * temp * a;
	}
	static BigInt rand() {
		BigInt temp;
		for (int i = 0; i < N; i++)
			temp.bits[i] = std::rand() % 2;
		return temp;
	}
	static unsigned int log(BigInt a) { // log_2(a)
		for (unsigned int i = N - 1; i >= 0; i--)
			if (a.bits[i] != 0)return i;
		throw std::domain_error("log invalid argument");
	}
	void factorizare(BigInt& p, BigInt& q) {
		BigInt n = *this;
		BigInt s_patrat, t;
		t = sqrt(n) + 1;
		s_patrat = t * t - n;
		while (!isSqrt(s_patrat) && (t <= n)) {
			t++;
			s_patrat = t * t - n;
		}
		if (isSqrt(s_patrat) >= 0) {
			s_patrat = sqrt(s_patrat);
			p = t + s_patrat;
			q = t - s_patrat;
		}
		else {
			p = 0; q = 0;
		}
	}
	void scrie(ostream& out) {
		unsigned char c = 0;
		unsigned long i;
		for (i = 0; i < N; i++) {
			c <<= 1;
			if (bits[N - i - 1])c++;
			if (i % 8 == 7) {
				out << c;
				c = 0;
			}
		}
		if (i % 8)out << c;
	}
	bool citeste(istream& in) {
		unsigned char c = 0;
		unsigned long i = 0;
		bits.reset();
		while (i<N && in >> noskipws >> c) {
			for (int j = 7; i < N && j >= 0; j--) {
				bits[N - 1 - i++] = c & (1 << j);
			}
		}
		return !in.eof();
	}
	static bool este_supercrescator(BigInt v[], const int dim) {
		if (dim <= 0)return true;
		BigInt S = v[0];
		for (int i = 1; i < dim; i++) {
			if (S >= v[i])return false;
			S += v[i];
		}
		return true;
	}
	static bool rezolva_rucsac(BigInt V, BigInt v[], bool eps[], int dim) {
		//punem rezultatul in eps
		for (int i = 0; i < dim; i++)
			eps[i] = 0;
		for (int i = dim - 1; i >= 0; i--) {
			if (V >= v[i]) {
				eps[i] = 1;
				V -= v[i];
				if (V == 0)return true;
			}
		}
		return V == 0;
	}
	static BigInt cmmdc(BigInt x, BigInt y) {
		BigInt rest;
		do {
			rest = x % y;
			x = y;
			y = rest;
		} while (rest != 0);
		return x;
	}
	BigInt invers(BigInt n) {
		BigInt q, r, x0 = 1, x1 = 0, copy_n = n, a = *this % n;
		while (n != 0)
		{
			r = n;
			q = a / n;
			n = a % n;
			a = r;

			r = x1;
			x1 = q * x1;
			while (x0 < x1)x0 += copy_n;

			x1 = x0 - x1;
			x0 = r;
		}
		if (a == BigInt(1))
			return x0 % copy_n;
		return 0;
	}

	BigInt inmultire(const BigInt& x) {
		BigInt temp = 0;
		if (bits.count() < x.bits.count()) {
			for (unsigned int i = 0; i < N; i++)
				if (bits[i]) temp += x << i;
		}
		else {
			for (unsigned int i = 0; i < N; i++)
				if (x[i])  temp += (*this << i);
		}
		return temp;
	}
	int pozitie_max_bit() const {
		nr++;
		for (int i = N - 1; i >= 0; i--) {
			if (bits[i])return i;
		}
		return -1;
	}
	BigInt karatsuba(const BigInt& num) {
		int n1 = pozitie_max_bit();
		int n2 = num.pozitie_max_bit();
		int max = n1, min = n2;
		if (n1 < n2) {
			max = n2;
			min = n1;
		}
		if (max < 8)return inmultire(num);

		int mij = (max + 1) / 2;
		BigInt s1 = *this >> mij, d1 = *this << (N - mij) >> N - mij;
		BigInt s2 = num >> mij, d2 = num << (N - mij) >> N - mij;
		BigInt z0 = d1.karatsuba(d2);
		BigInt z1 = (s1 + d1).karatsuba(s2 + d2);
		BigInt z2 = s1.karatsuba(s2);
		return (z2 << (2 * mij)) + ((z1 - z2 - z0) << mij) + z0;
	}
	bitset<N> Bits() {
		return bits;
	}
	bool operator[](int n) const {
		return bits[n];
	}
	operator string() const {
		int nrBlocuri = 1 + N / 3;
		bitset<4> bloc[1 + N / 3];

		for (int i = N - 1; i >= 0; i--) {
			for (int j = 0; j < nrBlocuri; j++) {
				if (bloc[j].to_ulong() >= 5)bloc[j] = bloc[j].to_ulong() + 3;
			}
			for (int j = 0; j < nrBlocuri - 1; j++) {
				bloc[j] <<= 1;
				bloc[j][0] = bloc[j + 1][3];
			}
			bloc[nrBlocuri - 1] <<= 1;
			bloc[nrBlocuri - 1][0] = bits[i];
		}
		string rez = "";
		bool inceput = false;
		for (int i = 0; i < nrBlocuri; i++) {
			if (bloc[i].to_ulong() > 0)inceput = true;
			if (inceput)rez.append(to_string(bloc[i].to_ulong()));
		}
		return inceput ? rez : "0";
	}
	BigInt& operator<<=(unsigned int n) {
		bits <<= n;
		return *this;
	}
	BigInt& operator>>=(unsigned int n) {
		bits >>= n;
		return *this;
	}
	BigInt operator++(int) {
		BigInt temp = *this;
		operator++();
		return temp;
	}
	BigInt operator--(int) {
		BigInt temp = *this;
		operator--();
		return temp;
	}
	BigInt& operator++() {
		bool temp = false;
		bool suma = ~bits[0] ^ temp;
		temp = bits[0] || temp || (bits[0] && temp);
		bits[0] = suma;
		for (int i = 1; i < N; i++) {
			suma = bits[i] ^ temp;
			temp = bits[i] && temp;
			bits[i] = suma;
		}
		return *this;
	}
	BigInt& operator--() {
		bool temp = !bits[0];
		bits[0] = ~bits[0];
		for (int i = 1; i < N; i++) {
			if (temp) {
				temp = !bits[i];
				bits[i] = !bits[i];
			}
			else {
				temp = 0;
			}
		}
		return *this;
	}
	BigInt& operator+=(const BigInt& x) {
		bool temp = false;
		for (int i = 0; i < N; i++) {
			bool suma = (bits[i] ^ x.bits[i]) ^ temp;
			temp = (bits[i] && x.bits[i]) || (bits[i] && temp) || (x.bits[i] && temp);
			bits[i] = suma;
		}
		return *this;
	}
	BigInt& operator-=(const BigInt& x) {
		bool borrow = false;
		for (int i = 0; i < N; i++) {
			if (borrow) {
				if (bits[i]) {
					bits[i] = x.bits[i];
					borrow = x.bits[i];
				}
				else {
					bits[i] = !x.bits[i];
					borrow = true;
				}
			}
			else {
				if (bits[i]) {
					bits[i] = !x.bits[i];
					borrow = false;
				}
				else {
					bits[i] = x.bits[i];
					borrow = x.bits[i];
				}
			}
		}
		return *this;
	}
	BigInt& operator*=(const BigInt& x) {
		BigInt temp = *this;
		*this = 0;
		if (temp.bits.count() < x.bits.count()) {
			for (unsigned int i = 0; i < N; i++)
				if (temp[i]) *this += x << i;
		}
		else {
			for (unsigned int i = 0; i < N; i++)
				if (x[i]) *this += (temp << i);
		}
		return *this;
	}
	BigInt& operator/=(const BigInt& x) {
		BigInt temp;
		divide(*this, x, *this, temp);
		return *this;
	}
	BigInt& operator%=(const BigInt& x) {
		BigInt temp;
		divide(*this, x, temp, *this);
		return *this;
	}
	BigInt operator~() const {
		return ~bits;
	}
	BigInt& operator&=(BigInt x) {
		bits &= x.bits;
		return *this;
	}
	BigInt& operator|=(BigInt x) {
		bits |= x.bits;
		return *this;
	}
	BigInt& operator^=(BigInt x) {
		bits ^= x.bits;
		return *this;
	}
	friend BigInt operator<<(BigInt x, unsigned int n) {
		return x <<= n;
	}
	friend BigInt operator >> (BigInt x, unsigned int n) {
		return x >>= n;
	}
	friend BigInt operator+(BigInt x, const BigInt& y) {
		return x += y;
	}
	friend BigInt operator-(BigInt x, const BigInt& y) {
		return x -= y;
	}
	friend BigInt operator*(BigInt x, const BigInt& y) {
		return x *= y;
	}
	friend BigInt operator/(BigInt x, const BigInt& y) {
		return x /= y;
	}
	friend BigInt operator%(BigInt x, const BigInt& y) {
		return x %= y;
	}
	friend BigInt operator^(BigInt x, const BigInt& y) {
		return x ^= y;
	}
	friend BigInt operator&(BigInt x, const BigInt& y) {
		return x &= y;
	}
	friend BigInt operator|(BigInt x, const BigInt& y) {
		return x |= y;
	}
	friend bool operator==(const BigInt& x, const BigInt& y) {
		return x.bits == y.bits;
	}
	friend bool operator!=(const BigInt& x, const BigInt& y) {
		return x.bits != y.bits;
	}
	friend bool operator>(const BigInt& x, const BigInt& y) {
		for (int i = N - 1; i >= 0; i--) {
			if (x[i] && !y[i]) return true;
			if (!x[i] && y[i]) return false;
		}
		return false;
	}
	friend bool operator<(const BigInt& x, const BigInt& y) {
		for (int i = N - 1; i >= 0; i--) {
			if (x[i] && !y[i]) return false;
			if (!x[i] && y[i]) return true;
		}
		return false;
	}
	friend bool operator>=(const BigInt& x, const BigInt& y) {
		for (int i = N - 1; i >= 0; i--) {
			if (x[i] && !y[i]) return true;
			if (!x[i] && y[i]) return false;
		}
		return true;
	}
	friend bool operator<=(const BigInt& x, const BigInt& y) {
		for (int i = N - 1; i >= 0; i--) {
			if (x[i] && !y[i]) return false;
			if (!x[i] && y[i]) return true;
		}
		return true;
	}
	friend istream& operator >> (istream& in, BigInt& n) {
		in >> n.bits;
		return in;
	}
	friend ostream& operator << (ostream& out, const BigInt& n) {
		out << (string)n;
		return out;
	}
	friend class CitesteBinar;
};
template<unsigned int N> int BigInt<N>::nr = 0;

class CitesteBinar {
	ifstream in;
	long dimMax, dimCitit;
	unsigned char c;
	int index;
public:
	CitesteBinar(const char* sursa) {
		in = ifstream(sursa, std::ios::binary);
		c = 0;
		dimMax = dimCitit = 0;
		//citim lungimea fisierului
		if (in) {
			in.seekg(0, in.end);
			dimMax = in.tellg();
			in.seekg(0, in.beg);
		}
		index = -1;
	}
	int citeste(bool bts[], int max) {
		int i = 0;
		while (i < max && index >= 0) {
			bts[i++] = c & (1 << index--);
		}
		while (i<max && in >> noskipws >> c) {
			dimCitit++;
			for (index = 7; index >= 0 && i < max; index--) {
				bts[i++] = c & (1 << index);
			}
		}
		return i;
	}
	template<unsigned int N>
	int citeste(BigInt<N>& V) {
		V.bits.reset();
		int i = 0;
		while (i < N && index >= 0) {
			V.bits[N - 1 - i++] = c & (1 << index--);
		}
		while (i<N && in >> noskipws >> c) {
			dimCitit++;
			for (index = 7; index >= 0 && i < N; index--) {
				V.bits[N - 1 - i++] = c & (1 << index);
			}
		}
		return i;
	}
	void scrieProcent() {
		int procent = (dimMax == 0) ? 100 : 100 * dimCitit / dimMax;
		printf("\r%3d%% [%.*s%*s]", procent, procent / 2, "||||||||||||||||||||||||||||||||||||||||||||||||||", 50 - procent / 2, "");
		fflush(stdout);
	}
	void close() {
		if (in)in.close();
	}
};
//criptosistem Merkle-Hellman
template<int N>
void generareCheiMH(BigInt<N> w[], int nr_elem, BigInt<N>& m, BigInt<N>& cheia_secreta) {
	srand(time(NULL));
	BigInt<N> pas_max = 1 << (N / 2 - nr_elem);//diferenta dintre noul termen si suma termenilor anteriori
	BigInt<N> S = 0;
	for (int i = 0; i < nr_elem; i++) {
		w[i] = S + 1 + rand() % pas_max;//punem 1 pentru ca primul termen sa nu fie zero niciodata
		S += w[i];
	}
	m = S + 1 + rand() % pas_max;
	cheia_secreta = 1 + rand() % (m - 1);
	while (BigInt<N>::cmmdc(cheia_secreta, m) != 1) {//pentru ca a sa fie inversabil in Zm
		cheia_secreta++;
	}
	BigInt<N> b = cheia_secreta.invers(m);

	for (int i = 0; i < nr_elem; i++) {
		w[i] = (b * w[i]) % m;
	}
}
template<int N>
void criptareMH(const char* sursa, const char* destinatie, BigInt<N> w[], int nr_elem) {
	CitesteBinar cb(sursa);
	ofstream out(destinatie, std::ios::binary);
	int k;
	bool* bts = new bool[nr_elem];
	while ((k = cb.citeste(bts, nr_elem)) > 0) {
		BigInt<N> S = 0;
		for (int i = 0; i < k; i++)
			if (bts[i])S += w[i];
		S.scrie(out);
		cb.scrieProcent();
	}
	delete[] bts;
	cb.close();
	out.close();
}
template<int N>
void decriptareMH(const char* sursa, const char* destinatie, BigInt<N> w[], int nr_elem, BigInt<N> m, BigInt<N> a) {
	CitesteBinar cb(sursa);
	ofstream out(destinatie, std::ios::binary);
	BigInt<N> V;
	int k;
	bool* bts = new bool[nr_elem];
	unsigned char c = 0;
	int index = 0;
	for (int i = 0; i < nr_elem; i++)w[i] = (w[i] * a) % m;
	while ((k = cb.citeste(V)) > 0) {
		V = (V * a) % m;
		BigInt<N>::rezolva_rucsac(V, w, bts, nr_elem);
		for (int i = 0; i < nr_elem; i++) {
			c <<= 1;
			if (bts[i])c++;
			if (++index == 8) {
				out << c;
				index = 0;
			}
		}
		cb.scrieProcent();
	}
	if (index > 0) {
		out << c;
	}
	delete[] bts;
	cb.close();
	out.close();
}
class SHA1 {
	unsigned long l1, l2;//pentru lungimea textului text (numarul de biti). Nr. total de biti va fi l1*2^32+l2<2^64
	unsigned long* h;
public:
	SHA1() {
		l1 = l2 = 0;
		h = new unsigned long[5];
		h[0] = 0x67452301;//valorile initiale
		h[1] = 0xEFCDAB89;
		h[2] = 0x98BADCFE;
		h[3] = 0x10325476;
		h[4] = 0xC3D2E1F0;
	}
	void aduna_lungime(unsigned long valoare) {//se aduna valoare la valoarea pastrata in l1 si l2
		unsigned long x = 0xFFFFFFFF - l2;
		if (x >= valoare) {//daca valoare incape in l2
			l2 += valoare;
		}
		else {
			l1++;
			l2 = valoare - x - 1;
		}
	}
	unsigned long leftrotate(unsigned long x, int n) {
		return (x << n) | (x >> (32 - n));
	}
	unsigned longxor(unsigned long x, unsigned long y) {//se poate folosi in locul operatorului ^
		return (x | y) & (~x | ~y);
	}
	void transforma_h(unsigned long* w) {//w are 80 elemente
		unsigned long a, b, c, d, e, f, k, temp;
		a = h[0];
		b = h[1];
		c = h[2];
		d = h[3];
		e = h[4];
		for (int i = 16; i < 80; i++)
			w[i] = leftrotate(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
		for (int i = 0; i < 80; i++) {
			if (0 <= i && i < 20) {
				f = (b & c) | ((~b) & d);
				k = 0x5A827999;
			}
			else if (20 <= i && i < 40) {
				f = b ^ c ^ d;
				k = 0x6ED9EBA1;
			}
			else if (40 <= i && i < 60) {
				f = (b & c) | (b & d) | (c & d);
				k = 0x8F1BBCDC;
			}
			else if (60 <= i && i < 80) {
				f = b ^ c ^ d;
				k = 0xCA62C1D6;
			}
			temp = leftrotate(a, 5) + f + e + k + w[i];
			e = d;
			d = c;
			c = leftrotate(b, 30);
			b = a;
			a = temp;
		}
		h[0] = h[0] + a;
		h[1] = h[1] + b;
		h[2] = h[2] + c;
		h[3] = h[3] + d;
		h[4] = h[4] + e;
	}

	void unsigned_long_to_bin(unsigned long l, char* rez) {
		for (int i = 7; i >= 0; i--) {
			for (int j = 3; j >= 0; j--)
				rez[3 - j + i * 4] = ((l % 16) & (1 << j)) ? '1' : '0';
			l /= 16;
		}
	}
	unsigned long* Valoare(char* sursa) {
		ifstream in;
		in.open(sursa, std::ios_base::binary);
		in.seekg(0, ios::end);
		int n = in.tellg();
		in.seekg(0, ios::beg);
		char* res = new char[n];
		in.read(res, n);
		in.close();
		return Valoare(res, n);
	}
	char* ValoareBin(char* text, int len) {
		char rez[161];
		unsigned long* hash = Valoare(text, len);
		for (int i = 0; i < 5; i++)
			unsigned_long_to_bin(hash[i], rez + i * 32);
		rez[160] = '\0';
		return rez;
	}
	unsigned long* Valoare(char* text, int len) {
		unsigned long* w = new unsigned long[80];
		unsigned char* c = new unsigned char[4];//din 4 char facem un double
		int i = 0, index = 0;
		while (index < len) {
			c[i % 4] = text[index++];
			i++;
			aduna_lungime(8);//am citit 8 biti
			if (i % 4 == 0) {
				w[i / 4 - 1] = (c[0] << 24) | (c[1] << 16) | (c[2] << 8) | c[3];
				if (i % 64 == 0) {
					transforma_h(w);
					i = 0;
				}
			}
		}
		//completam corespunzator
		c[i % 4] = 1 << 7;
		i++;
		while ((i % 4) > 0) {
			c[i % 4] = 0;
			i++;
		}
		w[i / 4 - 1] = (c[0] << 24) | (c[1] << 16) | (c[2] << 8) | c[3];
		if (i <= 56) {//incap in acest bloc cei 64 biti in care trecem lungimea blocului
			while (i < 56) {
				i += 4;
				w[i / 4 - 1] = 0;
			}
			w[14] = l1;
			w[15] = l2;
			transforma_h(w);
		}
		else {//nu incap cei 64 biti, vor fi trecuti in blocul urmator
			while (i < 64) {
				i += 4;
				w[i / 4 - 1] = 0;
			}
			transforma_h(w);
			for (i = 4; i < 56; i += 4)
				w[i / 4 - 1] = 0;
			w[14] = l1;
			w[15] = l2;
			transforma_h(w);
		}
		return h;
	}

	/*============================== SFARSIT TIPURI UTILIZATOR ======================================*/
};
#endif