// estructuras básicas

struct Nodo{
    int val;
    Nodo* siguiente;
};

struct Pila{

};

// árboles y grafos

struct NodoArbol{
    int valor;
    NodoArbol* izq;
    NodoArbol* der;
};

NodoArbol* Raiz = nullptr;
NodoArbol* Hoja = new NodoArbol;

