#include <iostream>
#include <fstream>
#include <ctime>
template <typename tip>
void bubble_sort(tip *niz, int vel) {
    for(int i = vel - 1; i >= 1; i--) {
        for(int j = 1; j <= i; j++) {
            if( niz[j - 1] > niz[j] ) {
                tip priv = niz[j - 1];
                niz[j - 1] = niz[j];
                niz[j] = priv;
            }
        }
    }
}

template <typename tip>
void selection_sort(tip *niz, int vel) {
    for(int i = 0; i <= vel - 2; i++) {
        tip min = niz[i];
        int pmin = i;
        for(int j = i + 1; j <= vel - 1; j++) {
            if( niz[j] < min) {
                min = niz[j];
                pmin = j;
            }
        }
        niz[pmin] = niz[i];
        niz[i] = min;
    }
}

template <typename tip>
void quick_sort(tip *niz, int vel) {
    tip pivot = niz[vel - 1];
    int indeksNajveceg = 0;

    for(int i = 0; i < vel - 1; i++) {

        if( niz[i] < pivot) {
            // zamjeni najveci s ovim
            tip priv = niz[i];
            niz[i] = niz[indeksNajveceg];
            niz[indeksNajveceg] = priv;
            indeksNajveceg++;
        }
    }

    tip priv = niz[indeksNajveceg];
    niz[indeksNajveceg] = niz[vel - 1];
    niz[vel - 1] = priv;
    if( indeksNajveceg > 1)
        quick_sort(niz, indeksNajveceg);

    if( vel - indeksNajveceg - 1 > 1)
        quick_sort(niz + indeksNajveceg + 1, vel - indeksNajveceg - 1);
}


template <typename tip>
void merge(tip *niz, const int l, const int p, const int q, const int u) {
    int i = 0, j = q - l , k = l;
    tip *B = new tip[u - l + 1];
    for(int m = 0; m <= u - l; m++)
    B[m] = niz[l + m];

    while( i <= p - l && j <= u - l) {
        if( B[i] < B[j])
            niz[k] = B[i++];
        else
            niz[k] = B[j++];

        k++;
    }

    while( i <= p - l) niz[k++] = B[i++];
    while( j <= u - l) niz[k++] = B[j++];

    delete[] B;

}

template<typename tip>
void moj_merge(tip *niz, const int l, const  int u) {
    if( u > l) {
        int p = (l + u - 1)/2;
        int q = p + 1;
        moj_merge(niz, l, p);
        moj_merge(niz, q, u);
        merge(niz, l, p, q, u);
    }
}

template <typename tip>
void merge_sort(tip *niz, int vel) {

   /*( vel  > 0) {
       int p = (vel - 1)/2;
       int q = p + 1;
       merge_sort(niz, p);
       merge_sort(niz + q , vel - 1);
      merge(niz, 0, p, q, vel - 1);
   } */

   moj_merge(niz, 0, vel - 1 );

}

void ucitaj(std::string filename, int *&niz, int &vel) {
    std::ifstream ulazni_tok(filename);
    if(!ulazni_tok) {
        std:: cout << "Došlo je do greške pri otvaranju datoteke!";
        return;
    }

    int broj1 = 0;

    int brojac = 0;
    while( ulazni_tok >> broj1) {
        //std:: cout << broj1 << " ";
        brojac++;
    }

    vel = brojac;
    ulazni_tok.close();
    ulazni_tok.open(filename);

    niz = new int[brojac];

    int k = 0;
    while( ulazni_tok >> broj1) {
        niz[k++] = broj1;
    }
    merge_sort(niz, vel);
}

void generisi(std::string filename, int vel) {
    std::ofstream izlazni_tok(filename);

    if( !izlazni_tok) {
        std:: cout << "Došlo je do greške pri otvaranju datoteke!";
        return;
    }

    for(int i = 0; i < vel/2; i++) {
        izlazni_tok << (rand() % 10) << " ";
    }
    for(int i = vel/2 + 1; i < vel; i++) {
        if( i != vel - 1)
        izlazni_tok << (rand() % 100) << " ";
        else
        izlazni_tok << (rand() % 100);
    }



}

void funkcija(std::string citaj_iz, std::string upisi_u, int brojElemenata, int *& niz) {
    std::ifstream ulazni_tok(citaj_iz);
    if(!ulazni_tok) {
        std::cout << "Došlo je do greške pri otvaranju datoteke";
        return;
    }

    int brojac = 0;
    int broj = 0;
    while(ulazni_tok >> broj) {
        brojac++;
    }

    niz = new int[brojac];
    ulazni_tok.close();
    ulazni_tok.open(citaj_iz);

    int k = 0;
    while(ulazni_tok >> broj) {
        niz[k++] = broj;
    }

    std:: cout << "Koji algoritam želite za sortiranje ?" << std::endl;
    std:: cout << "Pritisnite 1 za bubble sort." << std::endl;
    std:: cout << "Pritisnite 2 za selection sort." << std::endl;
    std:: cout << "Pritisnite 3 za quick sort." << std::endl;
    std:: cout << "Pritisnite 4 za merge sort." << std::endl;

    int unos;
    std:: cout << "Unesite izbor: "; std:: cin >> unos;
    clock_t prije_sorta = clock();
    if( unos == 1) {
        bubble_sort(niz, k);
    }
    if( unos == 2) {
        selection_sort(niz, k);
    }
    if( unos == 3) {
        quick_sort(niz, k);
    }
    if( unos == 4) {
        merge_sort(niz, k);
    }
    clock_t poslije_sorta = clock();

    int vrijeme = (poslije_sorta - prije_sorta) /(CLOCKS_PER_SEC / 1000);
    std:: cout << "Vrijeme izvršavanja: " << vrijeme << std::endl;

    for(int i = 0; i < k - 1 ; i++) {
        if( niz[i] >= niz[i + 1]) {
            std:: cout << "Greška u sortiranju!";
            delete[] niz; niz = nullptr;
            return;
        }
    }

    std::ofstream izlazni_tok(upisi_u);
    if( !izlazni_tok) {
        std:: cout << "Došlo je do greške pri otvaranju datoteke!";
        izlazni_tok.close();
        return;
    }

    for(int i = 0; i < k; i++) {
        izlazni_tok << niz[i] << " ";
    }

    ulazni_tok.close();
    izlazni_tok.close();

    delete[] niz;


}

int main() {


    return 0;
}

