#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

enum Color { ROJO, NEGRO };

struct Nodo {
    int dato;
    Color color;
    Nodo* izq;
    Nodo* der;
    Nodo* padre;
    Nodo(int d) : dato(d), color(ROJO), izq(nullptr), der(nullptr), padre(nullptr) {}
};

class BST {
protected:
    Nodo* raiz;

    Nodo* insertarNodo(int valor) { //misma insercion del ejercicio 1, devuelve el nodo creado
        Nodo* padre = nullptr;
        Nodo* actual = raiz;
        while (actual != nullptr) {
            if (valor == actual->dato) return nullptr;
            padre = actual;
            if (valor < actual->dato) actual = actual->izq;
            else actual = actual->der;
        }
        Nodo* nuevo = new Nodo(valor);
        nuevo->padre = padre;
        if (padre == nullptr) raiz = nuevo;
        else if (valor < padre->dato) padre->izq = nuevo;
        else padre->der = nuevo;
        return nuevo;
    }

    int altura(Nodo* n) {
        if (n == nullptr) return 0;
        return 1 + max(altura(n->izq), altura(n->der));
    }

    void preOrden(Nodo* n) {
        if (n == nullptr) return;
        cout << n->dato << " ";
        preOrden(n->izq);
        preOrden(n->der);
    }

    void liberar(Nodo* n) {
        if (n == nullptr) return;
        liberar(n->izq);
        liberar(n->der);
        delete n;
    }

public:
    BST() : raiz(nullptr) {}

    ~BST() {
        liberar(raiz);
    }

    void insertar(int valor) {
        insertarNodo(valor);
    }

    int altura() {
        return altura(raiz);
    }

    void preOrden() {
        preOrden(raiz);
        cout << endl;
    }
};

class RedBlackTree : public BST {
    bool esRojo(Nodo* n) {
        return n != nullptr && n->color == ROJO; //la hoja nula cuenta como negra
    }

    void rotarIzquierda(Nodo* x) {
        Nodo* y = x->der;
        x->der = y->izq;
        if (y->izq != nullptr) y->izq->padre = x;
        y->padre = x->padre;
        if (x->padre == nullptr) raiz = y;
        else if (x == x->padre->izq) x->padre->izq = y;
        else x->padre->der = y;
        y->izq = x;
        x->padre = y;
    }

    void rotarDerecha(Nodo* x) {
        Nodo* y = x->izq;
        x->izq = y->der;
        if (y->der != nullptr) y->der->padre = x;
        y->padre = x->padre;
        if (x->padre == nullptr) raiz = y;
        else if (x == x->padre->izq) x->padre->izq = y;
        else x->padre->der = y;
        y->der = x;
        x->padre = y;
    }

    void recolorear(Nodo* padre, Nodo* tio, Nodo* abuelo) {
        padre->color = NEGRO;
        tio->color = NEGRO;
        abuelo->color = ROJO;
    }

    void corregir(Nodo* z) {
        while (esRojo(z->padre)) { //si el padre es rojo no es la raiz, entonces hay abuelo
            Nodo* padre = z->padre;
            Nodo* abuelo = padre->padre;
            if (padre == abuelo->izq) {
                Nodo* tio = abuelo->der;
                if (esRojo(tio)) {
                    recolorear(padre, tio, abuelo);
                    z = abuelo; //el problema puede haber subido al abuelo
                } else {
                    if (z == padre->der) { //forma de triangulo, se pasa a linea
                        z = padre;
                        rotarIzquierda(z);
                        padre = z->padre;
                    }
                    padre->color = NEGRO;
                    abuelo->color = ROJO;
                    rotarDerecha(abuelo);
                }
            } else {
                Nodo* tio = abuelo->izq;
                if (esRojo(tio)) {
                    recolorear(padre, tio, abuelo);
                    z = abuelo;
                } else {
                    if (z == padre->izq) {
                        z = padre;
                        rotarDerecha(z);
                        padre = z->padre;
                    }
                    padre->color = NEGRO;
                    abuelo->color = ROJO;
                    rotarIzquierda(abuelo);
                }
            }
        }
        raiz->color = NEGRO;
    }

    bool sinRojosSeguidos(Nodo* n) {
        if (n == nullptr) return true;
        if (esRojo(n) && (esRojo(n->izq) || esRojo(n->der))) return false;
        return sinRojosSeguidos(n->izq) && sinRojosSeguidos(n->der);
    }

    int alturaNegra(Nodo* n) { //devuelve -1 si algun camino tiene distinta cantidad de negros
        if (n == nullptr) return 1;
        int izq = alturaNegra(n->izq);
        int der = alturaNegra(n->der);
        if (izq == -1 || izq != der) return -1;
        if (esRojo(n)) return izq;
        return izq + 1;
    }

    void preOrdenColor(Nodo* n) {
        if (n == nullptr) return;
        cout << n->dato << (esRojo(n) ? "(R) " : "(N) ");
        preOrdenColor(n->izq);
        preOrdenColor(n->der);
    }

public:
    void insertar(int valor) {
        Nodo* nuevo = insertarNodo(valor); //el nodo nuevo entra como rojo
        if (nuevo != nullptr) corregir(nuevo);
    }

    bool raizNegra() { //propiedad 2
        return !esRojo(raiz);
    }

    bool sinRojosSeguidos() { //propiedad 3
        return sinRojosSeguidos(raiz);
    }

    bool mismaAlturaNegra() { //propiedad 4
        return alturaNegra(raiz) != -1;
    }

    bool verificar() { //la 1 y la 5 no se revisan porque el color es un enum y el nulo se toma como negro
        return raizNegra() && sinRojosSeguidos() && mismaAlturaNegra();
    }

    void preOrden() {
        preOrdenColor(raiz);
        cout << endl;
    }
};

int main() {
    int n = 20;
    BST bst;
    RedBlackTree rbt;
    for (int i = 1; i <= n; i++) {
        bst.insertar(i);
        rbt.insertar(i);
    }

    cout << "pre-orden bst: ";
    bst.preOrden();
    cout << "pre-orden red-black: ";
    rbt.preOrden();

    cout << "altura bst: " << bst.altura() << endl;
    cout << "altura red-black: " << rbt.altura() << endl;
    cout << "cota 2*log2(n+1): " << 2 * log2(n + 1) << endl;

    cout << "raiz negra: " << (rbt.raizNegra() ? "si" : "no") << endl;
    cout << "sin rojos seguidos: " << (rbt.sinRojosSeguidos() ? "si" : "no") << endl;
    cout << "misma cantidad de negros en cada camino: " << (rbt.mismaAlturaNegra() ? "si" : "no") << endl;
    cout << "es red-black valido: " << (rbt.verificar() ? "si" : "no") << endl;

    return 0;
}
