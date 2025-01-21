#pragma once
#include "shakki.h.cpp"
#include <iostream>
using namespace std;

// Pelin tilan kuvaaminen ja siihen liittyvat operaatiot.
class Asema {
public:

	// Laudan nappulat. Indeksointi [rivi][linja], esim.
	//
	// [0][0] : vasen ylanurkka ("a8")
	// [7][0] : vasen alanurkka ("a1")
	// [7][7] : oikea alanurkka ("h1")
	//
	int _lauta[8][8] = {
		{ bR, bN, bB, bQ, bK, bB, bN, bR },
		{ bP, bP, bP, bP, bP, bP, bP, bP },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ wP, wP, wP, wP, wP, wP, wP, wP },
		{ wR, wN, wB, wQ, wK, wB, wN, wR }
	};

	// Kumman pelaajan siirtovuoro?
	int _siirtovuoro = VALKEA;

	// Kirjanpito siita, onko kuningas tai torni liikkunut.
	// Asetetaan tarpeen mukaan falseksi.
	bool _valkea_lyhyt_linna_sallittu = true;
	bool _valkea_pitka_linna_sallittu = true;
	bool _musta_lyhyt_linna_sallittu = true;
	bool _musta_pitka_linna_sallittu = true;

	// Ohestaly�ntiin tarvittava info.
	int _kaksoisaskel_linjalla = -1;


	// LaKSY
	// Tyhjentaa laudan.
	//eli uudelleenkirjoitetaan jokainen laudan arvo NA:ksi?
	void tyhjenna()
	{
	    for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++) {
                _lauta[i][j] = NA;
            }
	    }
	}
	// LaKSY
	// Tulosta lauta ascii-grafiikkana (mahdollisesti flagit
	// voisi myas tulostaa jotenkin siististi).
	void tulosta() const {
	    const char* asciiChessPieces[] = {
	    "R", "N", "B", "Q", "K", "P", //valkoiset nappulat
	    "r", "n", "b", "q", "k", "p", //mustat nappulat
	    "."                           //tyhja ruutu
	    };
	    /* merkit eivät näkyneet terminaalissa
	    const char* unicodeChessPieces[] = {
	        // ♔ 	♕ 	♖ 	♗ 	♘ 	♙ 	♚ 	♛ 	♜ 	♝ 	♞ 	♟
	        "♖", "♘", "♗", "♕", "♔", "♙", //valkoiset nappulat
	        "♜", "♞", "♝", "♛", "♚", "♟", //mustat nappulat
	        "."                             //tyhja ruutu
	    };*/
        cout << "  a b c d e f g h" << endl;
        for (int i = 0; i < 8; i++) {
                cout << 8 - i << " ";
                for (int j = 0; j < 8; j++) {
                    int piece = _lauta[i][j];
                    if (piece >= 0 && piece <= NA) {
                        cout << asciiChessPieces[piece] << " ";
                    } else {
                        cout << "? ";
                        }
                    }
                    cout << 8 - i << endl;
                }

        cout << "  a b c d e f g h" << endl;
        }
	// L�KSY
	// etsii annetun kuninkaan (wK tai mK) rivin ja linjan

	// en saanut toimimaan
	void etsi_kuningas(int nappula, int& rivi, int& linja) const {
		for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (_lauta[i][j] == nappula) {
                    rivi = i;
                    linja = j;
                    return;
                    }
                }
            }
            rivi = -1; // Not found
            linja = -1;
        }
};
int main() {
Asema asema;
asema.tulosta();
//asema.etsi_kuningas(wK);
return 0;
}
