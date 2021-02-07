#include <iostream>
#include <stdexcept>
template<typename tip>
class Stek {
  private:
  struct Cvor {
      tip vrijednost;
      Cvor *sljedeci;
      Cvor (const tip &vrijednost, Cvor *sljedeci): vrijednost(vrijednost), sljedeci(sljedeci){}
      };
  Cvor *prvi;
  int velicina;
  bool isEmpty() const { return prvi == 0;}

  public:
  Stek(): prvi(0), velicina(0) {}
  Stek(const Stek &s){kopiraj(s);}
  ~Stek() {brisi();}
  Stek &operator = (const Stek &s);
  void brisi();
  void stavi(const tip &el) {
    prvi = new Cvor(el, prvi); velicina++;
  }
  tip skini();
  tip &vrh();
  int brojElemenata() {return velicina;}
  void prikazi() const;
  void kopiraj(const Stek &s);
};



template<typename tip>
Stek<tip> &Stek<tip>::operator = (const Stek &s) {
  if( &s == this) return *this;

  brisi();
  kopiraj(s);
  return *this;
}

template<typename tip>
void Stek<tip>::kopiraj(const Stek &s) {
  Cvor *q, *p(s.prvi);
  prvi = 0;
  velicina = s.velicina;
  while ( p != 0) {
    Cvor *n(new Cvor(p->vrijednost, 0));
    if( prvi == 0) prvi = n;
    else q->sljedeci = n;
    p = p->sljedeci;
    q = n;
  }
}

template<typename tip>
tip Stek<tip>::skini() {
  if( brojElemenata() == 0)
  throw std::domain_error("nesto");

  tip vr(prvi->vrijednost);
  Cvor *p = prvi->sljedeci;
  delete prvi;
  prvi = p;
  velicina--;
  return vr;
}

template <typename tip>
void Stek<tip>::brisi() {
  while( !isEmpty()) skini();
}

template <typename tip>
void Stek<tip>::prikazi() const {
  Cvor *p(prvi);
  std:: cout << "Sadrzaj: ";
  while( p != 0) {
    std:: cout << p->vrijednost << " ";
    p = p->sljedeci;
  }
}

template <typename tip>
tip& Stek<tip>::vrh() {
  if( isEmpty()) throw;
  return prvi->vrijednost;
}

//test kopirajuceg operatora dodjele
void testKopiraj() {
  Stek<int> stek;
  stek.stavi(1); stek.stavi(2); stek.stavi(3);

  Stek<int> novi;
  novi.kopiraj(stek);

  stek.prikazi(); std:: cout << std::endl;
  novi.prikazi(); std:: cout << std::endl;
}

//test referenca na vrh
void testVrh() {
  Stek<int> stek;
  stek.stavi(1); stek.stavi(2); stek.stavi(3);

  // na vrhu je 3

  stek.vrh() = 16;

  std:: cout << stek.vrh();
}

//test skini
void testSkini() {
  Stek<int> stek;
  for(int i = 0; i < 10; i++) stek.stavi(i);

  int a = stek.skini();

  std:: cout << "Skinut je broj: " << a << std::endl;

  std:: cout << "Sada je na vrhu: " << stek.vrh() << std::endl;

}

//test brisanja
void testBrisi() {
  Stek<int> stek;
  for(int i = 1; i <= 15; i++) stek.stavi(i);

  std:: cout << "Broj elemenata je: " << stek.brojElemenata() << std::endl;
  stek.brisi();
  std:: cout << "Broj elemenata je: " << stek.brojElemenata() << std::endl;

}

//test kopirajuci ctor
void testCopyCtor() {
  Stek<int> stek1;
  for(int i = 1; i <= 15; i++) stek1.stavi(i);

  Stek<int> stek2(stek1);

  stek1.prikazi(); std:: cout << std::endl;
  stek2.prikazi();

}

// test copy operator dodjele
void testCopyDodjele() {
  Stek<int> s1;
  for(int i = 1; i <= 15; i++) s1.stavi(i);

  Stek<int> s2;
  for(int i = 1; i <= 10; i++) s2.stavi(i);

  s2 = s1;

  s1.prikazi(); std:: cout << std::endl;
  s2.prikazi();



}

// kombinacija svih metoda

void testSve() {
  Stek<int> stek;

  for(int i = 1; i <= 130; i++) stek.stavi(i);

  std:: cout << "Na vrhu je: " << stek.vrh() << std::endl;
  stek.vrh() = 879;
  std:: cout << "Na vrhu je: " << stek.vrh() << std::endl;

  stek = stek;

  Stek<int> novi;
  for(int i = 1; i <= 8; i++) novi.stavi(i);

  stek.brisi();

  stek = novi;

  stek.prikazi();
}

int main() {


    return 0;
}
