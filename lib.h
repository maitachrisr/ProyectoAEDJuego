#include <ctime>   // para time()
#include <cstdlib> // para srand() y rand()
#include <iostream>
using namespace std;

/* ESTRUCTURAS BASICAS LINEALES */

struct NodoEscudo{
    int salud;
    NodoEscudo* siguiente;
};

struct PilaDefensa{
    NodoEscudo* tope = nullptr;
};

struct NodoBlast{
    int tipoBlast;
    NodoBlast* siguiente;
};

/* tipos de blasts
---------------------------------------
 tipo. objeto

    1. laser
    2. perforante (emp)
    3. troyano (control mental/nucleo)
    4. disrupcion
    5. racimo (AoE)
---------------------------------------
*/

// transformación de números a palabras
string nombreBlast(int tipo) {
    switch(tipo) {
        case 1: return "Laser";
        case 2: return "EMP (Perforante)";
        case 3: return "Troyano (Control Mental)";
        case 4: return "Disrupcion";
        case 5: return "Racimo (AoE)";
        default: return "Desconocido";
    }
}

struct ColaArsenal{
    NodoBlast* frente = nullptr;
    NodoBlast* final = nullptr;
};

/* ESTRUCTURA PRINCIPAL */

struct Operativo{
    int ID_Clave;
    int Bando;
    int HP_Base;
    int Clase; 
    PilaDefensa escudos;
    ColaArsenal municiones;
};

