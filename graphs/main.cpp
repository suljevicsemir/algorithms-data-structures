#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;
/*
unsigned int mojhash(int ulaz, unsigned int max) {
	unsigned int suma=23;
	suma = suma*31 + ulaz;
	return suma % max;
}

unsigned int glupihash(int ulaz,
unsigned int max) {
	return max-1;
}*/
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
class HashMapa: public Mapa<TypeKey,TypeValue> {
     private:
     std::pair<TypeKey, TypeValue> **nizParova;
     unsigned int (*zapamtiHash)(TypeKey key, unsigned int nesto);
     int kapacitet;
     int duzina;

     public:
     ~HashMapa();
     HashMapa();
     HashMapa(const HashMapa<TypeKey, TypeValue> &mapa);
     HashMapa operator = (const HashMapa<TypeKey, TypeValue> &mapa);
     void definisiHashFunkciju(unsigned int (*funkcija)(TypeKey key, unsigned int nesto)) {
         zapamtiHash = funkcija;
     }
     int brojElemenata() const override {return duzina;}
     void obrisi() override;
     void obrisi(const TypeKey &key) override;
     const TypeValue operator[] (const TypeKey &key) const override;
     TypeValue &operator [](const TypeKey &key) override;
     void ispisi() const;

};

//destruktor
template <typename TypeKey, typename TypeValue>
HashMapa<TypeKey, TypeValue>::~HashMapa() {
    for(int i = 0; i < kapacitet; i++) {
        if( nizParova[i] != 0) {
        delete nizParova[i];
        nizParova[i] = nullptr;
        }
    }
    delete[] nizParova; nizParova = nullptr;
}

// konstruktori
template <typename TypeKey, typename TypeValue>
HashMapa<TypeKey, TypeValue>::HashMapa(const HashMapa<TypeKey, TypeValue> &mapa) {

    nizParova = new std::pair<TypeKey, TypeValue>*[mapa.kapacitet]();
    kapacitet = mapa.kapacitet;
    duzina = mapa.duzina;
    zapamtiHash = mapa.zapamtiHash;
    for(int i = 0; i < kapacitet; i++) {
            if(mapa.nizParova[i] != nullptr) {
             nizParova[i] = new std::pair<TypeKey, TypeValue>();
             nizParova[i]->first = mapa.nizParova[i]->first;
             nizParova[i]->second = mapa.nizParova[i]->second;
             }
    }
}

template<typename TypeKey, typename TypeValue>
HashMapa<TypeKey, TypeValue>::HashMapa():nizParova(new std::pair<TypeKey, TypeValue>*[100]()), zapamtiHash(0), kapacitet(100), duzina(0){}

// kopirajuci operator dodjele
template <typename TypeKey, typename TypeValue>
HashMapa<TypeKey,TypeValue> HashMapa<TypeKey, TypeValue>::operator = (const HashMapa<TypeKey, TypeValue> &mapa) {

    if( &mapa == this)
        return *this;
    for(int i = 0; i < kapacitet; i++) {
        delete nizParova[i]; nizParova[i] = nullptr;
    }

    delete[] nizParova; nizParova = nullptr;
    duzina = 0;
    kapacitet = mapa.kapacitet;
    nizParova = new std::pair<TypeKey, TypeValue>*[kapacitet]();

    for(int i = 0; i < kapacitet; i++) {
        nizParova[i] = new std::pair<TypeKey, TypeValue>();
        if( mapa.nizParova[i] != nullptr) {
        nizParova[i]->first = mapa.nizParova[i]->first;
        nizParova[i]->second = mapa.nizParova[i]->second;
        }
    }
    duzina = mapa.duzina;
    return *this;
}

