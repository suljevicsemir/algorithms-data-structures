#include <iostream>

template <typename tip>
class Red {
    private:
    struct Cvor {
        tip element;
        Cvor *sljedeci;
        Cvor(const tip &element, Cvor *sljedeci) : element(element), sljedeci(sljedeci){}
    };
    Cvor *pocetak;
    Cvor *kraj;
    int velicina;
    void unisti();
    void kopiraj(const Red &red);
    bool isEmpty() const {
        return pocetak == 0;
    }
    public:
    Red(): pocetak(0), kraj(0), velicina(0) {}
    Red(const Red &red) { kopiraj(red); }
    ~Red() {unisti();}
    void brisi() {unisti();}
    void stavi(const tip& element);
    tip skini();
    tip& celo() {
        return pocetak->element;
    }
    int brojElemenata() const { return velicina; }

    Red &operator =( const Red &red);
    void ispisi() {
        Cvor *p(pocetak);
        while( p != 0) {
            std:: cout << p->element << " ";
            p = p->sljedeci;
        }
        std:: cout << std::endl;
    }
};

template<typename tip>
void Red<tip>::kopiraj(const Red &red) {
    pocetak = kraj = 0;
    velicina = 0;
    Cvor *p(red.pocetak);
    while (p != 0) {
        stavi(p->element);
        p = p->sljedeci;
    }
}

template<typename tip>
Red<tip> &Red<tip>:: operator = (const Red& red ) {
    if( &red == this) return *this;

    unisti();
    kopiraj(red);
    return *this;
}

template<typename tip>
void Red<tip>::unisti() {
    while(!isEmpty()) skini();
}

template<typename tip>
void Red<tip>::stavi(const tip &element) {
    Cvor *n(new Cvor(element, 0));
    if( pocetak == 0) pocetak = kraj = n;
    else {
        kraj->sljedeci = n;
        kraj = kraj->sljedeci;
    }
    velicina++;
}

template<typename tip>
tip Red<tip>::skini() {
    if( isEmpty()) throw "\nRed je prazan!\n";

    tip element(pocetak->element);
    Cvor *p = pocetak;
    if( pocetak == kraj)
        pocetak = kraj = 0;
    else
        pocetak = pocetak->sljedeci;

    delete p;
    velicina--;
    return element;
}

//test konstruktora
void test1() {
    Red<int> red;
    std:: cout << "Broj elemenata praznog reda: " << red.brojElemenata() << std::endl;

}
// test copy ctor
void test2() {
   Red<int> red;
   for(int i = 1; i <= 10;  i++) red.stavi(i);

   Red<int> novi(red);

   red.ispisi(); std:: cout << std::endl;
   novi.ispisi(); std:: cout << std::endl;

}

// test operator dodjele
void test3() {
    Red<int> red;
    for(int i = 1; i <= 10; i++) red.stavi(i);

    Red<int> novi;
    novi = red;

    red.ispisi();
    std:: cout << std::endl;
    novi.ispisi(); std:: cout << std::endl;
}
//test skini
void test4() {
    Red<int> red;
    for(int i = 1; i <= 10; i++) red.stavi(i);

    int a = red.skini(); // 1

    std:: cout << a << "  =  1" << " "  << std::endl;
    std:: cout << "Broj elemenata je sada: " << red.brojElemenata() << std::endl;

}

//test stavi i skini
void test5() {
    Red<int> red;
    for(int i = 1; i <= 10; i++) red.stavi(i);

    std:: cout << "Broj elemenata: " << red.brojElemenata() << std::endl;
    int pokupi = red.skini();
    red.stavi(pokupi);
    std:: cout << "Broj elemenata: " << red.brojElemenata() << std::endl;
    for(int i = 1; i <= 9; i++) red.skini();

    std:: cout << pokupi << " = " << red.skini();


}

//test celo
void test6() {
    Red<int> red;
    red.stavi(4);
    red.stavi(5);

    std:: cout << red.celo(); red.skini(); std:: cout << ", " << red.skini();

}

//unisti je u destruktoru, privatna metoda, ne mogu test napisat
int main() {


    return 0;

}
