#include <iostream>

using namespace std;

int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
#include <iostream>
#include <ctime>
#include <typeinfo>
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
    virtual void ispisi()  = 0;
    virtual const tip& operator [] (int i) const = 0;
    virtual tip& operator[] (int i) = 0;
};

template <typename tip>
class NizLista: public Lista<tip>{
  private:
  int duzina;
  int tekuci;
  int laznjak;
  tip *p;
  public:
  NizLista(): duzina(0), tekuci(0), p(0) {}
 ~NizLista()
 {
   delete[] p; p = nullptr;
 }


  NizLista(const NizLista<tip> &niz2) {
    p = new tip[niz2.duzina];
    for(int i = 0; i < niz2.duzina; i++)
    p[i] = niz2.p[i];

    duzina = niz2.duzina;
    tekuci = 0;
  }
  NizLista(NizLista &&lista)
  {
    p = lista.p;
    delete[] lista.p; lista.p = nullptr;
    duzina = lista.duzina;
    tekuci = 0;
    lista.duzina = 0;
    lista.tekuci = 0;
  }
  NizLista &operator =(const NizLista &lista)
  {
    delete[] p; p = nullptr;
    p = new tip[100005]();
    for(int i = 0; i < lista.duzina; i++)
    p[i] = lista.p[i];

    tekuci = 0;
    duzina = lista.duzina;

    return *this;
  }
  NizLista &operator =(NizLista &&lista)
  {
    delete[] p;
    p = std::move(lista.p); // ne piše da nije dozvoljeno, implementirano zbog move semantike
    tekuci = 0;
    duzina = lista.duzina;
  }

  int brojElemenata() const override { return duzina; }

  const tip& trenutni() const override {
      if( duzina == 0) throw ("Prazna lista");
      return p[tekuci];
  }
  tip &trenutni() override {
      if( duzina == 0) throw ("Prazna lista");
      return p[tekuci];
  }
  bool prethodni() override {
      if( duzina == 0) throw ("Prazna lista");
      if( !tekuci ) return false;

      tekuci--;
      return true;

  }
  bool sljedeci() override {
      if( duzina == 0) throw ("Prazna lista");
     if( tekuci == duzina - 1) return false;

      tekuci++;
      return true;
  }
  void pocetak() override {
      if( duzina == 0) throw ("Prazna lista");
      tekuci = 0;
  }
  void kraj() override {
      if( duzina == 0) throw ("Prazna lista");
      tekuci = duzina - 1;
  }
  void obrisi() override {
      if( duzina == 0) throw ("Prazna lista");

      if( duzina == 1)
      {
        delete[] p; p = nullptr;
        duzina--;
        return;
      }
      if( tekuci == duzina - 1)
      {
        duzina--;
        tekuci--;
        return;
      }

        for(int j = tekuci; j < duzina - 1; j++)
        {
          p[j] = p[j + 1];
        }

      duzina--;

  }

  void dodajIspred(const tip &el) override
  {
         if( duzina == 0)
         {

           p = new tip[100005]();
           p[duzina] = el;
           duzina = 1;
           tekuci = 0;
           return;
         }
           /*
          tip* temp = new tip[duzina + 1];
           for(int i = 0; i < tekuci; i++)
           {
             temp[i] = p[i];
           }

           temp[tekuci] = el;

           for(int i = tekuci; i < duzina; i++)
           {
             temp[i + 1] = p[i];
           }



          delete[] p;
          p = temp;
          duzina++;
          tekuci++;*/

         for(int i = duzina - 1; i >= tekuci; i--)
         p[i + 1] = p[i];

         p[tekuci] = el;
         tekuci++;
         duzina++;
  }
  void dodajIza(const tip &el) override {
    if( duzina == 0) {
      p = new tip[100005]();
      p[duzina] = el;
      duzina = 1;
      tekuci = 0;
      return;
    }
    /*
    tip *temp = new tip[duzina + 1];

    for(int i = 0; i <= tekuci; i++)
    temp[i] = p[i];

    temp[tekuci + 1] = el;
    for(int i = tekuci + 1; i < duzina; i++)
    temp[i + 1] = p[i];

    delete[] p;
    p = temp;
    duzina++;
    */

    for(int i = duzina - 1; i > tekuci; i--)
    p[i + 1] = p[i];

    p[tekuci + 1] = el;
    duzina++;
  }
  void ispisi()  override
  {
     for(int i = 0; i < duzina; i++)
     std:: cout << p[i] << " ";
  }

