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
    cout << endl;

    // se inspeccionan las colas de arsenal (FIFO)
    cout << "  Arsenal (Cola): ";
    NodoBlast* tempB = op->municiones.frente;
    if (!tempB){
        cout << "[VACIO]";
    }
    while (tempB){
        cout << "{Tipo " << nombreBlast(tempB->tipoBlast) << "} -> ";
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

    return nuevo;
}

/* PERSONAJES
----------------------------------------------------------------------------------------
 tipo. personaje

    1. Juggernaut (El Tanque): HP Base: 150. Arma Base: Daño 15. Nace con 3 Escudos
        Físicos en su Pila.
    2. Ejecutor (El Asesino): HP Base: 100. Arma Base: Daño 30. Nace con 1 Escudo
        Anti-Plasma.
    3. Espectro (El Hacker): HP Base: 60. Arma Base: Daño 10. Nace sin escudos. Única
        clase capaz de encolar el ataque de Control Mental.
-----------------------------------------------------------------------------------------
*/

/* HEROES UNICOS POR SU ID_CLAVE
----------------------------------------------------------------------------------------
 tipo. personaje

    1. Borin "Corazón de Bronce" (Neón): Clase Tanque. ID_Clave = 10. Nace con 2
        Blasts de Racimo encolados.
    2. Dra. Emily Carter (Neón): Clase Hacker. ID_Clave = 20. Nace con 3 Blasts
        Troyanos.
    3. Gnashrak "El Asediador" (OMEGA): Clase Asesino. ID_Clave = 990. Nace con
        2 Blasts EMP y 1 Disrupción.
    4. El Alfa / Paciente Cero (OMEGA): Clase Tanque. ID_Clave = 880. Nace con 1
        Escudo Espejo y 3 Blasts de Racimo.
-----------------------------------------------------------------------------------------
*/

/* equipos 

(N)	La Resistencia Neón	- 1  

(O)	La Corporación OMEGA - 2

*/

/* ESTRUCTURA BASICAS DE ARBOL B-TREE4 */

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
// para referenciar más rápido

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
    delete hermano;
}

void eliminarDelNodo(NodoBTree4* nodo, int id) {
    int idx = 0;
    while (idx < nodo->cantidad_actual && nodo->ocupantes[idx]->ID_Clave < id){
        idx++;
    }

    // el ID está en ese nodo
    if (idx < nodo->cantidad_actual && nodo->ocupantes[idx]->ID_Clave == id){
        if (nodo->esHoja){
            // es una hoja, se elimina y desplaza
            delete nodo->ocupantes[idx]; 
            for (int i = idx + 1; i < nodo->cantidad_actual; i++)
                nodo->ocupantes[i - 1] = nodo->ocupantes[i];
            nodo->cantidad_actual--;
        } 
        else{
            // es un nodo interno, se busca un sustituto para no romper la estructura
            Operativo* predecesor = obtenerPredecesor(nodo->hijos[idx]);
            
            // se clonan los datos del predecesor al nodo actual (sin borrar el original aún)
            int idSustituto = predecesor->ID_Clave;
            
            // se reemplaza el operativo actual por el del predecesor
            nodo->ocupantes[idx] = crearOperativo(predecesor->Clase, predecesor->ID_Clave, predecesor->Bando);
            
            // se elimina el id duplicado en el subarbol
            eliminarDelNodo(nodo->hijos[idx], idSustituto);
        }
    } 
    else {
        // el ID no está, se baja
        if (nodo->esHoja) return; // no existe, hacer nada

        // el hijo necesita refuerzos?
        if (nodo->hijos[idx]->cantidad_actual == 1){
            // intentar pedir al hermano izquierdo
            if (idx != 0 && nodo->hijos[idx - 1]->cantidad_actual > 1){
                prestarDelNodoAnterior(nodo, idx);
            }
            // intentar pedir al hermano derecho
            else if (idx != nodo->cantidad_actual && nodo->hijos[idx + 1]->cantidad_actual > 1){
                prestarDelNodoSiguiente(nodo, idx);
            }
            // si ambos tienen el minimo, fusionar (o hacer merge)
            else{
                if (idx != nodo->cantidad_actual){
                    fusionarNodosArbol(nodo, idx);
                }
                else{
                    fusionarNodosArbol(nodo, idx - 1);
                    idx--; // se ajusta el indice tras la fusión
                }
            }
        }
        eliminarDelNodo(nodo->hijos[idx], id);
    }
}