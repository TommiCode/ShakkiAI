#pragma once
#include <string>
#include "shakki.h"
using namespace std;

// Kuvaa aseman muutosta.
class Siirto
{
public:

  // Oletuskonstruktori (ei tee oikeastaan mitaan).
	Siirto() {}

  // Alustaa siirron koordinaateista. (a_r, a_l) => (l_r, l_l)
	Siirto(int a_r, int a_l, int l_r, int l_l) :
		_a_r(a_r), _a_l(a_l), _l_r(l_r), _l_l(l_l)
	{}

	// Konstruktori, jonka avulla siirto-olio alustetaan
	// annetusta merkkijonosta, esim. "e2e4" tai "g8f6".
	Siirto(const string& s) {
    if (s.length() == 4) {
        _a_l = s[0] - 'a'; // Muuntaa a-h → 0-7 alkulinja
        _a_r = s[1] - '1'; // Muuntaa 1-8 → 7-0 alkurivi
        _l_l = s[2] - 'a'; //                   loppulinja
        _l_r = s[3] - '1'; //                   loppurivi

        //cout << "Siirto: " << _a_r << ", " << _a_l << " -> " << _l_r << ", " << _l_l << endl;
        }
    }
    void tulosta() const {
    char alkuLinja = 'a' + _a_l;  // Muutetaan linja kirjaimeksi
    int alkuRivi = 1 +_a_r;      // Muutetaan rivi shakkinumeroksi

    char loppuLinja = 'a' + _l_l;
    int loppuRivi = 1 +_l_r;
    cout << "(" << alkuLinja << alkuRivi << loppuLinja << loppuRivi << ")" << endl;
    }
private:

	int _a_r; // siirron alkuruudun rivi
	int _a_l; // siirron alkuruudu linja

	int _l_r; // siirron loppuruudun rivi
	int _l_l; // siirron loppuruudun linja

	int _korotettava_nappula = NA; // esim. wQ, wR, bN jne.
	bool _ohestalyonti = false;

	friend class Asema;
	//
};
