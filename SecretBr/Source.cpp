#include "CripTools.h"

string Bob_key = "0101111100110010001010010011111010111101000011111011";


string decriptare_vigenere(string cheie, string mesaj) {
	citeste_alfabet();
	int i, k;
	int corect = 1;
	for (i = 0; cheie[i] != '\0'; i++) {
		int temp = da_cod(cheie[i]);
		if (temp < 0 || temp >= N)corect = 0;
	}
	k = i;
	if (!corect) {
		cout << "\nCheia aleasa contine caractere care nu sunt in alfabet." << endl;
		return 0;
	}

	char c;
	i = 0;
	string decodificat = "";
	for (int j = 0; j < mesaj.length(); j++) {
		decodificat = decodificat + da_caracter(da_cod(mesaj[j]) + da_cod(cheie[i]));
		i++;
		if (i == k)i = 0;
	}
	return decodificat;

}

string testeaza_fisier(const char* msg, string key) {
	ifstream in(msg);
	if (!in) {
		cout << "Cannot open input file.\n";
	}
	bool flag = false;
	string str;

	while (getline(in, str)) {
		if (str.length() == 0) continue;
		string decodificat;
		string str1, str2;
		decodificat = decriptare_vigenere(key, str);
		str1 = decodificat.substr(0, 128);
		str2 = decodificat.substr(128, 160);


		char* B1;
		B1 = new char[str1.length()];
		for (int i = 0; i < str1.length(); i++) B1[i] = str1[i];
		SHA1 sha;
		unsigned long* rezultat = sha.Valoare(B1, str1.length());
		bitset<32>* b;
		b = new bitset<32>[5];
		string str = "";
		for (int i = 0; i < 5; i++) {
			b[i] = bitset<32>(rezultat[i]);
			str = str + b[i].to_string();
		}
		if (str == str2) {
			in.close();
			return str1;
		}

	}
	in.close();
	return "";

}

int RSA_key(int n, int e) {
	int p, q;
	factorizare(n, p, q);
	int phi = (p - 1) * (q - 1);
	int d = invers(e, phi);
	return d;
}

void decriptare_fisier(const char* sursa, const char* destinatie, string cheie) {
	ifstream in(sursa, ios::binary | ios::in);
	ofstream out(destinatie, ios::binary | ios::out);
	char ch;

	string mesaj = "";
	while (in >> noskipws >> ch) {
		mesaj = mesaj + bitset<8>(int(ch)).to_string();
	}
	string msg = decriptare_vigenere(cheie, mesaj);
	string* impartire;
	impartire = new string[msg.length() / 8];
	for (int i = 0; i < msg.length() / 8; i++) {
		impartire[i] = msg.substr(i * 8, 8);
	}

	string str = "";
	for (int i = 0; i < msg.length() / 8; i++) {
		str = str + char(bitset<8>(impartire[i]).to_ullong());
	}
	out << str << endl;

	in.close();
	out.close();

}


int main() {
	string decrypt_key1, decrypt_key2;
	decrypt_key1 = testeaza_fisier("msg1.txt", Bob_key);
	decrypt_key2 = testeaza_fisier("msg2.txt", Bob_key);
	
	if (decrypt_key1 != "" && decrypt_key2 != "") {
		cout << "Bob are acces la ambele fisiere" << endl;
		cout << "Cheia de decriptare pentru prima fotografie: " << decrypt_key1 << endl;
		cout << "Cheia de decriptare pentru a doua fotografie: " << decrypt_key2 << endl;
	}
	else
		cout << "Bob are acces la un singur fisier" << endl;

	cout << endl;

	int n = 17741;
	int e = 5587;
	int d = RSA_key(n, e);
	cout << "Cheia secreta a colegului lui Bob pentru criptosistemul RSA: " << d << '\n';

	for (int nr = 0; nr < 2339; nr++) {
		bitset<12> b(nr);
		if (testeaza_fisier("msg1.txt", b.to_string()) != "" && testeaza_fisier("msg2.txt", b.to_string()) != "") {
			cout << "Cheia secreta a colegului lui Bob pentru comunicarea cu serverul este: " << nr << '\n';
			cout << "Reprezentarea " << nr << " in binar: " << b << '\n';
			//cout << "Cheia de decriptare pentru prima fotografie: " << testeaza_fisier("msg1.txt", b.to_string()) << endl;
			//cout << "Cheia de decriptare pentru a doua fotografie: " << testeaza_fisier("msg2.txt", b.to_string()) << endl;
			break;
		}

	}
	
	
	decriptare_fisier("cript_F1.png", "decript_F1.png", decrypt_key1);
	decriptare_fisier("cript_F2.jpg", "decript_F2.jpg", decrypt_key2);


	system("pause");
	return 0;
}
