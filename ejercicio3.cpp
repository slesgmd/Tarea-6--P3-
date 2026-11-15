#include <iostream>
#include <stack>
using namespace std;

struct Nodo {
    int dato;
    Nodo* izq;
    Nodo* der;
    Nodo(int d) : dato(d), izq(nullptr), der(nullptr) {}
};

bool caminoConSuma(Nodo* raiz, int k, stack<int>& camino) { //camino queda con la raiz en el tope
    stack<Nodo*> pila; //siempre guarda el camino de la raiz al nodo actual
    Nodo* actual = raiz;
    Nodo* ultimo = nullptr; //ultimo nodo que salio de la pila
    int suma = 0;

    while (actual != nullptr || !pila.empty()) {
        if (actual != nullptr) {
            pila.push(actual);
            suma += actual->dato;
            actual = actual->izq;
        } else {
            Nodo* tope = pila.top();
            if (tope->der != nullptr && tope->der != ultimo) {
                actual = tope->der; //todavia falta bajar por la derecha
            } else {
                if (tope->izq == nullptr && tope->der == nullptr && suma == k) {
                    while (!pila.empty()) {
                        camino.push(pila.top()->dato);
                        pila.pop();
                    }
                    return true;
                }
                suma -= tope->dato; //se retrocede un nivel
                pila.pop();
                ultimo = tope;
            }
        }
    }
    return false;
}

void probar(Nodo* raiz, int k) {
    stack<int> camino;
    cout << "k = " << k << ": ";
    if (caminoConSuma(raiz, k, camino)) {
        while (!camino.empty()) {
            cout << camino.top() << " ";
            camino.pop();
        }
        cout << endl;
    } else {
        cout << "no existe camino" << endl;
    }
}

void liberar(Nodo* raiz) {
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

int main() {
    Nodo* raiz = new Nodo(10);
    raiz->izq = new Nodo(6);
    raiz->der = new Nodo(15);
    raiz->izq->izq = new Nodo(3);
    raiz->izq->der = new Nodo(8);
    raiz->der->izq = new Nodo(12);
    raiz->der->der = new Nodo(20);
    raiz->izq->izq->izq = new Nodo(1);
    raiz->izq->der->der = new Nodo(9);
    raiz->der->der->der = new Nodo(25);

    probar(raiz, 33); //10 6 8 9
    probar(raiz, 37); //10 15 12
    probar(raiz, 70); //10 15 20 25
    probar(raiz, 16); //10 + 6 suma 16 pero 6 no es hoja
    probar(raiz, 100);

    liberar(raiz);
    return 0;
}