// preklopljeni operatori []
template <typename TypeKey, typename TypeValue>
TypeValue &HashMapa<TypeKey, TypeValue>::operator[] (const TypeKey &key) {
   if(  duzina == kapacitet - 1) {
        std::pair<TypeKey, TypeValue> **temp = new std::pair<TypeKey, TypeValue>*[kapacitet]();

        for(int i = 0; i < kapacitet; i++) {
            temp[i] = new std::pair<TypeKey, TypeValue>();
            if( nizParova[i] != nullptr) {
                temp[i]->first = nizParova[i]->first;
                temp[i]->second = nizParova[i]->second;
            }
        }

        for(int i = 0; i < kapacitet; i++) {
            delete nizParova[i]; nizParova[i] = nullptr;
        }

        delete[] nizParova; nizParova = nullptr;

        nizParova = new std::pair<TypeKey, TypeValue>*[kapacitet * 2]();

        for(int i = 0; i < kapacitet; i++) {
            nizParova[i] = new std::pair<TypeKey, TypeValue>();
            nizParova[i]->first = temp[i]->first;
            nizParova[i]->second = temp[i]->second;
        }



        for(int i = 0; i < kapacitet; i++) {
            delete temp[i]; temp[i] = nullptr;
        }

        delete[] temp; temp = nullptr;
        kapacitet = kapacitet * 2;

   }

   unsigned int izracunajHash = 0;

   if( zapamtiHash) {
       izracunajHash = zapamtiHash(key, kapacitet);
   }
   else throw("Error");


   if( nizParova[izracunajHash] != nullptr  && nizParova[izracunajHash]->first == key)
        return nizParova[izracunajHash]->second;

   while( nizParova[izracunajHash] != nullptr && nizParova[izracunajHash]->first != key) {
       izracunajHash = (izracunajHash + 1) % kapacitet;
   }

   if( nizParova[izracunajHash] != nullptr && nizParova[izracunajHash]->first == key) {

       return nizParova[izracunajHash]->second;

   }

   nizParova[izracunajHash] = new std::pair<TypeKey, TypeValue>();
   nizParova[izracunajHash]->first = key;
   duzina++;
   return nizParova[izracunajHash]->second;


}

template <typename TypeKey, typename TypeValue>
const TypeValue HashMapa<TypeKey, TypeValue>::operator [](const TypeKey &key) const {

    unsigned int izracunajHash = 0;

    if( zapamtiHash) {
        izracunajHash = zapamtiHash(key, kapacitet);
    }
    else throw;

    unsigned int zapamtiVrijednostHash = izracunajHash;
    bool nijeNaden = false;
    while( 1 ) {
        if( !nizParova[izracunajHash])
                izracunajHash = (izracunajHash + 1) % kapacitet;
        else if( nizParova[izracunajHash]->first != key)
                izracunajHash = (izracunajHash + 1) % kapacitet;
        else break;

        if( zapamtiVrijednostHash == izracunajHash) {
            nijeNaden = true;
            break;
        }
        }

        if( nijeNaden) {
            return TypeValue();
        }
        return nizParova[izracunajHash]->second;
}

//brisanje jednog kljuca
template <typename TypeKey, typename TypeValue>
void HashMapa<TypeKey, TypeValue>::obrisi(const TypeKey &key) {
    if( !duzina)
        throw;

    unsigned int izracunajHash = 0;
    if( zapamtiHash) {
        izracunajHash = zapamtiHash(key, kapacitet);
    }
    else throw;
    int zapamtiVrijednostHash = izracunajHash;
    while( 1 ) {
        if( nizParova[izracunajHash] == nullptr)
            izracunajHash = (izracunajHash + 1) % kapacitet;
        else if ( nizParova[izracunajHash]->first != key) {
            izracunajHash = (izracunajHash + 1) % kapacitet;
        }
        else break;

        if( izracunajHash == zapamtiVrijednostHash) {
            zapamtiVrijednostHash = -1;
            break;
        }
    }
    if( zapamtiVrijednostHash == -1) {

        throw;
    }
    else {

        delete nizParova[izracunajHash]; nizParova[izracunajHash] = nullptr;
        nizParova[izracunajHash] = new std::pair<TypeKey, TypeValue>();
        duzina--;
    }



}

//brisanje cijele mape
template <typename TypeKey, typename TypeValue>
void HashMapa<TypeKey, TypeValue>::obrisi() {
    for(int i = 0; i < kapacitet; i++) {
        delete nizParova[i]; nizParova[i] = nullptr;
    }
    delete[] nizParova; nizParova = nullptr;
    duzina = 0;
    kapacitet = 100;
    nizParova = new std::pair<TypeKey, TypeValue>*[kapacitet]();
}

template <typename TypeKey, typename TypeValue>
void HashMapa<TypeKey, TypeValue>::ispisi() const {
      std:: cout << std::endl;
      for(int i = 0; i < kapacitet; i++) {
          if( nizParova[i] != nullptr) {
              std:: cout << nizParova[i]->first << " " << nizParova[i]->second << std::endl;
          }
      }
}

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

template <typename TypeKey, typename TypeValue>
class Cvor {
    public:
    Cvor() { left = right = 0;}

