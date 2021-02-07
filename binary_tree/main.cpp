#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
using namespace std;
template<typename TypeKey, typename TypeValue>
class Mapa
{
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


template <typename TypeKey, typename TypeValue>
class Cvor
{
public:
    Cvor() {
        left = right = parent = 0;
    }
    Cvor(const TypeKey &key, const TypeValue &value, Cvor *left = 0, Cvor * right = 0, Cvor *parent = 0) {
        this->key = key;
        this->value = value;

        this->left = left;
        this->right = right;
        this->parent = parent;
    }
    Cvor(const Cvor<TypeKey, TypeValue> *pok) {
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
class BinStabloMapa : public Mapa<TypeKey, TypeValue>
{
private:
    Cvor<TypeKey, TypeValue> *root;
    int numberOfElements;
    void obrisi(Cvor<TypeKey,TypeValue> *cvor);
    void obidiCijeluMapu(Cvor<TypeKey, TypeValue> *pok);
    void ubaci(Cvor<TypeKey, TypeValue> *pok);
public:
    BinStabloMapa() {
        root = 0;
        numberOfElements = 0;
    }
    ~BinStabloMapa() {
        obrisi();
    }
    int brojElemenata() const override {
        return numberOfElements;
    }
    void obrisi() override;
    void obrisi(const TypeKey &value) override;
    const TypeValue operator [] (const TypeKey &key) const override;
    TypeValue &operator [] (const TypeKey &key) override;
    void obidi(Cvor<TypeKey, TypeValue> *pok);
    BinStabloMapa(const BinStabloMapa<TypeKey, TypeValue> &mapa);
    BinStabloMapa operator = (const BinStabloMapa<TypeKey, TypeValue> &mapa);
};

template<typename TypeKey, typename TypeValue>
void BinStabloMapa<TypeKey, TypeValue>::obrisi(Cvor<TypeKey,TypeValue> *cvor) {
    if( cvor) {
            obrisi(cvor->left);
            obrisi(cvor->right);
            delete cvor;
            cvor = 0;
        }
}

template<typename TypeKey, typename TypeValue>
void BinStabloMapa<TypeKey, TypeValue>::obidiCijeluMapu(Cvor<TypeKey, TypeValue> *pok) {
     if( pok != 0) {
           ubaci(pok);
           obidiCijeluMapu(pok->left);
           obidiCijeluMapu(pok->right);
     }
}

template<typename TypeKey, typename TypeValue>
void BinStabloMapa<TypeKey,TypeValue>::ubaci(Cvor<TypeKey, TypeValue> *pok) {
    if( pok != 0) {
        Cvor<TypeKey, TypeValue> *node = new Cvor<TypeKey,TypeValue>(pok->key, pok->value, 0, 0, 0);
        Cvor<TypeKey, TypeValue> *roditelj = 0;

    if( !numberOfElements) {
        root = node;
        root->parent = 0;
        ++numberOfElements;
        }
    else {
    Cvor<TypeKey, TypeValue> *trenutni = root;
    while(trenutni) {
        roditelj = trenutni;
        trenutni = node->key > trenutni->key ? trenutni->right : trenutni->left;
    }

    if( node->key < roditelj->key) {
        roditelj->left = node;
        numberOfElements++;
        node->parent = roditelj;

    } else {
        roditelj->right = node;
        numberOfElements++;
        node->parent = roditelj;
           }
        }
    }
}

template<typename TypeKey, typename TypeValue>
void BinStabloMapa<TypeKey, TypeValue>::obidi(Cvor<TypeKey, TypeValue> *pok) {
    if( pok) {
            std:: cout << pok->value;
            obidi(pok->left);
            obidi(pok->right);
        }
}

template<typename TypeKey, typename TypeValue>
BinStabloMapa<TypeKey,TypeValue>::BinStabloMapa(const BinStabloMapa<TypeKey, TypeValue> &mapa):root(0), numberOfElements(0) {
    Cvor<TypeKey, TypeValue> *pok = mapa.root;
    obidiCijeluMapu(pok);
}

template<typename TypeKey, typename TypeValue>
BinStabloMapa<TypeKey, TypeValue> BinStabloMapa<TypeKey,TypeValue>::operator =(const BinStabloMapa<TypeKey,TypeValue> &mapa) {
    if( this == &mapa)
            return *this;
        obrisi();
        root = 0;
        numberOfElements = 0;
        Cvor<TypeKey, TypeValue> *pok = mapa.root;
        obidiCijeluMapu(pok);
        return *this;
}

template<typename TypeKey, typename TypeValue>
const TypeValue BinStabloMapa<TypeKey, TypeValue>::operator[] (const TypeKey &key) const {
    Cvor<TypeKey, TypeValue> *find = root;
    Cvor<TypeKey, TypeValue> *node = new Cvor<TypeKey, TypeValue>(key, TypeValue(), 0, 0, 0);

    while( find) {
        if( find->key == key) {
            delete node;
            node = 0;
            return find->value;
        }
        find = node->key > find->key ? find->right : find->left;

    }

    delete node;
    node = 0;
    return TypeValue();
}

template<typename TypeKey, typename TypeValue>
TypeValue &BinStabloMapa<TypeKey, TypeValue>::operator[] (const TypeKey &key) {

    Cvor<TypeKey, TypeValue> *node = new Cvor<TypeKey, TypeValue>(key, TypeValue(), 0, 0, 0);
    Cvor<TypeKey, TypeValue> *roditelj = 0;

    if( !numberOfElements) {
        root = node;
        root->parent = 0;
        ++numberOfElements;
        return root->value;
    }
    Cvor<TypeKey, TypeValue> *find = root;
    while(find) {

        if( find->key == key) {
            delete node;
            node = 0;
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
        node->parent = roditelj;
        return node->value;
    } else {
        roditelj->right = node;
        numberOfElements++;
        node->parent = roditelj;
        return node->value;
    }

    if( node) {
        delete node;
        node = 0;
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
    delete p;
    p = 0;
    numberOfElements--;
}

template<typename TypeKey, typename TypeValue>
void BinStabloMapa<TypeKey, TypeValue>::obrisi() {
    obrisi(root);
    root = 0;
    numberOfElements = 0;
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

int main()
{
    NizMapa<int, int> nizMapa;
    BinStabloMapa<int, int> binStabloMapa;
    std::srand(std::time(nullptr));
    clock_t vrijeme1 = clock();
    for(int i = 0; i < 20000; i++) {
        int a = std::rand() % 200000000;
        nizMapa[a] = 15;
    }
    clock_t vrijeme2 = clock();
    int vrijemeIzvrsavanja = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC/1000);
    std::cout << "Vrijeme dodavanja 20000 elemenata u niz mapu: " << vrijemeIzvrsavanja << " ms." << std::endl;

    vrijeme2 = clock();

    for(int i = 0; i < 20000; i++) {
        int a = std::rand() % 2000000000;
        binStabloMapa[a] = 15;
    }

    vrijeme1 = clock();

    vrijemeIzvrsavanja = (vrijeme1 - vrijeme2) / (CLOCKS_PER_SEC/1000);

    std:: cout << "Vrijeme dodavanja 20000 elemenata u bin stablo mapu: " << vrijemeIzvrsavanja << " ms." << std::endl;

    //razlika za 20000 elemenata je 4195:67 što je oko 62 puta
    //obje mape pri dodavanju prvo petljom provjeravaju da li se taj ključ već nalazi u mapi
    //s tim da BinStablo u svakoj iteraciji polovi za 2 broj elemenata koje treba provjeriti, odbacuje uvijek jedno podstablo
    //što s porastom broja elemenata za niz mapu postaje sve gori problem

    // niz mapa takode mora konstantno alocirati dodatni prostor
    //sto ukljucuje prepisivanje trenutnih elemenata
    //brisanje stare mape
    // prepisivanje iz temp varijable, ovaj postupak mora uraditi čak 14 puta (log po bazi 2 od 20000)

    vrijeme1 = clock();

    nizMapa[17300] = 156;

    vrijeme2 = clock();

    std:: cout << "Dodavanje novog elementa u niz mapu: " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000000) << " mikros." << std::endl;

    vrijeme1 = clock();
    binStabloMapa[17300] = 156;
    vrijeme2 = clock();

    std:: cout << "Dodavanje novog elementa u bin stablo mapu: " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC/1000000) << " mikros." << std::endl;

    //sada razlika nije toliko izražena jer se dodaje samo jedan element, tako da se da zaključiti da niz mapa daje sve gore performanse kako
    //broj elemenata koji dodaje ili sadrži postaje sve veći

    vrijeme1 = clock();
    int broj = nizMapa[17300];
    vrijeme2 = clock();

    std:: cout << "Pristup trenutnog elementa " << broj << " u niz mapi: "  << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000000) << " mikros." << std::endl;
    vrijeme1 = clock();
    broj = binStabloMapa[17300];
    vrijeme2 = clock();
    std:: cout << "Pristup trenutnog elementa " << broj << " u bin stablo mapi: " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000000) << " mikros." << std::endl;

    // ista stvar kao u prethodnom testu, bin stablo je brža jer svakom provjera eliminiše veliki broj
    //elemenata, da smo pristupali većem broju elemenata, razlika bi bila veća


    //jos na kraju, potvrda da imaju priblizno isti broj elemenata

    std:: cout << nizMapa.brojElemenata() << " = " << binStabloMapa.brojElemenata() << std::endl;

    return 0;
}
