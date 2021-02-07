#include <iostream>


template <typename tip>
class Lista {
    public:

    Lista() {}
    virtual ~Lista() {}
    virtual int brojElemenata() const = 0;
    virtual const tip& trenutni() const = 0;
    virtual tip& trenutni() = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const tip& el) = 0;
    virtual void dodajIza(const tip& el) = 0;
    virtual void ispisi() = 0;
    virtual const tip&  operator[] ( int i ) const = 0;
    virtual tip& operator[] (int i ) = 0;
};


template<typename tip>
class DvostrukaLista: public Lista<tip> {
    private:
    struct Cvor {
    tip vrijednost;
    Cvor *sljedeci;
    Cvor *prethodni;
    Cvor( const tip& element, Cvor *sljedeci, Cvor *prethodni): vrijednost(element), sljedeci(sljedeci), prethodni(prethodni) {}
};

    int lduzina, dduzina;
    Cvor *prvi;
    Cvor *zadnji;
    Cvor *tekuci;
    public:
    DvostrukaLista(const DvostrukaLista<tip> &niz);
    DvostrukaLista &operator = (const DvostrukaLista<tip> &niz);
    DvostrukaLista(): lduzina(0), dduzina(0), prvi(0), zadnji(0), tekuci(0) {}
   ~DvostrukaLista();

    int brojElemenata() const override { return lduzina + dduzina;}
    const tip& trenutni() const override;
    tip &trenutni() override;
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override;
    void kraj() override;
    void obrisi() override;
    void dodajIspred(const tip& el) override;
    void dodajIza(const tip& el) override;
     const tip& operator[] ( int k) const override;
    tip& operator[] ( int k) override;
    void ispisi() override;

};
template<typename tip> // ne rade
DvostrukaLista<tip>::DvostrukaLista(const DvostrukaLista<tip> &niz) {
    prvi = zadnji = tekuci = 0;
    lduzina = dduzina = 0;
    Cvor *p(niz.prvi);
    while( p != 0)
    {
        dodajIza(p->vrijednost);
        sljedeci();
        p = p->sljedeci;
    }
}

template<typename tip> // ne rade
DvostrukaLista<tip> &DvostrukaLista<tip>:: operator = (const DvostrukaLista &lista) {
    if( &lista == this) return *this;

    Cvor *pomoc = prvi;
    Cvor *temp;
    while( pomoc != 0)
    {
        temp = pomoc->sljedeci;
        delete pomoc;
        pomoc = temp;
    }
    Cvor *p(lista.prvi);
    while( p != 0) {
        dodajIza(p->vrijednost);
        sljedeci();
        p = p->sljedeci;
    }
    return *this;

}
template <typename tip>
void DvostrukaLista<tip>::obrisi() {


     if( !lduzina) {
        Cvor *temp = tekuci;
        tekuci = tekuci->sljedeci;
        prvi = prvi->sljedeci;
        delete temp; temp = nullptr;
        dduzina--;
        //tekuci->prethodni = 0;
        //prvi->prethodni = 0;
    }
    else if( dduzina == 1 && brojElemenata() == 2)
    {
       Cvor *temp = tekuci;
       delete temp; temp = 0;
       tekuci = prvi; tekuci->sljedeci = 0; tekuci->prethodni = 0;
       zadnji = prvi; zadnji->sljedeci = 0; tekuci->prethodni = 0;
        lduzina--;


    }
    else if( dduzina == 1) // JEDINI SLUCAJ GDJE KOMPLEKSNOST NIJE O(1);
    {
        Cvor *temp = prvi;

        while( temp->sljedeci != tekuci->prethodni) temp = temp->sljedeci;

        Cvor *brisi = tekuci;
        delete brisi; brisi = 0;
        tekuci = temp->sljedeci; tekuci->prethodni = temp; tekuci->sljedeci = 0;
        zadnji = temp->sljedeci; zadnji->prethodni = temp; zadnji->sljedeci = 0;

        lduzina--;
    }
    else
    {
        Cvor *preusmjeri = tekuci->prethodni; preusmjeri->sljedeci = tekuci->sljedeci;
        Cvor *brisi = tekuci;
        tekuci = tekuci->sljedeci;
        tekuci->prethodni = preusmjeri;
        delete brisi; brisi = nullptr;
        dduzina--;
    }

}


template <typename tip>
const tip& DvostrukaLista<tip>::operator[] (int k) const {
  int i = 0;
   for(auto p = prvi; p != 0; p = p->sljedeci)
   {
       if( i == k) return p->vrijednost;
       i++;
   }

   return tekuci->vrijednost;
}