    Cvor(const TypeKey &key, const TypeValue &value, Cvor *left = 0, Cvor * right = 0, Cvor *parent = 0) {
        this->key = key;
        this->value = value;

        this->left = left;
        this->right = right;
        this->parent = parent;
    }

    TypeKey key;
    TypeValue value;
    Cvor *left, *right, *parent;
};

template<typename TypeKey, typename TypeValue>
class BinStabloMapa : public Mapa<TypeKey, TypeValue> {
    private:
    Cvor<TypeKey, TypeValue> *root;
    int numberOfElements;
    void obrisi(Cvor<TypeKey,TypeValue> *cvor) {
        if( cvor) {
            obrisi(cvor->left);
            obrisi(cvor->right);
            delete cvor; cvor = 0;
        }
    };
    public:
    BinStabloMapa() {root = 0; numberOfElements = 0;}
    ~BinStabloMapa() {
        obrisi();
    }
    int brojElemenata() const override {return numberOfElements;}
    void obrisi() override;
    void obrisi(const TypeKey &value) override;
    const TypeValue operator [] (const TypeKey &key) const override;
    TypeValue &operator [] (const TypeKey &key) override;

    void ispisi() const {
        std:: cout << root->right->value;
    }

    void prodjiKrozMapuDesno() const {

         Cvor<TypeKey, TypeValue> *find = root;
         while(find) {
            std::cout << find->value << " ";
            find = find->right;

    }

    }

    BinStabloMapa(const BinStabloMapa<TypeKey, TypeValue> &mapa):root(0), numberOfElements(0)  {

    }

    void obidiCijeluMapu(Cvor<TypeKey, TypeValue> *pok) const {
        if( pok) {
            std:: cout << pok->value << " ";
            obidiCijeluMapu(pok->left);
            obidiCijeluMapu(pok->right);
        }
    }

    Cvor<TypeKey, TypeValue> *getRoot() {
        return root;
    }


};

template<typename TypeKey, typename TypeValue>
const TypeValue BinStabloMapa<TypeKey, TypeValue>::operator[] (const TypeKey &key) const {
    Cvor<TypeKey, TypeValue> *find = root;
    Cvor<TypeKey, TypeValue> *node = new Cvor<TypeKey, TypeValue>(key, TypeValue(), 0, 0, 0);

    while( find) {
        if( find->key == key) {
            delete node; node = 0;
            return find->value;
        }
        find = node->key > find->key ? find->right : find->left;

    }

    delete node; node = 0;
    return TypeValue();
}

template<typename TypeKey, typename TypeValue>
TypeValue &BinStabloMapa<TypeKey, TypeValue>::operator[] (const TypeKey &key) {

 Cvor<TypeKey, TypeValue> *node = new Cvor<TypeKey, TypeValue>(key, TypeValue(), 0, 0, 0);
 Cvor<TypeKey, TypeValue> *roditelj = 0;

 if( !numberOfElements) {
     root = node;
     ++numberOfElements;
     return root->value;
 }

 // treba vratiti vrijednost

 Cvor<TypeKey, TypeValue> *find = root;
 while(find) {

     if( find->key == key) {
     delete node; node = 0;
     return find->value;
     }
     find = node->key > find->key ? find->right : find->left;

 }


 Cvor<TypeKey, TypeValue> *trenutni = root;
 while(trenutni) {
     roditelj = trenutni;
     trenutni = node->key > trenutni->key ? trenutni->right : trenutni->left;
 }

 if( node->key < roditelj->key) {
     roditelj->left = node;
     numberOfElements++;
     return node->value;
 }
 else {
     roditelj->right = node;
     numberOfElements++;
     return node->value;
 }

  if( node) {
      delete node; node = 0;
  }

}

template<typename TypeKey, typename TypeValue>
void BinStabloMapa<TypeKey, TypeValue>::obrisi(const TypeKey &key) {
    Cvor<TypeKey, TypeValue> *p = root, *q = 0;
    Cvor<TypeKey, TypeValue> *temp = 0, *pp = 0, *rp = 0;

    while( p != 0 && key != p->key) {
        q = p;
        if( key < p->key)
            p = p->left;
        else
            p = p->right;
    }

    if( p == 0)
        return;

    if( p->left == 0)
        rp = p->right;
    else if( p->right == 0)
        rp = p->left;
    else {
        pp = p;
        rp = p->left;
        temp = rp->right;
        while( temp != 0) {
            pp = rp;
            rp = temp;
            temp = rp->right;
        }
        if( pp != p) {
            pp->right = rp->left;
            rp->left = p->left;
        }
        rp->right = p->right;
    }
    if( q == 0)
        root = rp;
    else if( p == q->left)
        q->left = rp;
    else
        q->right = rp;
    delete p; p = 0;
    numberOfElements--;
}

