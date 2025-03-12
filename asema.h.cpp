#pragma once
#include "shakki.h"
#include "siirto.h"
#include <iostream>
#include <vector>
using namespace std;

// Pelin tilan kuvaaminen ja siihen liittyvat operaatiot.
class Asema {
public:

	// Laudan nappulat. Indeksointi [rivi][linja] rivi on numerot eli vaakarivit, linja on kirjaimet eli pystyrivit
	//
	int _lauta[8][8] = {
	    { wR, wN, wB, wQ, wK, wB, wN, wR },
        { wP, wP, wP, wP, wP, wP, wP, wP },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ bP, bP, bP, bP, bP, bP, bP, bP },
        { bR, bN, bB, bQ, bK, bB, bN, bR }
	};
	//
	/*
	int _lauta[8][8] = {
	    { NA, NA, NA, NA, NA, NA, NA, NA },
        { NA, NA, NA, NA, NA, NA, NA, NA },
		{ NA, NA, NA, wP, NA, NA, NA, NA },
		{ NA, NA, wP, NA, NA, NA, NA, NA },
		{ NA, NA, NA, wP, NA, NA, NA, NA },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
		{ NA, NA, NA, NA, NA, NA, NA, NA },
        { NA, NA, NA, NA, NA, NA, NA, NA }
	}; //piiritystilanne valkoisilla sotilailla
	*/

	// Kumman pelaajan siirtovuoro?
	int _siirtovuoro = VALKEA;

	// Kirjanpito siita, onko kuningas tai torni liikkunut.
	// Asetetaan tarpeen mukaan falseksi.
	bool _valkea_lyhyt_linna_sallittu = true;
	bool _valkea_pitka_linna_sallittu = true;
	bool _musta_lyhyt_linna_sallittu = true;
	bool _musta_pitka_linna_sallittu = true;

	// Ohestalyontiin tarvittava info.
	int _kaksoisaskel_linjalla = -1;
    //rivi on numerot eli vaakarivit, linja on kirjaimet eli pystyrivit
    void anna_ratsun_raakasiirrot(int rivi, int linja, int pelaaja, vector<Siirto>& siirrot) const {
        int vastustajaVari = vastustaja(pelaaja);
        int suunnat[8][2] = {
            {-2, -1}, {-2, 1},  //2 ylös
            {2, -1}, {2, 1},    //2 alas
            {-1, -2}, {-1, 2},  //1 ylös
            {1, -2}, {1, 2}     //1 alas
            };
            for (int k = 0; k < 8; k++) {
                int i = rivi + suunnat[k][0];
                int j = linja + suunnat[k][1];
                if (i >= 0 && i < 8 && j >= 0 && j < 8) { // Tarkistaa, että siirto on laudan sisällä
                    if (_lauta[i][j] == NA || nappulan_vari(_lauta[i][j]) == vastustajaVari) {
                        if (nappulan_vari(_lauta[i][j]) != pelaaja) {
                            siirrot.push_back(Siirto(rivi, linja, i, j));
                    }
                }
            }
        }
    }
    void anna_kuninkaan_raakasiirrot(int rivi, int linja, int pelaaja, vector<Siirto>& siirrot) const {
        int vastustajaVari = vastustaja(pelaaja);
        int suunnat[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},  // Yläviistoon vasemmalle, suoraan ylös, yläviistoon oikealle
        {0, -1},           {0, 1},   // Vasemmalle, oikealle
        {1, -1},  {1, 0},  {1, 1}    // Alaviistoon vasemmalle, suoraan alas, alaviistoon oikealle
        };

        for (int k = 0; k < 8; k++) { //suunnat
            int i = rivi + suunnat[k][0];
            int j = linja + suunnat[k][1];
            if (i >= 0 && i < 8 && j >= 0 && j < 8) { // Tarkistaa, että siirto on laudan sisällä
                if (_lauta[i][j] == NA || nappulan_vari(_lauta[i][j]) == vastustajaVari) {
                    siirrot.push_back(Siirto(rivi, linja, i, j));
            }
        }
    }
}
    void anna_kuningattaren_raakasiirrot(int rivi, int linja, int pelaaja, vector<Siirto>& siirrot) const {
        anna_lahetin_raakasiirrot(rivi, linja, pelaaja, siirrot);
        anna_tornin_raakasiirrot(rivi, linja, pelaaja, siirrot);
    }
    void anna_lahetin_raakasiirrot(int rivi, int linja, int pelaaja, vector<Siirto>& siirrot) const {
        int vastustajaVari = vastustaja(pelaaja);
        //ylös vasemmalle
        int i = rivi - 1;
        int j = linja - 1;
        while (i >= 0 && j >= 0) {
            if (_lauta[i][j] == NA) {
                siirrot.push_back(Siirto(rivi, linja, i, j));
            } else if (nappulan_vari(_lauta[i][j]) == vastustajaVari) {
                siirrot.push_back(Siirto(rivi, linja, i, j)); //vastustaja jonka voi syoda
                break;
            } else {
                break;
            }
            i--;
            j--;
        }
        //ylös oikealle
        i = rivi - 1;
        j = linja + 1;
        while (i >= 0 && j < 8) {
            if (_lauta[i][j] == NA) {
                siirrot.push_back(Siirto(rivi, linja, i, j));
            } else if (nappulan_vari(_lauta[i][j]) == vastustajaVari) {
                siirrot.push_back(Siirto(rivi, linja, i, j)); //vastustaja jonka voi syoda
                break;
            } else {
                break;
            }
            i--;
            j++;
        }
        //alas vasemmalle
        i = rivi + 1;
        j = linja - 1;
        while (i < 8 && j >= 0) {
            if (_lauta[i][j] == NA) {
                siirrot.push_back(Siirto(rivi, linja, i, j));
            } else if (nappulan_vari(_lauta[i][j]) == vastustajaVari) {
                siirrot.push_back(Siirto(rivi, linja, i, j)); //vastustaja jonka voi syoda
                break;
            } else {
                break;
            }
            i++;
            j--;
        }
        //alas oikealle
        i = rivi + 1;
        j = linja + 1;
        while (i < 8 && j < 8) {
            if (_lauta[i][j] == NA) {
                siirrot.push_back(Siirto(rivi, linja, i, j));
            } else if (nappulan_vari(_lauta[i][j]) == vastustajaVari) {
                siirrot.push_back(Siirto(rivi, linja, i, j)); //vastustaja jonka voi syoda
                break;
            } else {
                break;
            }
            i++;
            j++;
        }
    }
    void anna_tornin_raakasiirrot(int rivi, int linja, int pelaaja, vector<Siirto>& siirrot) const {
        //cout << endl << linja << endl << rivi << endl << pelaaja << endl;
        int vastustajaVari = vastustaja(pelaaja);

        // Ylös
        for (int i = rivi - 1; i >= 0; i--) {
            if (_lauta[i][linja] == NA) {
                siirrot.push_back(Siirto(rivi, linja, i, linja));
            } else {
                if (nappulan_vari(_lauta[i][linja]) == vastustajaVari) {
                    siirrot.push_back(Siirto(rivi, linja, i, linja)); //vastustaja jonka voi syoda
                }
                break;
            }
        }

        // Alas
        for (int i = rivi + 1; i < 8; i++) {
            if (_lauta[i][linja] == NA) {
                siirrot.push_back(Siirto(rivi, linja, i, linja));
            } else {
                if (nappulan_vari(_lauta[i][linja]) == vastustajaVari) {
                    siirrot.push_back(Siirto(rivi, linja, i, linja));
                }
                break;
            }
        }

        // Vasemmalle
        for (int j = linja - 1; j >= 0; j--) {
            if (_lauta[rivi][j] == NA) {
                siirrot.push_back(Siirto(rivi, linja, rivi, j));
            } else {
                if (nappulan_vari(_lauta[rivi][j]) == vastustajaVari) {
                    siirrot.push_back(Siirto(rivi, linja, rivi, j));
                }
                break;
            }
        }

        // Oikealle
        for (int j = linja + 1; j < 8; j++) {
            if (_lauta[rivi][j] == NA) {
                siirrot.push_back(Siirto(rivi, linja, rivi, j));
            } else {
                if (nappulan_vari(_lauta[rivi][j]) == vastustajaVari) {
                    siirrot.push_back(Siirto(rivi, linja, rivi, j));
                }
                break;
            }
        }
}
	// Tekee annetun siirron laudalla. Voidaan olettaa, että
	// siirto on laillinen.
	void tee_siirto(const Siirto& s) {
    _lauta[s._l_r][s._l_l] = _lauta[s._a_r][s._a_l]; // Siirretään nappula
    _lauta[s._a_r][s._a_l] = NA; // Tyhjennetään vanha ruutu
    }

    // Tyhjentää laudan.
	void tyhjenna(){
	    for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                _lauta[i][j] = NA;
            }
	    }
	    cout << "Lauta tyhjennetty." << endl;
	}
	// Tulostaa laudan
	void tulosta() const {
	    const char* asciiChessPieces[] = {
	    "R", "N", "B", "Q", "K", "P", //valkoiset nappulat
	    "r", "n", "b", "q", "k", "p", //mustat nappulat
	    "."                           //tyhja ruutu
	    };
	    /* testi, merkit eivät näkyneet konsolissa
	    const char* unicodeChessPieces[] = {
	        // ♔ 	♕ 	♖ 	♗ 	♘ 	♙ 	♚ 	♛ 	♜ 	♝ 	♞ 	♟
	        "♖", "♘", "♗", "♕", "♔", "♙", //valkoiset nappulat
	        "♜", "♞", "♝", "♛", "♚", "♟", //mustat nappulat
	        "."                             //tyhja ruutu
	    };*/
        cout << "  a b c d e f g h" << endl;
        for (int i = 0; i < 8; i++) {
                cout << i + 1 << " ";
                for (int j = 0; j < 8; j++) {
                    int piece = _lauta[i][j];
                    if (piece >= 0 && piece <= NA) {
                        cout << asciiChessPieces[piece] << " ";
                    } else {
                        cout << "? ";
                        }
                    }
                    cout << i + 1 << endl;
                }
        cout << "  a b c d e f g h" << endl << endl;;
        }
    // etsii annetun nappulan
	void etsi_nappula(int nappula, vector<pair<int, int>>& sijainnit) const {
	    sijainnit.clear();
		for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (_lauta[i][j] == nappula) {
                    sijainnit.push_back({i, j});
                    }
                }
            }
            if (!sijainnit.empty()) {
                cout << "Nappuloita on paikoilla: ";
                for (const auto& sijainti: sijainnit) {
                    char sarake = 'a' + sijainti.second;
                    int rivi = 1 + sijainti.first;
                    cout << sarake << rivi << " ";
                }
                cout << endl;
            } else {
                cout << "Nappulaa ei ole laudalla!" << endl;
            }
        cout << endl;
    }
};
int main() {
    //setuppi
    Asema asema;
    asema.tulosta();
    int valittu_nappula;
    cout << "Valitse nappula, jonka siirtoja haluat tarkistella:" << endl;
    cout << "1 = torni, 2 = lahetti, 3 = kuningatar, 4 = ratsu, 5 = kuningas" << endl;
    cout << "6 = Jos haluat koittaa minka tahansa nappulan siirtoa mihin tahansa" << endl;
    cout << "7 = Etsi haluamasi nappula laudalta" << endl;
    cin >> valittu_nappula;
    switch(valittu_nappula) {
case 1: {
    // ----
    // tornin lailliset siirrot
    // ----
    //anna_tornin_raakasiirrot(int rivi, int linja, int pelaaja, vector<Siirto>& siirrot)
    //int rivi, int linja, int pelaaja, vector<Siirto>& siirrot
    string torniLinjaKirjain;
    int torniRiviNumero, torniRivi, torniLinja, pelaajaColor;
    while (true) {

        cout << "Anna tornin rivi (1-8): ";
        cin >> torniRiviNumero;
        torniRivi = torniRiviNumero - 1;
        //cout << torniRiviNumero << endl << torniRivi;

        cout << "Anna tornin linja (a-h): ";
        cin >> torniLinjaKirjain;
        if (torniLinjaKirjain.length() != 1 || torniLinjaKirjain[0] < 'a' || torniLinjaKirjain[0] > 'h') {
            cout << "Virheellinen linja! Yritä uudelleen." << endl;
            continue;
            }
        torniLinja = torniLinjaKirjain[0] - 'a';   // Muuntaa a-h → 0-7
        //cout << torniLinjaKirjain << endl << torniLinja;

        cout << "Anna pelaajan vari (0=valkea, 1=musta): ";
        cin >> pelaajaColor;
        if (pelaajaColor != 0 && pelaajaColor != 1) {
            cout << "Virheellinen pelaajan väri! Yritä uudelleen." << endl;
            continue;
            }
        vector<Siirto> siirrot;
        //cout << endl << torniRivi << endl << torniLinja << endl << pelaajaColor << endl;
        asema.anna_tornin_raakasiirrot(torniRivi, torniLinja, pelaajaColor, siirrot);

        cout << "Tornin mahdolliset siirrot: " << endl;

        for (const auto& siirto : siirrot) {
            siirto.tulosta();
        }
        cout << endl;
        char jatka;
        cout << "Haluatko tarkistaa toisen tornin siirrot? (Y/N): ";
        cin >> jatka;

        if (jatka == 'N' || jatka == 'n') {
            break;
        }
    }
    break;
}
case 2: {
    //---
    //lähetin lailliset siirrot
    //---
    string lahettiLinjaKirjain;
    int lahettiRiviNumero, lahettiRivi, lahettiLinja, pelaajaColor;
    while (true) {
        cout << "Anna lahetin linja (a-h): ";
        cin >> lahettiLinjaKirjain;
        if (lahettiLinjaKirjain.length() != 1 || lahettiLinjaKirjain[0] < 'a' || lahettiLinjaKirjain[0] > 'h') {
            cout << "Virheellinen linja!" << endl;
            continue;
            }
        lahettiLinja = lahettiLinjaKirjain[0] - 'a';   // Muuntaa a-h → 0-7

        cout << "Anna lahetin rivi (1-8): ";
        cin >> lahettiRiviNumero;
        lahettiRivi = lahettiRiviNumero - 1;

        cout << "Anna pelaajan vari (0=valkea, 1=musta): ";
        cin >> pelaajaColor;
        if (pelaajaColor != 0 && pelaajaColor != 1) {
            cout << "Virheellinen pelaajan vari!" << endl;
            continue;
            }
        vector<Siirto> siirrot;
        asema.anna_lahetin_raakasiirrot(lahettiRivi, lahettiLinja, pelaajaColor, siirrot);

        cout << "Lahetin mahdolliset siirrot: " << endl;

        for (const auto& siirto : siirrot) {
            siirto.tulosta();
        }
        cout << endl;
        char jatka;
        cout << "Haluatko tarkistaa toisen lahetin siirrot? (Y/N): ";
        cin >> jatka;

        if (jatka == 'N' || jatka == 'n') {
            break;
        }
    }
        break;
}
case 3: {
    //---
    //kuningattaren lailliset siirrot
    //---
    string kuningatarLinjaKirjain;
    int kuningatarRiviNumero, kuningatarRivi, kuningatarLinja, pelaajaColor;
    while (true) {

        cout << "Anna kuningattaren rivi (1-8): ";
        cin >> kuningatarRiviNumero;
        kuningatarRivi = kuningatarRiviNumero - 1;

        cout << "Anna kuningattaren linja (a-h): ";
        cin >> kuningatarLinjaKirjain;
        if (kuningatarLinjaKirjain.length() != 1 || kuningatarLinjaKirjain[0] < 'a' || kuningatarLinjaKirjain[0] > 'h') {
            cout << "Virheellinen linja!" << endl;
            continue;
            }
        kuningatarLinja = kuningatarLinjaKirjain[0] - 'a';   // Muuntaa a-h → 0-7

        cout << "Anna pelaajan vari (0=valkea, 1=musta): ";
        cin >> pelaajaColor;
        if (pelaajaColor != 0 && pelaajaColor != 1) {
            cout << "Virheellinen pelaajan vari!" << endl;
            continue;
            }
        vector<Siirto> siirrot;
        asema.anna_kuningattaren_raakasiirrot(kuningatarRivi, kuningatarLinja, pelaajaColor, siirrot);

        cout << "Kuningattaren mahdolliset siirrot: " << endl;

        for (const auto& siirto : siirrot) {
            siirto.tulosta();
        }
        cout << endl;
        char jatka;
        cout << "Haluatko tarkistaa toisen kuningattaren siirrot? (Y/N): ";
        cin >> jatka;

        if (jatka == 'N' || jatka == 'n') {
            break;
        }
    }
    break;
}
case 4: {
    //---
    //ratsun lailliset siirrot
    //---
    string LinjaKirjain;
    int RiviNumero, Rivi, Linja, pelaajaColor;
    while (true) {

        cout << "Anna ratsun rivi (1-8): ";
        cin >> RiviNumero;
        Rivi = RiviNumero - 1;

        cout << "Anna ratsun linja (a-h): ";
        cin >> LinjaKirjain;
        if (LinjaKirjain.length() != 1 || LinjaKirjain[0] < 'a' || LinjaKirjain[0] > 'h') {
            cout << "Virheellinen linja!" << endl;
            continue;
            }
        Linja = LinjaKirjain[0] - 'a';   // Muuntaa a-h → 0-7

        cout << "Anna pelaajan vari (0=valkea, 1=musta): ";
        cin >> pelaajaColor;
        if (pelaajaColor != 0 && pelaajaColor != 1) {
            cout << "Virheellinen pelaajan vari!" << endl;
            continue;
            }
        vector<Siirto> siirrot;
        asema.anna_ratsun_raakasiirrot(Rivi, Linja, pelaajaColor, siirrot);

        cout << "Ratsun mahdolliset siirrot: " << endl;

        for (const auto& siirto : siirrot) {
            siirto.tulosta();
        }
        cout << endl;
        char jatka;
        cout << "Haluatko tarkistaa toisen ratsun siirrot? (Y/N): ";
        cin >> jatka;

        if (jatka == 'N' || jatka == 'n') {
            break;
        }
    }
    break;
}
case 5: {
    //---
    //kuninkaan lailliset siirrot
    //---
    string kuningasLinjaKirjain;
    int kuningasRiviNumero, kuningasRivi, kuningasLinja, pelaajaColor;
    while (true) {

        cout << "Anna kuninkaan rivi (1-8): ";
        cin >> kuningasRiviNumero;
        kuningasRivi = kuningasRiviNumero - 1;

        cout << "Anna kuninkaan linja (a-h): ";
        cin >> kuningasLinjaKirjain;
        if (kuningasLinjaKirjain.length() != 1 || kuningasLinjaKirjain[0] < 'a' || kuningasLinjaKirjain[0] > 'h') {
            cout << "Virheellinen linja!" << endl;
            continue;
            }
        kuningasLinja = kuningasLinjaKirjain[0] - 'a';   // Muuntaa a-h → 0-7

        cout << "Anna pelaajan vari (0=valkea, 1=musta): ";
        cin >> pelaajaColor;
        if (pelaajaColor != 0 && pelaajaColor != 1) {
            cout << "Virheellinen pelaajan vari!" << endl;
            continue;
            }
        vector<Siirto> siirrot;
        asema.anna_kuninkaan_raakasiirrot(kuningasRivi, kuningasLinja, pelaajaColor, siirrot);

        cout << "Kuninkaan mahdolliset siirrot: " << endl;

        for (const auto& siirto : siirrot) {
            siirto.tulosta();
        }
        cout << endl;
        char jatka;
        cout << "Haluatko tarkistaa toisen kuninkaan siirrot? (Y/N): ";
        cin >> jatka;

        if (jatka == 'N' || jatka == 'n') {
            break;
        }
    }
    break;
}
case 6:{
    string haluttu_siirto;
    while (haluttu_siirto != "lopeta"){
        cout << "Anna haluamasi siirto, muodossa esim e2e4. kirjoita 'lopeta' lopettaaksesi." << endl;
        cin >> haluttu_siirto;
        Siirto siirto(haluttu_siirto);
        asema.tee_siirto(siirto);
        asema.tulosta();
    }
    break;
    }
case 7:{
    vector<pair<int, int>> nappulapari;
    int nappula;
    string answer;
    while (true) {
    cout << "Kirjoita nappula, jonka haluat loytaa seuraavista: " << endl;
    cout << "0 = V.torni, 1 = V.ratsu, 2 = V.lahetti, 3 = V.kuningatar, 4 = V.kuningas, 5 = V.sotilas" << endl;
    cout << "6 = M.torni, 7 = M.ratsu, 8 = M.lahetti, 9 = M.kuningatar, 10 = M.kuningas, 11 = M.Sotilas, 12 = Tyhja" << endl;
    cin >> nappula;

    asema.etsi_nappula(nappula, nappulapari);
    asema.tulosta();

    cout << "haluatko tyhjentaa laudan? y/n" << endl;
    cin >> answer;
    if (answer == "y") {
        asema.tyhjenna();
        asema.tulosta();
        //asema.etsi_nappula(nappula, nappulapari);
    }
    cout << endl;
        char jatka;
        cout << "Haluatko koittaa etsia nappuloita uudestaan? (Y/N): ";
        cin >> jatka;
        if (jatka == 'N' || jatka == 'n') {
            break;
        }
    }
    break;
    }
    }
    //
}