template <typename tip>
tip& DvostrukaLista<tip>::operator [] ( int k) {
   int i = 0;
   for(auto p = prvi; p != 0; p = p->sljedeci)
   {
       if( i == k) return p->vrijednost;
       i++;
   }

   return tekuci->vrijednost;
}


template <typename tip>
DvostrukaLista<tip>::~DvostrukaLista() {
    Cvor *pomoc = prvi;
    Cvor *temp;
    while( pomoc != 0)
    {
        temp = pomoc->sljedeci;
        delete pomoc;
        pomoc = temp;
    }
}

template<typename tip>
const tip& DvostrukaLista<tip>::trenutni() const  {
    if( !brojElemenata()) throw;
    return tekuci->vrijednost;
}

template<typename tip>
tip& DvostrukaLista<tip>::trenutni() {
    if( !brojElemenata()) throw;
    return tekuci->vrijednost;
}

template<typename tip>
bool DvostrukaLista<tip>::prethodni()  {

    if( !brojElemenata()) throw;

    if( lduzina >= 1) {
        if( lduzina == 1)
        tekuci = prvi;

        else
        tekuci = tekuci->prethodni;


        lduzina--;
        dduzina++;
        return true;
    }
    return false;
}

template<typename tip>
bool DvostrukaLista<tip>::sljedeci()  {
    if( !brojElemenata()) throw;

    if( dduzina == 1) return false;
    tekuci = tekuci->sljedeci;
    lduzina++;
    dduzina--;
    return true;
}

template<typename tip>
void DvostrukaLista<tip>::pocetak() {
    if( !brojElemenata()) throw;

    tekuci = prvi;
    dduzina = brojElemenata();
    lduzina = 0;
}

template<typename tip>
void DvostrukaLista<tip>::kraj() {

   if( !brojElemenata()) throw;
   tekuci = zadnji;
   lduzina = brojElemenata() - 1;
   dduzina = 1;

}

template<typename tip>// radi bez greske
void DvostrukaLista<tip>::dodajIspred(const tip& el) {

    Cvor *p = new Cvor(el, 0, 0);

    if( !brojElemenata())
    {
        prvi = p;
        prvi->prethodni = 0;
        zadnji = p; zadnji->sljedeci = 0; zadnji->prethodni = 0;
        tekuci = p; tekuci->prethodni = 0; tekuci->sljedeci = 0;
        dduzina++;
    }
    else if( !lduzina)
    {
        prvi = p; prvi->sljedeci = tekuci; prvi->prethodni = 0;
        tekuci->prethodni = prvi;

        lduzina++;
    }
    else
    {
        Cvor *temp = tekuci->prethodni;
        temp->sljedeci = p;
        p->sljedeci = tekuci;
        p->prethodni = temp;
        tekuci->prethodni = p;
        lduzina++;
    }

}

template <typename tip> // radi bez greske
void DvostrukaLista<tip>::dodajIza(const tip& el) {
    Cvor *p = new Cvor(el, 0,0);

    if( !brojElemenata())
    {
        prvi = p;
        prvi->prethodni = 0;
        zadnji = p; zadnji->sljedeci = 0; zadnji->prethodni = 0;
        tekuci = p; tekuci->prethodni = 0; tekuci->sljedeci = 0;
        dduzina++;
    }
    else if( dduzina == 1)
    {
        p->prethodni = prvi;
        zadnji = p; zadnji->sljedeci = 0; zadnji->prethodni = tekuci;
        tekuci->sljedeci = p;
        dduzina++;
    }
    else
    {
        Cvor *temp = tekuci->sljedeci;
        temp->prethodni = p;
        p->sljedeci = temp;
        tekuci->sljedeci = p;
        dduzina++;
    }
}

template<typename tip>
void DvostrukaLista<tip>::ispisi() {
    for(auto p = prvi; p != 0; p = p->sljedeci)
    std:: cout << p->vrijednost << " ";

    std:: cout  << std::endl;
}

template<typename tip>
tip dajMaksimum(const Lista<tip> &n)
{
    DvostrukaLista<tip> pomocna = DvostrukaLista<tip>(*((DvostrukaLista<tip>*)(&n)));
    pomocna.pocetak();
    tip max = pomocna.trenutni();
    pomocna.sljedeci();
    for(int i = 1; i < pomocna.brojElemenata(); i++) {
       if( pomocna.trenutni() > max) max = pomocna.trenutni();
       pomocna.sljedeci();
   }
    return max;
}


//test dodajIza
void testDodajIza() {
    DvostrukaLista<int> dvostruka;
    for(int i = 1; i <= 10; i++) dvostruka.dodajIza(i);

    dvostruka.kraj();
    dvostruka.dodajIza(132);

    dvostruka.ispisi();
}