  const tip& operator[] (int i) const override
  {
    if( i < 0 || i >= duzina) throw;
    return p[i];
  }
  tip& operator[] ( int i) override
  {
    if( i < 0 || i >= duzina) throw;
    return p[i];
  }

};

//ctor
void test1() {
  NizLista<int> nizlista;

  std:: cout << nizlista.brojElemenata();
}

// kopirajuci ctor
void test2() {
  NizLista<int> nizlista;
  for(int i = 1; i <= 15; i++) nizlista.dodajIspred(i);

  NizLista<int> nova(nizlista);

  nizlista.ispisi(); std:: cout << std::endl;
  nova.ispisi(); std:: cout << std::endl;
}

// kopirajuci operator dodjele
void test3() {
 NizLista<int> nizLista;
  for(int i = 1; i <= 15; i++) {
    nizLista.dodajIspred(i);
  }

  NizLista<int> nova;
  nova = nizLista;
  std:: cout << nizLista.brojElemenata() << " = " << nova.brojElemenata() << std::endl;

}

// test broj elemenata
void test4() {
  NizLista<int> nizLista;
  for(int i = 0; i < 15; i++) nizLista.dodajIza(i);

  std:: cout << nizLista.brojElemenata() << std::endl;
}

// test trenutni
void test5() {
  NizLista<int> nizLista;
  for(int i = 0; i <15; i++) nizLista.dodajIspred(i);

  std:: cout << nizLista.trenutni() << std::endl;

  nizLista.trenutni() = 138;

  std:: cout << nizLista.trenutni() << std::endl;
}

// test sljedeci i prethodni i trenutni zajedno
void test6() {
  NizLista<int> nizLista;
  for(int i = 0; i < 15; i++) nizLista.dodajIza(i);

  nizLista.sljedeci(); std:: cout << nizLista.trenutni() << " ";
  nizLista.prethodni(); std:: cout << nizLista.trenutni() << std::endl;
}

//test pocetak i kraj
void test7() {
  NizLista<int> nizLista;
  for(int i = 1; i < 16; i++) nizLista.dodajIspred(i);

  nizLista.pocetak();
  std:: cout << nizLista.trenutni();
  nizLista.kraj();
  std:: cout << " " << nizLista.trenutni() << std::endl;

}

// test brisi
void test8() {
  NizLista<int> nizLista;
  for(int i = 1; i < 15; i++) nizLista.dodajIspred(i);

  nizLista.pocetak();
  nizLista.obrisi();
  nizLista.kraj();
  nizLista.obrisi();
  std:: cout << nizLista.trenutni();

}

//test operatora[]

void test9() {
  NizLista<int> nizLista;
  for(int i = 1; i <= 15; i++) nizLista.dodajIspred(i);

  nizLista.pocetak();
  std:: cout << nizLista.trenutni() << " = " << nizLista[0] << std::endl;
  nizLista[0] = 135;
  nizLista.sljedeci();
  std:: cout << nizLista.trenutni() << " = " << nizLista[0] << std::endl;

}
template <typename tip>
class JednostrukaLista: public Lista<tip> {
  private:
  struct Cvor {
    tip vrijednost;
    Cvor *sljedeci;
    Cvor( const tip &element, Cvor * sljedeci):
    vrijednost(element), sljedeci(sljedeci) {}
  };

  int lduzina, dduzina;
  Cvor *prvi;
  Cvor *zadnji;
  Cvor *tekuci;
  public:

  JednostrukaLista(const JednostrukaLista<tip> &niz2){
    lduzina = dduzina = 0;
    prvi = zadnji = tekuci = 0;

    Cvor *p(niz2.prvi);

    while (p != 0) {
      dodajIza(p->vrijednost);
      p = p->sljedeci;
    }
  }


