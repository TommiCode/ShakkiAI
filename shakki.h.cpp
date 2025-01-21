#pragma once
#include <iostream>
using namespace std;

// Pelaajat.
enum { VALKEA, MUSTA };

// Vakioarvot eri nappululoille (NA = tyhj� ruutu).
enum { wR, wN, wB, wQ, wK, wP, bR, bN, bB, bQ, bK, bP, NA};

// Palauttaa annetun nappulan v�rin (VALKEA, MUSTA).
int nappulan_vari(int nappula) {
if (nappula >= wR && nappula <= wP) return VALKEA;
if (nappula >= bR && nappula <= bP) return MUSTA;
return -1;
}

// Palauttaa pelaajan vastustajan.
int vastustaja(int pelaaja) {
    return (pelaaja == VALKEA) ? MUSTA : VALKEA;
}
