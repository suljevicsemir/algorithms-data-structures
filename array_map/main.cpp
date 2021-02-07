#include <iostream>
#include <string>
//#include <tuple>
#include <vector>
#include <new>

using namespace std;
template<typename TypeKey, typename TypeValue>
class Mapa {
    private:
    public:
    Mapa<TypeKey, TypeValue>() {}
    virtual ~Mapa() {}
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TypeKey &value) = 0;
    virtual const TypeValue operator [] (const TypeKey &key) const = 0;
    virtual TypeValue &operator [](const TypeKey &key) = 0;
};

template<typename TypeKey, typename TypeValue>
class NizMapa: public Mapa<TypeKey, TypeValue> {
  private:
  int velicina;
  int broj;
  std::pair<TypeKey, TypeValue> *nizParova;
  public:
  NizMapa<TypeKey, TypeValue>(): velicina(100), broj(0), nizParova(0){}
  ~NizMapa() {
      delete[] nizParova; nizParova = nullptr;
     broj = 0;
     velicina = 0;

  }

  NizMapa(const NizMapa<TypeKey, TypeValue> &mapa):velicina(mapa.velicina), broj(mapa.broj), nizParova(new std::pair<TypeKey, TypeValue>[velicina]()) {
    for(int i = 0; i < broj; i++)
        this->nizParova[i] = mapa.nizParova[i];
  }

  NizMapa operator = (const NizMapa<TypeKey, TypeValue> &mapa) {
    broj = mapa.broj;
    velicina = mapa.velicina;
    if( nizParova != 0)
      {
        delete[] nizParova; nizParova = nullptr;
      }

    nizParova = new std::pair<TypeKey, TypeValue>[velicina]();
    for(int i = 0; i < broj; i++)
    this->nizParova[i] = mapa.nizParova[i];

    return *this;

  }

  int brojElemenata() const override {
      return broj;
  }
  void obrisi() override {
      delete[] nizParova; nizParova = nullptr;
     broj = 0;
     velicina = 100;
     nizParova = new std::pair<TypeKey, TypeValue>[velicina]();
  };

  void obrisi(const TypeKey &valueOfKey) {
      for(int i = 0; i < broj; i++) {
          if( nizParova[i].first == valueOfKey) {
              for(int j = i; j < broj - 1; j++) {
                  nizParova[j] = nizParova[j + 1];
              }
          }
      }
       broj--;
  }
  void ispisi() const {
      for(int i = 0; i < broj; i++) {
          std:: cout << nizParova[i].first << " " << nizParova[i].second << std::endl;
      }
  }
  const TypeValue operator [] (const TypeKey &key) const override{
      for(int i = 0; i < velicina; i++) {
          if( nizParova[i].first == key)
            return nizParova[i].second;
      }
      return TypeValue();
  }
  TypeValue &operator [] (const TypeKey &key) override{
      if( !nizParova) {
          nizParova = new std::pair<TypeKey, TypeValue>[velicina]();
      }
        if( broj == velicina - 1) {
            std::pair<TypeKey, TypeValue> *temp = new std::pair<TypeKey, TypeValue>[velicina * 2]();
            for(int i = 0; i< broj; i++)
                temp[i] = nizParova[i];

            delete[] nizParova; nizParova = nullptr;
            nizParova = temp;
            velicina = velicina * 2;
        }
      for(int i = 0; i < broj; i++) {
          if( nizParova[i].first == key) {
              return nizParova[i].second;
          }
      }

      nizParova[broj].first = key;
      return nizParova[broj++].second;
  }

};


void test1() {
  NizMapa<int, std::string> mapa;

  //veliki unos
  for(int i = 1; i <= 1500; i++) {
    mapa[i] = "Ovo je broj " + i - '0';
  }

  //test brisanja

  mapa.obrisi();


  //test da li je mapa upotrebljiva

  mapa[4] = "Bok";


  std:: cout << "Mapa ima: " << mapa.brojElemenata() << std::endl;



}

class Ucenik {
  private:
  std::string jmbg;
  int visina;
  public:
  Ucenik(std::string jmbg, int visina) {
    this->jmbg = jmbg;
    this->visina = visina;
  }

  Ucenik() {
    jmbg = "";
    visina = 0;
  }

  std::string getJmbg() {
    return jmbg;
  }

  int getVisina() {
    return visina;
  }

};


//radi s korisnički definiranim tipovima
void test2() {
  NizMapa<int, Ucenik> mapa;
  mapa[0] = {"1234", 186};
  mapa[1] = {"1225", 156};

  std:: cout << mapa.brojElemenata() << std::endl;

  mapa.obrisi(1);

  std:: cout << mapa.brojElemenata();

}

void test3() {
  NizMapa<int, double> mapa;

    for(int i = 0; i <300; i++) {
      mapa[i] = i;
      mapa.obrisi();
    }

  mapa.obrisi();
  mapa.ispisi();

  // test upotrebljivosti
  NizMapa<int, double> novaMapa(mapa);

  novaMapa[0] = mapa[0];

  std:: cout << "Broj elemenata nove mape je: " << novaMapa.brojElemenata() << std::endl;
  std:: cout << "Broj elemenata stare mape je: " << mapa.brojElemenata() << std::endl;

}

void test4() {
  NizMapa<std::string, std::string> mapa;

  mapa["OR"] = "Dobar predmet";
  mapa["TP"] = "Dobar predmet";
  mapa["RPR"] = "Pravo dobar predmet";
  mapa["ASP"] = "Nije vala nešto";


  std:: cout << "Broj elemenata prije brisanja: " << mapa.brojElemenata() << std::endl;

  mapa.obrisi("Gdje si sta ima?");

  std:: cout << "Broj elemenata poslije brisanja: " << mapa.brojElemenata() << std::endl;

  mapa.obrisi("ASP");

  mapa.ispisi();

}

void test5() {
  NizMapa<int, int> mapa;

  for(int i = 1; i <= 15; i++)
    mapa[i] = i + 1;

  NizMapa<int, int> novaMapa;
  novaMapa = mapa;

  std:: cout << "Nova mapa:" << std::endl;
  novaMapa.ispisi();

  std:: cout << "Stara mapa:" << std::endl;
  mapa.ispisi();



}
int main() {

   NizMapa<std::string,std::string> m;
m["Sarajevo"] = "BiH";
m["Zagreb"] = "Hrvatska";
{
        // Konstruktor kopije
	NizMapa<std::string,std::string> m2(m);
        // Operator dodjele
	NizMapa<std::string,std::string> m3;
	m3["Kiseljak"] = "BiH";
	cout << (m3=m).brojElemenata() << " "; // 2
        // Destruktivna samododjela
	cout << (m=m).brojElemenata() << " "; // 2

        // Plitka kopija
	m.obrisi("Sarajevo");
	cout << m2.brojElemenata() << " "; // 2
        cout << m2["Sarajevo"] << " "; // BiH
        cout << m3.brojElemenata() << " "; // 2
        cout << m3["Sarajevo"] << " "; // BiH
}
cout << m.brojElemenata(); // 1
cout << " '" << m["Sarajevo"] << "' "; // ''
m.obrisi();
cout << m.brojElemenata(); // 0
cout << " '" << m["Zagreb"] << "'"; // ''
    return 0;
}