  JednostrukaLista &operator = (const JednostrukaLista &niz2)
  {
    while (prvi != 0) {
      tekuci = prvi;
      prvi = prvi->sljedeci;
      delete tekuci; tekuci = nullptr;
    }

  Cvor *p(niz2.prvi);
  prvi = tekuci = zadnji = 0;
  dduzina = lduzina = 0;
  while( p != 0) {
    dodajIza(p->vrijednost);
    p = p->sljedeci;
  }
   return *this;

  }
  JednostrukaLista(): lduzina(0), dduzina(0), prvi(0), zadnji(0), tekuci(0) {}
  ~JednostrukaLista(){

    while(  prvi != 0)
    {
      tekuci = prvi;
      prvi = prvi->sljedeci;
      delete tekuci;
    }
  }


  int brojElemenata() const override {return lduzina + dduzina;}
  const tip& trenutni() const override{
    if( lduzina + dduzina == 0) throw ("Ništa za vratiti");
    return tekuci->vrijednost;
  }
  bool prethodni() override  {
    if( dduzina + lduzina == 0) throw ("Prazna lista!");

    if( lduzina  >= 1)
    {
      if( lduzina == 1)
      tekuci = prvi;
      else
      {
        Cvor *klizni = prvi;
        while( klizni->sljedeci != tekuci) klizni = klizni->sljedeci;

        tekuci = klizni;
      }
      lduzina--;
      dduzina++;
      return true;
    }
    return false;

  }

  bool sljedeci() override {
    if( brojElemenata() == 0) throw ("Prazna lista!");


    if( dduzina == 1) return false;

      tekuci = tekuci->sljedeci;
      lduzina++;
      dduzina--;
      return true;
  }
  void pocetak() override {
    tekuci = prvi;
    dduzina = brojElemenata();
    lduzina = 0;
  }
  void kraj() override {
    if( brojElemenata() == 0) throw ("Prazna lista!");

    int k = 0;
    for(auto p = prvi; p != 0; p = p->sljedeci)
    {
      if( k == brojElemenata() - 1)
      tekuci = p;

      k++;
    }


  lduzina = brojElemenata() - 1;
  dduzina = 1;
  }

  tip &trenutni() override {
    if( lduzina + dduzina == 0) throw ("Ništa za vratiti");
    return tekuci->vrijednost;
  } //

  void obrisi() override {
    if( brojElemenata() == 0) throw ("Prazna lista!");

    if( dduzina == 1 && brojElemenata() != 1) // zadnji za vise jednog elementa
    {

      Cvor *klizni = prvi;
      while( klizni->sljedeci != tekuci) klizni = klizni->sljedeci;

      tekuci = klizni;
      delete tekuci->sljedeci; tekuci->sljedeci = nullptr;

      lduzina--;
      return;
    }
    if( brojElemenata() == 1)
    {
      delete prvi; prvi = nullptr;
      dduzina--;

      return;
    }

    if( lduzina == 0 && dduzina > 1)
    {
      Cvor *zapamti = tekuci->sljedeci;
      delete tekuci; tekuci = nullptr;
      tekuci = zapamti;
      prvi = zapamti;

      dduzina--;
      return;
    }

    if( lduzina != 0 && dduzina != 0)
    {
      Cvor *pomoc = prvi;
      while( pomoc->sljedeci != tekuci) pomoc = pomoc->sljedeci;

      pomoc->sljedeci = tekuci->sljedeci;
      delete tekuci;
      tekuci = pomoc->sljedeci;

      lduzina--;

    }


  }

  void dodajIspred(const tip& el) override {
    Cvor *p = new Cvor(el, 0);

   if( brojElemenata() == 0)
   {
     tekuci = p;
      tekuci->sljedeci = 0;
      prvi = p;
      prvi->sljedeci = 0;
      dduzina++;
   }
   else if( brojElemenata() == 1)
   {
     prvi = p;
     p->sljedeci = tekuci;
     tekuci->sljedeci = 0;
     lduzina++;
   }
   else
   {
      Cvor *klizni = prvi;
      while( klizni->sljedeci != tekuci) klizni = klizni->sljedeci;
      p->sljedeci = tekuci;
      klizni->sljedeci = p;
      lduzina++;

   }


  }