void liberarPilaDefensa(PilaDefensa& pila) {
    NodoEscudo* actual = pila.tope;
    while (actual != nullptr) {
        NodoEscudo* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    pila.tope = nullptr;
}

void liberarColaArsenal(ColaArsenal& cola) {
    NodoBlast* actual = cola.frente;
    while (actual != nullptr) {
        NodoBlast* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    cola.frente = nullptr;
    cola.final = nullptr;
}

int desencolarBlast(ColaArsenal& cola) {
    if (cola.frente == nullptr) return 0; // No hay munición, usa arma base

    NodoBlast* temp = cola.frente;
    int tipo = temp->tipoBlast;

    cola.frente = cola.frente->siguiente;
    if (cola.frente == nullptr) {
        cola.final = nullptr;
    }

    delete temp;
    return tipo;
}

void destruirOperativo(Operativo* op) {
    if (op == nullptr) return;
    liberarPilaDefensa(op->escudos);
    liberarColaArsenal(op->municiones);
    delete op;
}

/* FUNCIONES DE GESTIÓN */

void pushEscudo(PilaDefensa& pila, int vida){
    NodoEscudo* nuevo = new NodoEscudo;
    nuevo->salud = vida;
    nuevo->siguiente = pila.tope;
    pila.tope = nuevo;
}

void encolarBlast(ColaArsenal& cola, int tipo){
    NodoBlast* nuevo = new NodoBlast;
    nuevo->tipoBlast = tipo;
    nuevo->siguiente = nullptr;
    if (cola.frente == nullptr){
        cola.frente = nuevo;
    }
    else{
        cola.final->siguiente = nuevo;
    }
    cola.final = nuevo;
}

void inspeccionarSuministrosporID(Operativo* op) {
    if (op == nullptr) return;

    cout << "=== INVENTARIO OPERATIVO ID: " << op->ID_Clave << " ===" << endl;
    
    // se inspeccionan las pilas de escudos (LIFO)
    cout << "  Escudos (Pila): ";
    NodoEscudo* tempE = op->escudos.tope;
    if (!tempE){
        cout << "[VACIO]";
    }
    while (tempE){
        cout << "[" << tempE->salud << " HP] -> ";
        tempE = tempE->siguiente;
    }
    cout << "NULL" << endl;

    // se inspeccionan las colas de arsenal (FIFO)
    cout << "  Arsenal (Cola): ";
    NodoBlast* tempB = op->municiones.frente;
    if (!tempB){
        cout << "[VACIO]";
    }
    while (tempB){
        cout << "{" << nombreBlast(tempB->tipoBlast) << "} -> ";
        tempB = tempB->siguiente;
    }
    cout << "FINAL" << endl;
}

/* LÓGICA DE CREACIÓN Y SUMINISTROS */

Operativo* crearOperativo(int clase, int id, int bando){
    Operativo* nuevo = new Operativo;
    nuevo->escudos.tope = nullptr; 
    nuevo->municiones.frente = nullptr;
    nuevo->municiones.final = nullptr;
    // -------------------------------------

    nuevo->ID_Clave = id;
    nuevo->Bando = bando;
    nuevo->Clase = clase;

    // Verificar si es un héroe único por su ID_Clave
    if (id == 10) { // Borin "Corazón de Bronce"
        nuevo->Clase = 1; // Tanque
        nuevo->HP_Base = 150;
        for(int i = 0; i < 3; i++) pushEscudo(nuevo->escudos, 50); // 3 escudos físicos
        encolarBlast(nuevo->municiones, 5); // Racimo
        encolarBlast(nuevo->municiones, 5); // Racimo
    }
    else if (id == 20) { // Dra. Emily Carter
        nuevo->Clase = 3; // Hacker
        nuevo->HP_Base = 60;
        // Sin escudos
        encolarBlast(nuevo->municiones, 3); // Troyano
        encolarBlast(nuevo->municiones, 3); // Troyano
        encolarBlast(nuevo->municiones, 3); // Troyano
    }
    else if (id == 990) { // Gnashrak "El Asediador"
        nuevo->Clase = 2; // Asesino
        nuevo->HP_Base = 100;
        pushEscudo(nuevo->escudos, 100); // 1 escudo anti-plasma
        encolarBlast(nuevo->municiones, 2); // EMP
        encolarBlast(nuevo->municiones, 2); // EMP
        encolarBlast(nuevo->municiones, 4); // Disrupción
    }
    else if (id == 880) { // El Alfa / Paciente Cero
        nuevo->Clase = 1; // Tanque
        nuevo->HP_Base = 150;
        pushEscudo(nuevo->escudos, 100); // 1 Escudo Espejo (100 HP)
        encolarBlast(nuevo->municiones, 5); // Racimo
        encolarBlast(nuevo->municiones, 5); // Racimo
        encolarBlast(nuevo->municiones, 5); // Racimo
    }
    else {
        // valores predeterminados por clase
        if (clase == 1){ // juggernaut
            nuevo->HP_Base = 150;
            for(int i = 0; i < 3; i++) pushEscudo(nuevo->escudos, 50); // 3 escudos
        }
        else if (clase == 2){ // ejecutor
            nuevo->HP_Base = 100;
            pushEscudo(nuevo->escudos, 100); // 1 escudo
        }
        else{ // espectro
            nuevo->HP_Base = 60;
            // nace sin escudos
        }

        // suministros aleatorios
        int cantidadBlasts = rand() % 4 + 1; 
        
        for (int i = 0; i < cantidadBlasts; i++){
            // generar tipo de blast aleatorio de los 5 existentes
            int tipoAleatorio = rand() % 5 + 1; 
            
            // especial: solo los espectros encolan el control mental (tipo 3)
            if (tipoAleatorio == 3 && clase != 3){
                tipoAleatorio = 1; // si no es hacker, se da un laser
            }
            
            encolarBlast(nuevo->municiones, tipoAleatorio);
        }
    }

    return nuevo;
}

/* ESTRUCTURA PRINCIPAL DE ARBOL B-TREE4 */

struct NodoBTree4{
    Operativo* ocupantes[3]; 
    NodoBTree4* hijos[4];    
    
    // ocupantes actuales (rango válido de 0 a 3)
    int cantidad_actual;     
    // el nodo tiene hijos? (es vacio?)
    bool esHoja;             
};

struct ArbolB4{
    NodoBTree4* raiz = nullptr;
};

NodoBTree4* crearNodo(bool hoja){
    NodoBTree4* nuevoNodo = new NodoBTree4;
    nuevoNodo->cantidad_actual = 0;
    nuevoNodo->esHoja = hoja;
    
    // los ocupantes e hijos inicializan en null para evitar problemas de memoria
    for (int i = 0; i < 3; i++){
        nuevoNodo->ocupantes[i] = nullptr;
    }
    for (int i = 0; i < 4; i++){
        nuevoNodo->hijos[i] = nullptr;
    }
    
    return nuevoNodo;
}

/* MÁS LOGICA DE LIBERACIÓN: Destrucción completa del Árbol al cerrar el programa */
void liberarArbolBinario(NodoBTree4* nodo) {
    if (nodo == nullptr) return;

    // 1. Destruir recursivamente a todos los hijos primero
    if (!nodo->esHoja) {
        for (int i = 0; i <= nodo->cantidad_actual; i++) {
            liberarArbolBinario(nodo->hijos[i]);
        }
    }

    // 2. Destruir eficientemente a los ocupantes de este nodo
    for (int i = 0; i < nodo->cantidad_actual; i++) {
        if (nodo->ocupantes[i] != nullptr) {
            destruirOperativo(nodo->ocupantes[i]);
        }
    }

    // 3. Borrar el nodo contenedor
    delete nodo;
}

void mostrarArbol(NodoBTree4* nodo, int nivel){
    if (nodo != nullptr){
        // según el nivel
        for (int j = 0; j < nivel; j++) cout << "    "; 
        
        cout << "Nivel " << nivel << " [ ";
        for (int i = 0; i < nodo->cantidad_actual; i++) {
            cout << nodo->ocupantes[i]->ID_Clave;
            if (nodo->ocupantes[i]->Bando == 1) cout << "(N)"; else cout << "(O)";
            cout << " ";
        }
        cout << "]" << endl;

        // si !esHoja, se llama recursivamente para los hijos
        if (!nodo->esHoja){
            for (int i = 0; i <= nodo->cantidad_actual; i++){
                mostrarArbol(nodo->hijos[i], nivel + 1);
            }
        }
    }
}

void dividirHijoenArbol(NodoBTree4* padre, int i, NodoBTree4* lleno){
    NodoBTree4* nuevoHermano = crearNodo(lleno->esHoja);
    
    // elemento 2 va al nuevo hermano
    nuevoHermano->ocupantes[0] = lleno->ocupantes[2];
    nuevoHermano->cantidad_actual = 1;

    if (!lleno->esHoja){
        nuevoHermano->hijos[0] = lleno->hijos[2];
        nuevoHermano->hijos[1] = lleno->hijos[3];
        lleno->hijos[2] = nullptr;
        lleno->hijos[3] = nullptr;
    }

    Operativo* sube = lleno->ocupantes[1];

    lleno->ocupantes[1] = nullptr; 
    lleno->ocupantes[2] = nullptr;
    lleno->cantidad_actual = 1; 

    // desplazar hijos en el padre
    for (int j = padre->cantidad_actual; j >= i + 1; j--){
        padre->hijos[j + 1] = padre->hijos[j];
    }
    padre->hijos[i + 1] = nuevoHermano;

    // desplazar ocupantes en el padre
    for (int j = padre->cantidad_actual - 1; j >= i; j--){
        padre->ocupantes[j + 1] = padre->ocupantes[j];
    }

    padre->ocupantes[i] = sube; // se sube el que se guardó
    padre->cantidad_actual++;
}

void insertarenArbolNoLleno(NodoBTree4* nodo, Operativo* nuevoOp){
    int i = nodo->cantidad_actual - 1;

    if (nodo->esHoja){
        // si esHoja, se busca y desplaza ordenadamente
        while (i >= 0 && nodo->ocupantes[i]->ID_Clave > nuevoOp->ID_Clave){
            nodo->ocupantes[i + 1] = nodo->ocupantes[i];
            i--;
        }
        nodo->ocupantes[i + 1] = nuevoOp;
        nodo->cantidad_actual++;
    }
    else{
        // si !esHoja, se busca a qué hijo bajar
        while (i >= 0 && nuevoOp->ID_Clave < nodo->ocupantes[i]->ID_Clave){
                i--;
        }
        i++;

        if (nodo->hijos[i]->cantidad_actual == 3){
            dividirHijoenArbol(nodo, i, nodo->hijos[i]);
            
            // después del split, el elemento del medio subió
            // se revisa de nuevo si el nuevo ID es mayor al que subió
            if (nodo->ocupantes[i]->ID_Clave < nuevoOp->ID_Clave){
                i++;
            }
        }
        insertarenArbolNoLleno(nodo->hijos[i], nuevoOp);
    }
}

void insertarenArbol(ArbolB4 &arbol, Operativo* nuevoOp){
    if (arbol.raiz == nullptr){
        arbol.raiz = crearNodo(true);
        arbol.raiz->ocupantes[0] = nuevoOp;
        arbol.raiz->cantidad_actual = 1;
        return;
    }

    if (arbol.raiz->cantidad_actual == 3){
        NodoBTree4* nuevaRaiz = crearNodo(false);
        nuevaRaiz->hijos[0] = arbol.raiz;
        
        dividirHijoenArbol(nuevaRaiz, 0, arbol.raiz);
        
        int i = 0;
        if (nuevaRaiz->ocupantes[0]->ID_Clave < nuevoOp->ID_Clave){
            i++;
        }
        insertarenArbolNoLleno(nuevaRaiz->hijos[i], nuevoOp);
        
        arbol.raiz = nuevaRaiz;
    }
    else{
        insertarenArbolNoLleno(arbol.raiz, nuevoOp);
    }
}

bool buscarOperativo(NodoBTree4* nodo, int idBusqueda, int nivel){
    if (nodo == nullptr){
        cout << "ID " << idBusqueda << " no encontrado en el sistema." << endl;
        return false;
    }

    int i = 0;
    // se busca dentro del nodo actual
    while (i < nodo->cantidad_actual && idBusqueda > nodo->ocupantes[i]->ID_Clave){
        i++;
    }

    // se encontró
    if (i < nodo->cantidad_actual && nodo->ocupantes[i]->ID_Clave == idBusqueda){
        cout << "-> [ENCONTRADO] en Nivel " << nivel << ", posicion " << i << endl;
        return true;
    }

    // es una hoja Y no está
    if (nodo->esHoja){
        cout << "-> [FIN DE RUTA] ID no existe en Yggdrasil." << endl;
        return false;
    }

    // no está, pero hay hijos
    cout << "-> Nivel " << nivel << " (Bajando por hijo " << i << ") ";
    return buscarOperativo(nodo->hijos[i], idBusqueda, nivel + 1);
}

// se busca el operativo con el ID más alto en un subárbol (el más a la derecha)
Operativo* obtenerPredecesor(NodoBTree4* nodo) {
    NodoBTree4* actual = nodo;
    while (!actual->esHoja) {
        actual = actual->hijos[actual->cantidad_actual];
    }
    return actual->ocupantes[actual->cantidad_actual - 1];
}

Operativo* buscarYRetornarPersonaje(NodoBTree4* nodo, int id) {
    if (nodo == nullptr) return nullptr;
    int i = 0;
    while (i < nodo->cantidad_actual && id > nodo->ocupantes[i]->ID_Clave) i++;
    
    if (i < nodo->cantidad_actual && nodo->ocupantes[i]->ID_Clave == id) 
        return nodo->ocupantes[i];
    
    if (nodo->esHoja) return nullptr;
    return buscarYRetornarPersonaje(nodo->hijos[i], id);
}

// borrow del arbol
void prestarDelNodoAnterior(NodoBTree4* padre, int idx) {
    NodoBTree4* hijo = padre->hijos[idx];
    NodoBTree4* hermano = padre->hijos[idx - 1];

    // se desplaza ocupantes del hijo para hacer espacio
    for (int i = hijo->cantidad_actual - 1; i >= 0; i--)
        hijo->ocupantes[i + 1] = hijo->ocupantes[i];

    // el hijo recibe el operativo del padre
    hijo->ocupantes[0] = padre->ocupantes[idx - 1];

    if (!hijo->esHoja){
        for (int i = hijo->cantidad_actual; i >= 0; i--){
            hijo->hijos[i + 1] = hijo->hijos[i];
        }
         hijo->hijos[0] = hermano->hijos[hermano->cantidad_actual];
    }

    // el padre recibe el último operativo del hermano
    padre->ocupantes[idx - 1] = hermano->ocupantes[hermano->cantidad_actual - 1];

    hijo->cantidad_actual++;
    hermano->cantidad_actual--;
}

void prestarDelNodoSiguiente(NodoBTree4* padre, int idx) {
    NodoBTree4* hijo = padre->hijos[idx];
    NodoBTree4* hermano = padre->hijos[idx + 1];

    // el hijo vuelve a recibir el operativo del padre
    hijo->ocupantes[hijo->cantidad_actual] = padre->ocupantes[idx];

    // si !esHoja, entonces el hijo recibe el primer hijo del hermano
    if (!hijo->esHoja) {
        hijo->hijos[hijo->cantidad_actual + 1] = hermano->hijos[0];
    }

    // padre recibe el primer operativo del hermano
    padre->ocupantes[idx] = hermano->ocupantes[0];

    // se desplazan los elementos del hermano hacia la izquierda
    for (int i = 1; i < hermano->cantidad_actual; i++){
        hermano->ocupantes[i - 1] = hermano->ocupantes[i];
    }
    if (!hermano->esHoja){
        for (int i = 1; i <= hermano->cantidad_actual; i++){
            hermano->hijos[i - 1] = hermano->hijos[i];
        }
    }

    hijo->cantidad_actual++;
    hermano->cantidad_actual--;
}

// merge del arbol
void fusionarNodosArbol(NodoBTree4* padre, int idx) {
    NodoBTree4* hijo = padre->hijos[idx];
    NodoBTree4* hermano = padre->hijos[idx + 1];

    // se baja el operativo del padre al nodo hijo
    hijo->ocupantes[1] = padre->ocupantes[idx];

    // se pasan los operativos del hermano al hijo
    hijo->ocupantes[2] = hermano->ocupantes[0];

    if (!hijo->esHoja){
        hijo->hijos[2] = hermano->hijos[0];
        hijo->hijos[3] = hermano->hijos[1];
    }

    // se desplazan los ocupantes e hijos en el padre para llenar el hueco
    for (int i = idx + 1; i < padre->cantidad_actual; i++)
        padre->ocupantes[i - 1] = padre->ocupantes[i];
    for (int i = idx + 2; i <= padre->cantidad_actual; i++)
        padre->hijos[i - 1] = padre->hijos[i];

    hijo->cantidad_actual = 3; // nodo lleno
    padre->cantidad_actual--;
    delete hermano; // Aquí se elimina solo el nodo estructural sobrante
}

// VALIDACIÓN PROACTIVA: Asegura que el nodo hijo tenga suficientes ocupantes antes de descender
void asegurarHijoSuficiente(NodoBTree4* nodo, int idx) {
    if (nodo == nullptr || nodo->hijos[idx] == nullptr) return;

    // Si el hijo está en el mínimo absoluto de ocupantes (1 elemento)
    if (nodo->hijos[idx]->cantidad_actual == 1) {
        
        // intentar pedir al hermano izquierdo
        if (idx != 0 && nodo->hijos[idx - 1] != nullptr && nodo->hijos[idx - 1]->cantidad_actual > 1) {
            prestarDelNodoAnterior(nodo, idx);
        }
        // intentar pedir al hermano derecho
        else if (idx != nodo->cantidad_actual && nodo->hijos[idx + 1] != nullptr && nodo->hijos[idx + 1]->cantidad_actual > 1) {
            prestarDelNodoSiguiente(nodo, idx);
        }
        // si ambos tienen el minimo, fusionar (o hacer merge)
        else {
            if (idx != nodo->cantidad_actual) {
                fusionarNodosArbol(nodo, idx);
            } else {
                fusionarNodosArbol(nodo, idx - 1);
            }
        }
    }
}

// ALGORITMO DE ELIMINACIÓN CON REBALANCEO AUTOMÁTICO POR MUERTE
void eliminarDelNodo(NodoBTree4* nodo, int id) {
    if (nodo == nullptr) return;

    int idx = 0;
    while (idx < nodo->cantidad_actual && nodo->ocupantes[idx]->ID_Clave < id) {
        idx++;
    }

    // el ID está en ese nodo
    if (idx < nodo->cantidad_actual && nodo->ocupantes[idx]->ID_Clave == id) {
        if (nodo->esHoja) {
            // Se llama a destruirOperativo para vaciar sus Pilas y Colas internas
            destruirOperativo(nodo->ocupantes[idx]); 
            
            for (int i = idx + 1; i < nodo->cantidad_actual; i++) {
                nodo->ocupantes[i - 1] = nodo->ocupantes[i];
            }
            nodo->cantidad_actual--;
        } 
        else {
            // es un nodo interno, se busca un sustituto para no romper la estructura: Previene Underflow antes de extraerlo
            asegurarHijoSuficiente(nodo, idx);
            
            // se recalcula el índice por si el rebalanceo movió el operativo
            idx = 0;
            while (idx < nodo->cantidad_actual && nodo->ocupantes[idx]->ID_Clave < id) {
                idx++;
            }
            
            if (idx < nodo->cantidad_actual && nodo->ocupantes[idx]->ID_Clave == id) {
                Operativo* predecesor = obtenerPredecesor(nodo->hijos[idx]);
                int idSustituto = predecesor->ID_Clave;
                
                destruirOperativo(nodo->ocupantes[idx]);
                nodo->ocupantes[idx] = clonarOperativo(predecesor);
                
                // se elimina el id duplicado en el subarbol
                eliminarDelNodo(nodo->hijos[idx], idSustituto);
            } else {
                eliminarDelNodo(nodo->hijos[idx], id);
            }
        }
    } 
    else {
        // el ID no está, se baja
        if (nodo->esHoja) return; // no existe, hacer nada

        // el hijo necesita refuerzos?
        asegurarHijoSuficiente(nodo, idx);

        // se ajusta el indice tras la fusión si es necesario
        if (idx > nodo->cantidad_actual) {
            idx--;
        }
        
        eliminarDelNodo(nodo->hijos[idx], id);
    }
}

// FUNCIÓN DE LIMPIEZA AUTOMÁTICA (Se ejecuta cíclicamente tras el combate)
void limpiarOperativosMuertos(ArbolB4& arbol) {
    if (arbol.raiz == nullptr) return;

    while (true) {
        Operativo* muerto = buscarOperativoMuerto(arbol.raiz);
        if (muerto == nullptr) break; 

        cout << "\n\t[VALIDACION DE BAJA]: ID " << muerto->ID_Clave 
             << " llego a 0 HP. Activando Underflow por Muerte..." << endl;
             
        eliminarDelNodo(arbol.raiz, muerto->ID_Clave);
        
        if (arbol.raiz != nullptr && arbol.raiz->cantidad_actual == 0) {
            NodoBTree4* viejaRaiz = arbol.raiz;
            if (arbol.raiz->esHoja) {
                arbol.raiz = nullptr;
            } else {
                arbol.raiz = arbol.raiz->hijos[0]; 
            }
            delete viejaRaiz;
            cout << "\t[SISTEMA]: Contraccion de Raiz ejecutada de forma automatica." << endl;
        }
    }
}

// ==========================================
// NUEVAS FUNCIONES DE COMBATE Y SIMULACIÓN
// ==========================================

Operativo* clonarOperativo(Operativo* original) {
    if (original == nullptr) return nullptr;
    Operativo* clon = new Operativo;
    clon->ID_Clave = original->ID_Clave;
    clon->Bando = original->Bando;
    clon->HP_Base = original->HP_Base;
    clon->Clase = original->Clase;
    clon->escudos.tope = nullptr;
    clon->municiones.frente = nullptr;
    clon->municiones.final = nullptr;
    
    // Clonar escudos (LIFO)
    int tempEscudos[100];
    int countE = 0;
    NodoEscudo* currE = original->escudos.tope;
    while (currE != nullptr && countE < 100) {
        tempEscudos[countE++] = currE->salud;
        currE = currE->siguiente;
    }
    for (int i = countE - 1; i >= 0; i--) {
        pushEscudo(clon->escudos, tempEscudos[i]);
    }
    
    // Clonar municiones (FIFO)
    NodoBlast* currB = original->municiones.frente;
    while (currB != nullptr) {
        encolarBlast(clon->municiones, currB->tipoBlast);
        currB = currB->siguiente;
    }
    return clon;
}

void recibirDano(Operativo* op, int cantidad, int tipoBlast) {
    if (op == nullptr || cantidad <= 0) return;
    
    cout << "  -> ID " << op->ID_Clave << " (" << (op->Bando == 1 ? "Neon" : "OMEGA") << ") recibe impacto (" << nombreBlast(tipoBlast) << ")" << endl;
    
    if (tipoBlast == 2) { 
        if (op->escudos.tope != nullptr) {
            NodoEscudo* escudoBorrar = op->escudos.tope;
            op->escudos.tope = escudoBorrar->siguiente;
            delete escudoBorrar;
            
            int absorbido = 30;
            int restante = 20;
            cout << "    [PILA: ESCUDO ROTO] EMP dreno e inactivo el escudo superior. Absorbio " << absorbido << " HP." << endl;
            op->HP_Base -= restante;
            cout << "    [VIDA] HP restante de ID " << op->ID_Clave << ": " << op->HP_Base << endl;
        } else {
            op->HP_Base -= cantidad;
            if (op->HP_Base < 0){
                op->HP_Base = 0;
            }
            cout << "    [VIDA] Impacto directo sin escudos. HP restante: " << op->HP_Base << endl;
        }
        return;
    }
    
    int danoRemanente = cantidad;
    while (danoRemanente > 0 && op->escudos.tope != nullptr) {
        NodoEscudo* escudoActual = op->escudos.tope;
        
        if (escudoActual->salud > danoRemanente) {
            escudoActual->salud -= danoRemanente;
            cout << "    [PILA: ESCUDO] El escudo del tope absorbio " << danoRemanente << " de daño. Salud escudo actual: " << escudoActual->salud << " HP" << endl;
            danoRemanente = 0;
        } else {
            danoRemanente -= escudoActual->salud;
            cout << "    [PILA: DESAPILAR] Escudo con " << escudoActual->salud << " HP destruido por completo." << endl;
            
            op->escudos.tope = escudoActual->siguiente; // POP en la Pila
            delete escudoActual;
        }
    }
    
    // Si aún queda daño después de vaciar la pila de escudos, impacta la vida base
    if (danoRemanente > 0) {
        op->HP_Base -= danoRemanente;
        cout << "    [VIDA] El daño sobrepaso los escudos. ID " << op->ID_Clave << " recibe " << danoRemanente << " directo a su HP. HP restante: " << op->HP_Base << endl;
    }
}

Operativo* buscarEnemigoConMayorID(NodoBTree4* nodo, int bandoAtacante) {
    if (nodo == nullptr) return nullptr;
    Operativo* target = nullptr;
    for (int i = 0; i < nodo->cantidad_actual; i++) {
        if (nodo->ocupantes[i]->Bando != bandoAtacante && nodo->ocupantes[i]->HP_Base > 0) {
            if (target == nullptr || nodo->ocupantes[i]->ID_Clave > target->ID_Clave) {
                target = nodo->ocupantes[i];
            }
        }
    }
    return target;
}

Operativo* buscarEnemigoEnHijos(NodoBTree4* nodo, int bandoAtacante) {
    if (nodo == nullptr || nodo->esHoja) return nullptr;
    Operativo* target = nullptr;
    for (int i = 0; i <= nodo->cantidad_actual; i++) {
        NodoBTree4* hijo = nodo->hijos[i];
        if (hijo != nullptr) {
            Operativo* temp = buscarEnemigoConMayorID(hijo, bandoAtacante);
            if (temp != nullptr) {
                if (target == nullptr || temp->ID_Clave > target->ID_Clave) {
                    target = temp;
                }
            }
        }
    }
    return target;
}

void aplicarDanoRacimo(NodoBTree4* nodo, int bandoAtacante) {
    if (nodo == nullptr) return;
    cout << "    [RACIMO AoE] Aplicando 20 de daño de área..." << endl;
    // Daño en el nodo actual
    for (int i = 0; i < nodo->cantidad_actual; i++) {
        if (nodo->ocupantes[i]->Bando != bandoAtacante && nodo->ocupantes[i]->HP_Base > 0) {
            recibirDano(nodo->ocupantes[i], 20, 5);
        }
    }
    // Daño en los hijos directos
    if (!nodo->esHoja) {
        for (int i = 0; i <= nodo->cantidad_actual; i++) {
            NodoBTree4* hijo = nodo->hijos[i];
            if (hijo != nullptr) {
                for (int j = 0; j < hijo->cantidad_actual; j++) {
                    if (hijo->ocupantes[j]->Bando != bandoAtacante && hijo->ocupantes[j]->HP_Base > 0) {
                        recibirDano(hijo->ocupantes[j], 20, 5);
                    }
                }
            }
        }
    }
}

NodoBTree4* buscarNodoDeOperativo(NodoBTree4* nodo, int id) {
    if (nodo == nullptr) return nullptr;
    for (int i = 0; i < nodo->cantidad_actual; i++) {
        if (nodo->ocupantes[i]->ID_Clave == id) return nodo;
    }
    if (nodo->esHoja) return nullptr;
    for (int i = 0; i <= nodo->cantidad_actual; i++) {
        NodoBTree4* res = buscarNodoDeOperativo(nodo->hijos[i], id);
        if (res != nullptr) return res;
    }
    return nullptr;
}

struct RegistroAtaque {
    Operativo* atacante;
    Operativo* defensor;
    int tipoBlast;
};

void recolectarAtaques(NodoBTree4* nodo, NodoBTree4* raiz, RegistroAtaque* ataques, int& cantidadAtaques) {
    if (nodo == nullptr) return;
    
    for (int i = 0; i < nodo->cantidad_actual; i++) {
        Operativo* op = nodo->ocupantes[i];
        
        if (op->HP_Base > 0) {
            int tipoBlast = 1; // Por defecto: Arma Base (Laser)
            bool tieneMunicionEspecial = (op->municiones.frente != nullptr);
            
            if (tieneMunicionEspecial) {
                tipoBlast = op->municiones.frente->tipoBlast;
            }
            
            Operativo* target = buscarEnemigoConMayorID(nodo, op->Bando);
            
            if (target == nullptr && op->Clase == 3 && tipoBlast == 3) {
                target = buscarEnemigoEnHijos(nodo, op->Bando);
            }
            
            if (target != nullptr) {
                // EFECTO COLA (FIFO): Si usamos munición especial de la cola, la desencolamos de la memoria
                if (tieneMunicionEspecial) {
                    NodoBlast* temp = op->municiones.frente;
                    op->municiones.frente = temp->siguiente;
                    if (op->municiones.frente == nullptr) {
                        op->municiones.final = nullptr;
                    }
                    delete temp;
                }
                
                ataques[cantidadAtaques].atacante = op;
                ataques[cantidadAtaques].defensor = target;
                ataques[cantidadAtaques].tipoBlast = tipoBlast;
                cantidadAtaques++;
            }
        }
    }
    
    if (!nodo->esHoja) {
        for (int i = 0; i <= nodo->cantidad_actual; i++) {
            recolectarAtaques(nodo->hijos[i], raiz, ataques, cantidadAtaques);
        }
    }
}

Operativo* buscarOperativoMuerto(NodoBTree4* nodo) {
    if (nodo == nullptr) return nullptr;
    for (int i = 0; i < nodo->cantidad_actual; i++) {
        if (nodo->ocupantes[i]->HP_Base <= 0) {
            return nodo->ocupantes[i];
        }
    }
    if (!nodo->esHoja) {
        for (int i = 0; i <= nodo->cantidad_actual; i++) {
            Operativo* res = buscarOperativoMuerto(nodo->hijos[i]);
            if (res != nullptr) return res;
        }
    }
    return nullptr;
}

void limpiarOperativosMuertos(ArbolB4& arbol) {
    while (true) {
        Operativo* muerto = buscarOperativoMuerto(arbol.raiz);
        if (muerto == nullptr) break;
        cout << "  [MUERTE CONFIRMADA] ID " << muerto->ID_Clave << " (Bando " << (muerto->Bando == 1 ? "Neon" : "OMEGA") << ") ha caido en batalla. Extirpando registro..." << endl;
        eliminarDelNodo(arbol.raiz, muerto->ID_Clave);
        if (arbol.raiz != nullptr && arbol.raiz->cantidad_actual == 0) {
            NodoBTree4* viejaRaiz = arbol.raiz;
            if (arbol.raiz->esHoja) {
                arbol.raiz = nullptr;
            } else {
                arbol.raiz = arbol.raiz->hijos[0];
            }
            delete viejaRaiz;
        }
    }
}

void resolverFaseCombate(ArbolB4& arbol) {
    if (arbol.raiz == nullptr) return;
    
    cout << "\n=== INICIANDO FASE DE COMBATE ACTIVA ===" << endl;
    RegistroAtaque ataques[100];
    int cantidadAtaques = 0;
    
    // 1. Recolectar todos los ataques simultáneos
    recolectarAtaques(arbol.raiz, arbol.raiz, ataques, cantidadAtaques);
    
    if (cantidadAtaques == 0) {
        cout << "No hay conflictos ni municiones para disparar en este turno." << endl;
        return;
    }
    
    // 2. Ejecutar cada ataque registrado
    for (int i = 0; i < cantidadAtaques; i++) {
        Operativo* atacante = ataques[i].atacante;
        Operativo* defensor = ataques[i].defensor;
        int tipo = ataques[i].tipoBlast;
        
        // 1. El atacante debe estar vivo para ejecutar el ataque
        if (atacante->HP_Base <= 0) continue;
        
        // 2. ¡SOLUCIÓN CRÍTICA!: Verificar si el defensor sigue existiendo en el árbol
        // Si fue destruido o expulsado por un ataque previo en este mismo turno, lo ignoramos.
        if (buscarYRetornarPersonaje(arbol.raiz, defensor->ID_Clave) == nullptr) {
            cout << "* El ataque de ID " << atacante->ID_Clave << " fallo porque el objetivo ya no esta en su posicion original." << endl;
            continue; 
        }
        
        cout << "* ID " << atacante->ID_Clave << " dispara " << nombreBlast(tipo) << " contra ID " << defensor->ID_Clave << endl;
        
        if (tipo == 1) { // Láser Estándar (25)
            recibirDano(defensor, 25, 1);
        }
        else if (tipo == 2) { // EMP (50)
            recibirDano(defensor, 50, 2);
        }
        else if (tipo == 3) { // Troyano (Control Mental)
            defensor->Bando = atacante->Bando;
            cout << "    [HACKEO EXITOSO] ID " << defensor->ID_Clave << " ha sido hackeado. Nuevo bando: " << (defensor->Bando == 1 ? "Neon" : "OMEGA") << endl;
        }
        else if (tipo == 4) { // Disrupción (Expulsar y reinsertar)
            cout << "    [DISRUPCION] ID " << defensor->ID_Clave << " es expulsado del nodo y reinsertado desde la Raíz." << endl;
            Operativo* clon = clonarOperativo(defensor);
            eliminarDelNodo(arbol.raiz, defensor->ID_Clave);
            // Si la raíz quedó vacía
            if (arbol.raiz != nullptr && arbol.raiz->cantidad_actual == 0) {
                NodoBTree4* viejaRaiz = arbol.raiz;
                if (arbol.raiz->esHoja) {
                    arbol.raiz = nullptr;
                } else {
                    arbol.raiz = arbol.raiz->hijos[0];
                }
                delete viejaRaiz;
            }
            insertarenArbol(arbol, clon);
        }
        else if (tipo == 5) { // Racimo (AoE 20 en nodo e hijos directos)
            NodoBTree4* nodoAtacante = buscarNodoDeOperativo(arbol.raiz, atacante->ID_Clave);
            aplicarDanoRacimo(nodoAtacante, atacante->Bando);
        }
    }
    
    limpiarOperativosMuertos(arbol);
    cout << "=== FIN DE LA FASE DE COMBATE ===" << endl;
}

void ejecutarAtaque(Operativo* atacante, Operativo* defensor, ArbolB4& arbol) {
    if (atacante == nullptr || defensor == nullptr) return;
    if (atacante->HP_Base <= 0 || defensor->HP_Base <= 0) return;

    // Extraer el proyectil del frente de la cola (FIFO)
    int tipoProyectil = desencolarBlast(atacante->municiones);

    // Determinar el daño base del arma base segun clase
    int danoBase = 10; 
    if (atacante->Clase == 1) danoBase = 15;      // Juggernaut (El Tanque)
    else if (atacante->Clase == 2) danoBase = 30; // Ejecutor (El Asesino)
    else if (atacante->Clase == 3) danoBase = 10; // Espectro (El Hacker)

    cout << "\n   [*] ID " << atacante->ID_Clave << " ataca a ID " << defensor->ID_Clave << endl;

    // Evaluar impacto según el tipo de proyectil
    if (tipoProyectil == 0) {
        // estandar
        cout << "   -> Usa Ataque Base de su clase (Dano: " << danoBase << ")" << endl;
        recibirDano(defensor, danoBase, 1);
    } 
    else if (tipoProyectil == 1) {
        // laser
        recibirDano(defensor, 25, 1);
    } 
    else if (tipoProyectil == 2) { 
        cout << "   -> ¡DISPARO EMP PERFORANTE (50 Daño)!" << endl;
        
        if (defensor->escudos.tope != nullptr) {
            NodoEscudo* escudoTop = defensor->escudos.tope;
            
            if (escudoTop->salud <= 50) {
                int danoSobrante = 50 - escudoTop->salud;
                cout << "    [PILA: POP] EMP destruyo por completo el escudo superior (" << escudoTop->salud << " HP)." << endl;
                
                defensor->escudos.tope = escudoTop->siguiente;
                delete escudoTop;

                if (danoSobrante > 0) {
                    defensor->HP_Base -= danoSobrante;
                    cout << "    [VIDA] El dano remanente dreno " << danoSobrante << " HP de la vida base. Vida actual: " << defensor->HP_Base << endl;
                }
            } else {
                escudoTop->salud -= 50;
                cout << "    [PILA: ABSORBIDO] El escudo del tope absorbio los 50 de dano. Salud restante del escudo: " << escudoTop->salud << " HP." << endl;
                cout << "    [VIDA] La vida base permanece intacta." << endl;
            }
        } else {
            // Si no tiene escudos en absoluto, recibe los 50 de daño neto en la vida
            defensor->HP_Base -= 50;
            cout << "    [VIDA] Impacto EMP directo sin escudos. HP restante: " << defensor->HP_Base << endl;
        }
    }
    else if (tipoProyectil == 3) {
        // troyano
        if (atacante->Clase == 3) {
            defensor->Bando = atacante->Bando;
            cout << "   -> [TROYANO INTERCEPTADO] ¡ID " << defensor->ID_Clave 
                 << " fue hackeado con éxito! Ahora lucha para el Bando " 
                 << (defensor->Bando == 1 ? "Neon (1)" : "OMEGA (2)") << endl;
        } else {
            cout << "   -> Intento usar Troyano pero no es un Hacker. Se reduce a disparo estándar." << endl;
            recibirDano(defensor, danoBase, 1);
        }
    } 
    else if (tipoProyectil == 4) {
        cout << "   -> [DISRUPCION ELECTRÓNICA] ID " << defensor->ID_Clave << " es expulsado del nodo cuántico..." << endl;
        
        Operativo* clon = clonarOperativo(defensor);
        
        defensor->escudos.tope = nullptr;
        defensor->municiones.frente = nullptr;
        defensor->municiones.final = nullptr;
        
        eliminarDelNodo(arbol.raiz, defensor->ID_Clave);
        
        insertarenArbol(arbol, clon);
        cout << "    [ARBOL] ID " << clon->ID_Clave << " ha sido reinsertado en una nueva posicion estacional." << endl;
    } 
    else if (tipoProyectil == 5) {
        cout << "   -> [BLAST DE RACIMO] Impacto explosivo de area." << endl;
        recibirDano(defensor, 20, 5);
    }
}
