#include <iostream>
#include <stack>
#include <algorithm>
using namespace std;

struct Nodo {
    int dato;
    Nodo* izq;
    Nodo* der;
    Nodo(int d) : dato(d), izq(nullptr), der(nullptr) {}
};

class BST {
    Nodo* raiz;

    int altura(Nodo* n) {
        if (n == nullptr) return 0; //el arbol vacio tiene altura 0
        return 1 + max(altura(n->izq), altura(n->der));
    }

public:
    BST() : raiz(nullptr) {}

    ~BST() {
        stack<Nodo*> pila;
        if (raiz != nullptr) pila.push(raiz);
        while (!pila.empty()) {
            Nodo* n = pila.top();
            pila.pop();
            if (n->izq != nullptr) pila.push(n->izq);
            if (n->der != nullptr) pila.push(n->der);
            delete n;
        }
    }

    void insertar(int valor) {
        Nodo* padre = nullptr;
        Nodo* actual = raiz;
        while (actual != nullptr) {
            if (valor == actual->dato) return; //no se guardan repetidos
            padre = actual;
            if (valor < actual->dato) actual = actual->izq;
            else actual = actual->der;
        }
        Nodo* nuevo = new Nodo(valor);
        if (padre == nullptr) raiz = nuevo;
        else if (valor < padre->dato) padre->izq = nuevo;
        else padre->der = nuevo;
    }

    bool eliminar(int valor) {
        Nodo* padre = nullptr;
        Nodo* actual = raiz;
        while (actual != nullptr && actual->dato != valor) {
            padre = actual;
            if (valor < actual->dato) actual = actual->izq;
            else actual = actual->der;
        }
        if (actual == nullptr) return false; //el valor no esta en el arbol

        if (actual->izq != nullptr && actual->der != nullptr) {
            Nodo* sucesor = actual->der; //el menor del subarbol derecho
            padre = actual;
            while (sucesor->izq != nullptr) {
                padre = sucesor;
                sucesor = sucesor->izq;
            }
            actual->dato = sucesor->dato;
            actual = sucesor; //ahora se borra el sucesor, que tiene a lo mucho un hijo
        }

        Nodo* hijo = actual->izq;
        if (hijo == nullptr) hijo = actual->der;

        if (padre == nullptr) raiz = hijo;
        else if (padre->izq == actual) padre->izq = hijo;
        else padre->der = hijo;
        delete actual;
        return true;
    }

    void inOrder() {
        stack<Nodo*> pila;
        Nodo* actual = raiz;
        while (actual != nullptr || !pila.empty()) {
            while (actual != nullptr) {
                pila.push(actual);
                actual = actual->izq;
            }
            actual = pila.top();
            pila.pop();
            cout << actual->dato << " ";
            actual = actual->der;
        }
        cout << endl;
    }

    int altura() {
        return altura(raiz);
    }
};

int main() {
    int datos[20] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55, 65, 75, 90, 5, 15, 27, 62, 85};

    BST arbol;
    for (int i = 0; i < 20; i++) arbol.insertar(datos[i]);

    cout << "in-order con los 20 elementos: ";
    arbol.inOrder();
    cout << "altura: " << arbol.altura() << endl;

    arbol.eliminar(30); //tiene dos hijos
    arbol.eliminar(90); //tiene un solo hijo

    cout << "in-order despues de eliminar 30 y 90: ";
    arbol.inOrder();
    cout << "altura: " << arbol.altura() << endl;

    return 0;
}