//test dodajIza
void testDodajIza2() {
    DvostrukaLista<int> dvostruka;
    for(int i = 1; i <= 15; i++) dvostruka.dodajIza(i);

    dvostruka.ispisi(); std:: cout << std::endl;

    dvostruka.kraj();
    std:: cout << "Zadnji: " << dvostruka.trenutni() << std::endl;

    dvostruka.prethodni();
    dvostruka.prethodni();
    std:: cout << dvostruka.trenutni();
}

//test dodajIspred() {
void testDodajIspred() {
    DvostrukaLista<int> lista;
    for(int i = 1; i <= 15; i++) lista.dodajIspred(i);

    lista.kraj();
    lista.dodajIspred(12);

    lista.ispisi();

}

//test dodajIspred
void testDodajIspred2() {
    DvostrukaLista<int> dvostruka;
    for(int i = 1; i <= 15; i++) dvostruka.dodajIspred(i);

    dvostruka.ispisi(); std:: cout << std::endl;

    dvostruka.kraj();
    std:: cout << "Zadnji: " << dvostruka.trenutni() << std::endl;

    dvostruka.prethodni();
    dvostruka.prethodni();
    std:: cout << dvostruka.trenutni();


}

//test kraj
void testKraj() {
    DvostrukaLista<int> lista;
    for(int i = 1; i <= 15; i++) lista.dodajIspred(i);

    //zadnji je tekuci

    std:: cout << lista.trenutni() << " = ";
    lista.kraj();
    std:: cout << lista.trenutni() << std::endl;
}

//test pocetak
void testPocetak() {
    DvostrukaLista<int> lista;
    for(int i = 1; i <= 15; i++) lista.dodajIspred(i);

    lista.pocetak();

    std:: cout << lista.trenutni() << " = " << lista[0] << std::endl;


}

//test sljedeci/prethodni
void testSljedeciPrethodni() {
    DvostrukaLista<int> lista;
    for(int i = 1; i <= 15; i++)
    lista.dodajIspred(i);

    for(int i = 1; i <= 14; i++) lista.prethodni();

    std:: cout << lista[0] << " = " << lista.trenutni() << std::endl;

    for(int i = 1; i <= 14; i++) lista.sljedeci();

    std:: cout << lista[14]  << " = " << lista.trenutni() << std::endl;

}

//test referenca na trenutni

void testReferencaNaTrenutni() {
    DvostrukaLista<int> lista;
    for(int i = 1; i <= 15; i++) lista.dodajIspred(i);

    std::cout << "Trenutni prije promjene: " << lista.trenutni() << std::endl;

    lista.trenutni() = 1345;

    std:: cout << "Trenuti poslije promjene: " << lista.trenutni() << std::endl;
}

//test operatora []
void testOperatora() {
    DvostrukaLista<int> lista;
    for(int i = 1; i <= 15; i++) lista.dodajIspred(i);

    std:: cout << lista[14] << " = " << lista.trenutni() << std::endl;
    lista[14] = 1789;
    std:: cout << lista[14] << " = " << lista.trenutni() << std::endl;
}

//test brisanja trenutnog

void testBrisanjaTrenutnog() {
    DvostrukaLista<int> lista;

    for(int i = 1; i <= 15; i++) lista.dodajIspred(i);
    lista.ispisi();
    std:: cout << "Trenutni je: " << lista.trenutni() << std::endl;

    lista.obrisi();

    std:: cout << "Trenutni je: " << lista.trenutni() << std::endl;

}

//test brisanja trenutnog
void testBrisanjaTrenutnog2() {
    DvostrukaLista<int> lista;
    for(int i = 1; i <= 1000; i++) lista.dodajIspred(i);

    for(int i = 1; i <= 999; i++) lista.obrisi();


    std:: cout << lista.trenutni() << std::endl;
    lista.obrisi();

    std:: cout << lista.brojElemenata() << std::endl;

}

//test kopiracujeg konstruktora i operatora dodjele
void testCopyCtorOperatorDodjele() {
    DvostrukaLista<int> lista;
    DvostrukaLista<int> testna;
    for(int i = 1; i <= 10; i++) lista.dodajIspred(i);
    for(int i = 10; i >= 1; i--) testna.dodajIza(i);
    DvostrukaLista<int> lista_nova = lista;

    (lista_nova = lista_nova).brojElemenata();

    std:: cout << "Pocetna lista: "; lista.ispisi();
    std:: cout << "Prekopirana u novu: "; lista_nova.ispisi();

}


int main() {


    return 0;
}