  void dodajIza(const tip& el) override {
    Cvor *p = new Cvor(el, 0);
    if( brojElemenata() == 0)
    {
      tekuci = p;
      tekuci->sljedeci = 0;
      prvi = p;
      prvi->sljedeci = 0;
      dduzina++;
    }
    else if( dduzina == 1)
    {
      tekuci->sljedeci = p;
      p->sljedeci = 0;
      dduzina++;
    }
    else
    {
      p->sljedeci = tekuci->sljedeci;
      tekuci->sljedeci = p;
      dduzina++;
    }

  }

  const tip& operator[] (int k) const override
  {
    if( k < 0 || k >= brojElemenata()) throw;

    int brojac = 0;
    for(auto p = prvi; p != 0; p = p->sljedeci)
    {
      if( brojac == k) return p->vrijednost;
      brojac++;
    }

  }

  tip & operator[] ( int k) override
  {
    if( k < 0 || k >= brojElemenata()) throw;


    int brojac = 0;
    for(auto p = prvi; p != 0; p = p->sljedeci)
    {
      if( brojac == k)
      return p->vrijednost;

      brojac++;
    }

  }

  void ispisi()  override
  {
   for(auto p = prvi; p != 0; p = p->sljedeci)
   std:: cout << p->vrijednost << " ";
  }
};


//test ctor
void testa() {
  JednostrukaLista<int> lista;

  std:: cout << lista.brojElemenata() << std::endl;
}

//kopirajuci ctor
void testb() {
  JednostrukaLista<int> lista;
  for(int i = 1; i <= 15; i++) lista.dodajIspred(i);

  JednostrukaLista<int> nova(lista);

  std:: cout << nova.brojElemenata() << " = " << lista.brojElemenata() << std::endl;
}

// kopirajuci operator dodjele
void testc() {
  JednostrukaLista<int> lista;
  for(int i = 1; i <= 15; i++) lista.dodajIspred(i);

  JednostrukaLista<int> nova;
  nova = lista;

  std:: cout << lista.brojElemenata() << " = " << nova.brojElemenata() << std::endl;
}

// test broj elemenata
void testd() {
  JednostrukaLista<int> lista;

  for(int i = 1; i <= 15; i++) {
    lista.dodajIspred(i);
  }

  std:: cout << lista.brojElemenata() << std::endl;

}

// test trenutni sljedeci i prethodni
void teste() {
  JednostrukaLista<int> lista;
  for(int i = 1; i <= 15; i++) lista.dodajIspred(i);

  std:: cout << lista.trenutni();

  lista.prethodni(); std:: cout << " " << lista.trenutni() << " ";
  lista.sljedeci(); std:: cout << lista.trenutni() << std::endl;

}

// test pocetak i kraj

void testf() {
  JednostrukaLista<int> lista;
  for(int i = 1; i <= 15; i++) lista.dodajIspred(i);

  lista.pocetak();

  std:: cout << lista.trenutni();
  lista.kraj();
  std:: cout << lista.trenutni();




}

// test brisanja trenutnog
void testg() {
  JednostrukaLista<int> lista;

  for(int i = 1; i <= 15; i++) lista.dodajIspred(i);

  lista.pocetak(); lista.obrisi();
  std:: cout << lista.trenutni();

  lista.kraj(); lista.obrisi(); lista.obrisi(); lista.obrisi();

  std:: cout << lista.trenutni();





}

// test operatora[]
void testh() {
  JednostrukaLista<int> lista;

  for(int i = 1; i <= 15; i++) lista.dodajIspred(i);

  lista.pocetak();
  std:: cout << lista.trenutni() << " = " << lista[0] << std::endl;


}

// test brisanje cijele liste

void testi() {
  JednostrukaLista<int> lista;
  for(int i = 1; i <= 15; i++) lista.dodajIspred(i);

  for(int i = 1; i <= 15; i++) lista.obrisi();
}
int main() {


    return 0;
}
