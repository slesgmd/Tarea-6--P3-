#include <iostream>
#include <queue>
using namespace std;

struct Nodo {
    int dato;
    Nodo* izq;
    Nodo* der;
    Nodo(int d) : dato(d), izq(nullptr), der(nullptr) {}
};

bool esCompleto(Nodo* raiz) {
    queue<Nodo*> cola;
    cola.push(raiz);
    bool huboHueco = false;
    while (!cola.empty()) {
        Nodo* actual = cola.front();
        cola.pop();
        if (actual == nullptr) {
            huboHueco = true;
        } else {
            if (huboHueco) return false; //hay un nodo despues de un espacio vacio
            cola.push(actual->izq);
            cola.push(actual->der);
        }
    }
    return true;
}

int anchoMaximo(Nodo* raiz) {
    if (raiz == nullptr) return 0;
    queue<Nodo*> cola;
    cola.push(raiz);
    int maximo = 0;
    while (!cola.empty()) {
        int enNivel = cola.size(); //en este momento la cola tiene un nivel entero
        if (enNivel > maximo) maximo = enNivel;
        for (int i = 0; i < enNivel; i++) {
            Nodo* actual = cola.front();
            cola.pop();
            if (actual->izq != nullptr) cola.push(actual->izq);
            if (actual->der != nullptr) cola.push(actual->der);
        }
    }
    return maximo;
}

int profundidadMinima(Nodo* raiz) { //la raiz esta en la profundidad 1
    if (raiz == nullptr) return 0;
    queue<Nodo*> cola;
    cola.push(raiz);
    int profundidad = 1;
    while (!cola.empty()) {
        int enNivel = cola.size();
        for (int i = 0; i < enNivel; i++) {
            Nodo* actual = cola.front();
            cola.pop();
            if (actual->izq == nullptr && actual->der == nullptr) return profundidad; //primera hoja que aparece
            if (actual->izq != nullptr) cola.push(actual->izq);
            if (actual->der != nullptr) cola.push(actual->der);
        }
        profundidad++;
    }
    return profundidad;
}

void mostrar(Nodo* raiz) {
    cout << "es completo: " << (esCompleto(raiz) ? "si" : "no") << endl;
    cout << "ancho maximo: " << anchoMaximo(raiz) << endl;
    cout << "profundidad minima: " << profundidadMinima(raiz) << endl;
}

void liberar(Nodo* raiz) {
    queue<Nodo*> cola;
    if (raiz != nullptr) cola.push(raiz);
    while (!cola.empty()) {
        Nodo* n = cola.front();
        cola.pop();
        if (n->izq != nullptr) cola.push(n->izq);
        if (n->der != nullptr) cola.push(n->der);
        delete n;
    }
}

int main() {
    Nodo* a = new Nodo(1); //arbol completo de 6 nodos
    a->izq = new Nodo(2);
    a->der = new Nodo(3);
    a->izq->izq = new Nodo(4);
    a->izq->der = new Nodo(5);
    a->der->izq = new Nodo(6);

    Nodo* b = new Nodo(10); //el 15 es hoja pero hay nodos mas abajo
    b->izq = new Nodo(6);
    b->der = new Nodo(15);
    b->izq->izq = new Nodo(3);
    b->izq->der = new Nodo(8);
    b->izq->izq->izq = new Nodo(1);
    b->izq->izq->der = new Nodo(4);
    b->izq->der->izq = new Nodo(7);
    b->izq->der->der = new Nodo(9);

    cout << "arbol a" << endl;
    mostrar(a);
    cout << "arbol b" << endl;
    mostrar(b);

    liberar(a);
    liberar(b);
    return 0;
}
