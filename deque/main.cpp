#include <iostream>
template<typename tip>
class DvostraniRed {
  private:
  struct Cvor {
      tip element;
      Cvor *sljedeci;
      Cvor *prethodni;
      Cvor(const tip &element, Cvor *sljedeci, Cvor *prethodni): element(element), sljedeci(sljedeci), prethodni(prethodni) {}
  };
  Cvor *prvi;
  Cvor *zadnji;
  int velicina;
  public:
  DvostraniRed(): prvi(0), zadnji(0), velicina(0) {}
  ~DvostraniRed() {brisi();}
  DvostraniRed(const DvostraniRed<tip> &red);
  DvostraniRed &operator = (const DvostraniRed<tip> &red);
  int brojElemenata() const { return velicina;}
  void staviNaVrh(const tip &element) {

    if( velicina == 0) {
      prvi = zadnji = new Cvor(element, 0, 0);

    }
    else if(velicina == 1) {
      prvi = new Cvor(element, prvi, 0);
      zadnji->prethodni = prvi;
    }
    else {
      prvi = new Cvor(element, prvi, 0);
      Cvor *p = prvi->sljedeci;
      p->prethodni = prvi;

    }

    velicina++;

  }
  tip skiniSaVrha();
  void staviNaCelo(const tip &element);
  tip skiniSaCela();
  tip &vrh() {
     if(!brojElemenata())
    throw std::logic_error("Nema elemenata!");
    return prvi->element;
  };
  tip &celo() {
     if(!brojElemenata())
    throw std::logic_error("Nema elemenata!");
    return zadnji->element;
  };
  void ispisi() {
    Cvor *p(prvi);
    while( p != 0) {
      std:: cout << p->element << " ";
      p = p->sljedeci;
    }
    std:: cout << std::endl;
  }
  void brisi() {
    while( brojElemenata()) skiniSaCela();
  }
};

template<typename tip>
DvostraniRed<tip> &DvostraniRed<tip>::operator = (const DvostraniRed<tip> &red) {

  if( this == &red) return *this;

  brisi();
  prvi = zadnji = 0;
  velicina = 0;
  Cvor *p(red.prvi);

  while( p != 0) {
    staviNaVrh(p->element);
    p = p->sljedeci;
  }
  return *this;
}

template<typename tip>
DvostraniRed<tip>::DvostraniRed(const DvostraniRed<tip> &red) {
  prvi = zadnji = 0;
  velicina = 0;
  Cvor *p(red.prvi);
  while( p != 0){
    staviNaVrh(p->element);
    p = p->sljedeci;
  }
}

template<typename tip>
tip DvostraniRed<tip>::skiniSaVrha() {
  if( !brojElemenata())
    throw std::logic_error("Nema elemenata!");

  tip vrati = prvi->element;
  Cvor *temp = prvi->sljedeci;
  delete prvi;
  prvi = temp;
  velicina--;
  return vrati;

}


template<typename tip>
void DvostraniRed<tip>::staviNaCelo(const tip &element) {
  if( velicina == 0) {
    prvi = zadnji = new Cvor(element, 0, 0);
  }
  else if( velicina == 1) {
    zadnji = new Cvor(element, 0, prvi);
    prvi->sljedeci = zadnji;
  }
  else {
    zadnji = new Cvor(element, 0, zadnji);
    Cvor *p = zadnji->prethodni;
    p->sljedeci = zadnji;
  }
  velicina++;
}
template<typename tip>
tip DvostraniRed<tip>::skiniSaCela() {
  if(!brojElemenata())
    throw std::logic_error("Nema elemenata!");

  tip vrati = zadnji->element;
  if( velicina == 1) {

   delete prvi; prvi = 0;

  }
  else {
    Cvor *p = zadnji;
    zadnji = zadnji->prethodni;
    delete p; p = 0;
    zadnji->sljedeci = 0;
  }
  velicina--;
  return vrati;

}

void testStaviNaCelo() {
  DvostraniRed<int> dr;
  for(int i = 0; i < 10; i++) dr.staviNaCelo(i);

  std:: cout << dr.brojElemenata() << std::endl;

  std:: cout << dr.celo();
}

void testStaviNaVrh() {
  DvostraniRed<int> dr;
  for(int i = 0; i < 10; i++) dr.staviNaVrh(i);

  std:: cout << dr.brojElemenata() << std::endl;
  std:: cout << dr.celo();

}

void testSkiniSaCela() {
  DvostraniRed<int> dr;
  for(int i = 1; i <= 15; i++) {
    dr.staviNaCelo(i);
    dr.staviNaVrh(i);
  }

  std:: cout << dr.skiniSaCela();
}

void testSkiniSaVrh() {
  DvostraniRed<int> dr;

  for(int i = 1; i <= 10; i++)
  dr.staviNaCelo(i);

  std:: cout << dr.vrh() << " " << dr.skiniSaVrha();

}

void testCelo() {
  DvostraniRed<int> dr;

  dr.staviNaCelo(15);

  std:: cout << dr.celo() << " " << dr.vrh() << std::endl;

}

void testVrh() {
  DvostraniRed<int> dr;
  dr.staviNaVrh(15);

  std:: cout << dr.vrh() << " " << dr.skiniSaVrha() << std::endl;
  std:: cout << dr.brojElemenata();
}
int main() {

    testCelo();
    return 0;
}