template<typename TypeKey, typename TypeValue>
void BinStabloMapa<TypeKey, TypeValue>::obrisi() {
    obrisi(root);
    root = 0;
    numberOfElements = 0;
}


int main() {

    /*NizMapa<int, int> nizMapa;
    BinStabloMapa<int, int> binStabloMapa;
    HashMapa<int, int> hashMapa;
    hashMapa.definisiHashFunkciju(mojhash);

    // unos velikog broja elemenata
    //posebno se mjeri vrijeme za unos svake mape

    clock_t vrijeme1 = clock();
    for(int i = 0; i < 20000; i++)
        nizMapa[i] = i + 33;

    clock_t vrijeme2 = clock();
    std::srand(std::time(nullptr));
    for(int i = 0; i < 20000 ;i++)
        binStabloMapa[std::rand() % 200000000] = i + 33;

    clock_t vrijeme3 = clock();
        for(int i = 0; i < 20000; i++)
        hashMapa[i] = i + 33;

    clock_t vrijeme4 = clock();
    int vrijemeIzvrsavanja = 0;
    vrijemeIzvrsavanja = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC/1000);

    std:: cout << "Vrijeme izvrsavanja za NIZ mapu je: " << vrijemeIzvrsavanja << " ms." <<  std::endl;

    vrijemeIzvrsavanja = (vrijeme3 - vrijeme2) /(CLOCKS_PER_SEC /1000);
    std:: cout << "Vrijeme izvrsavanja za BIN STABLO mapu je: " << vrijemeIzvrsavanja << " ms." << std::endl;



    vrijemeIzvrsavanja = (vrijeme4 - vrijeme3) / (CLOCKS_PER_SEC / 1000);
    std:: cout << "Vrijeme unosa elemenata u HASH mapu je: " << vrijemeIzvrsavanja << " ms." << std::endl;

    //razlike izmedu bin i niz mape su pisane na vjezbi 8, isto vazi i ovdje

    // ispisuje se broj elemenata bin mape, koji je isti kao i kod druge dvije oko (20000) +- 3 elementa
    // bin mapa je 7 puta brza od hash mape, a od niz mape oko 90 puta
    // razlika zasto je hash mapa brza od niz mape jeste da ni ona ne pretrazuje sve elemente pri svakom unosu
    //izracuna se hash kljuca i pristupa se direktno pokazivacu na tom mjestu
    //posto je hash mapa izvedena kao pok na pok, ako nema nista na tom mjestu, taj pok je nullptr i element se odma smjesta
    //a ako taj pok nije nullptr onda za sve kljuceve koji imaju isti hash pretrazujemo dok ne dodemo do nullptr
    //kada se dode do nullptr smjesti se neki kljuc koji ima isti hash kao i neki drugi kljucevi
    //takode pristup je brzi jer direktno moze pokupiti trazena vrijednost
    //zadnje sto se radi je dodavanje elementa
    //mana hash mape sto se tice dodavanja je da i ona mora vrsiti alokaciju novih elemenata 14-15 puta za ovaj broj elemenata


    std:: cout << "Broj elemenata u BIN STABLO mapi: " << binStabloMapa.brojElemenata() << std::endl;

    //ubacam na isto mjesto element u sve tri mape, radi boljeg poredenja

    nizMapa[18356] = 220;
    binStabloMapa[18356] = 220;
    hashMapa[18356] = 220;

    // testiranje pristupa random elementu, recimo 17 345
    vrijeme1 = clock();
    int a = nizMapa[18356];
    vrijeme2 = clock();

    std:: cout << "Vrijeme pristupa elementu " << a << " u NIZ MAPI: " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC/1000000) << " mikro sekundi" << std::endl;

    a = binStabloMapa[18356];
    vrijeme1 = clock();
    std:: cout << "Vrijeme pristupa elementu "<< a << " u BIN MAPI: " << (vrijeme1- vrijeme2) / (CLOCKS_PER_SEC / 1000000) << " mikro sekundi" << std::endl;
    a = hashMapa[18356];
    vrijeme2 = clock();

    std:: cout << "Vrijeme pristupa elementu " << a << " u HASH MAPI: " << ( vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000000) << " mikro sekundi" << std::endl;

    */

    return 0;
}
