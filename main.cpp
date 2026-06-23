/* PROYECTO DE JUEGO - ALGORITMOS Y ESTRUCTURAS DE DATOS
    REALIZADO POR CHRISTOPHER MAITA - 31_761_223 */

#include <iostream>
#include "lib.h"

using namespace std;


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

void menu() {
    cout << "\n========================================" << endl;
    cout << "     SISTEMA DE MANDO YGGDRASIL  " << endl;
    cout << "========================================" << endl;
    cout << "1. Registrar / Insertar Operativo" << endl;
    cout << "2. Mostrar Estructura del Arbol" << endl;
    cout << "3. Buscar Operativo con Ruta de Acceso" << endl;
    cout << "4. Inspeccionar Suministros por ID" << endl;
    cout << "5. Dar de baja / Extirpar Operativo" << endl;
    cout << "6. Ejecutar Banco de Pruebas Automatico" << endl;
    cout << "7. Ejecutar Simulacion Automática (10 Turnos)" << endl;
    cout << "0. Salir" << endl;
    cout << "========================================" << endl;
    cout << "Seleccione una opcion: ";
}

Operativo* generarOperativoAleatorio(int bando) {
    int clase = rand() % 3 + 1;
    int id = 0;
    if (clase == 1) { // Tanque
        id = rand() % 200 + 801;
        while (id == 880 || id == 990) id = rand() % 200 + 801;
    } else if (clase == 2) { // Asesino
        id = rand() % 300 + 501;
    } else { // Hacker
        id = rand() % 500 + 1;
        while (id == 10 || id == 20) id = rand() % 500 + 1;
    }
    return crearOperativo(clase, id, bando);
}

void ejecutarSimulacion(ArbolB4& arbol) {
    cout << "\n=============================================" << endl;
    cout << "     SIMULADOR AUTOMATICO YGGDRASIL  " << endl;
    cout << "     (MODO AUTOMATICO: 10 TURNOS CON DADO)" << endl;
    cout << "=============================================" << endl;
    
    cin.ignore(10000, '\n'); // Limpiar buffer de entrada de cin anteriores
    int turnosTotales = 10;
    int totalInyectados = 0;
    
    for (int t = 1; t <= turnosTotales; t++) {
        int bando = (t % 2 == 1) ? 1 : 2; // Turno impar = Neon (1), par = OMEGA (2)
        string nombreBando = (bando == 1) ? "Neon (N)" : "OMEGA (O)";
        
        cout << "\n=============================================" << endl;
        cout << "TURNO " << t << ": Equipo " << nombreBando << endl;
        cout << "=============================================" << endl;
        
        int dado = rand() % 3 + 1;
        cout << "[PASO 1] Tirada de dado de inyeccion: " << dado << endl;
        
        for (int i = 0; i < dado; i++) {
            if (totalInyectados >= 60) {
                cout << "[LIMITE DE PARTIDA] Se ha alcanzado el maximo de 60 inyecciones globales." << endl;
                break;
            }
            
            Operativo* nuevoOp = generarOperativoAleatorio(bando);
            
            if (nuevoOp != nullptr) {
                cout << "\n[PASO 2 y 3] Intentando inyectar Operativo ID " << nuevoOp->ID_Clave 
                     << " (Clase: " << nuevoOp->Clase << ", Bando: " << (nuevoOp->Bando == 1 ? "Neon" : "OMEGA") << ")" << endl;
                
                // Buscar duplicados
                if (buscarYRetornarPersonaje(arbol.raiz, nuevoOp->ID_Clave) != nullptr) {
                    cout << "  -> [ERROR] El ID " << nuevoOp->ID_Clave << " ya existe en la estructura. Inyeccion fallida." << endl;
                    destruirOperativo(nuevoOp);
                } else {
                    insertarenArbol(arbol, nuevoOp);
                    cout << "  -> [EXITO] Operativo inyectado." << endl;
                    totalInyectados++;
                }
            }
        }
        
        cout << "\n[ESTADO DEL ARBOL TRAS INYECCIONES]:" << endl;
        mostrarArbol(arbol.raiz, 0);
        
        // [PASO 4] Combate y Limpieza
        resolverFaseCombate(arbol);
        
        // VALIDACIÓN EXTRA EN EL MAIN: Si tras todo el combate el árbol quedó completamente vacío
        if (arbol.raiz != nullptr && arbol.raiz->cantidad_actual == 0) {
            NodoBTree4* viejaRaiz = arbol.raiz;
            if (arbol.raiz->esHoja) {
                arbol.raiz = nullptr;
            } else {
                arbol.raiz = arbol.raiz->hijos[0];
            }
            delete viejaRaiz;
            cout << "\n\t[SISTEMA - ALERTA CRITICA]: Toda la red Yggdrasil ha colapsado por bajas masivas." << endl;
        }
        
        if (t < turnosTotales) {
            cout << "\nPresione ENTER para avanzar al Turno " << (t + 1) << "...";
            cin.get();
        }
    }
    cout << "\n=============================================" << endl;
    cout << "     SIMULACION COMPLETADA. TOTAL INYECTADOS: " << totalInyectados << endl;
    cout << "=============================================" << endl;
}

int main() {
    srand(time(0)); // para evitar lanzadas de dados consistentes
    ArbolB4 yggdrasil;
    int opcion, clase, id, bando;

    do {
        menu();
        cin >> opcion;

        switch(opcion) {
            case 1:
                cout << "\n--- REGISTRAR NUEVO OPERATIVO ---" << endl;
                cout << "Ingrese ID Clave (Entero Unico): ";
                cin >> id;
                if (buscarYRetornarPersonaje(yggdrasil.raiz, id) != nullptr) {
                    cout << "[ERROR] El ID ya existe en el Nucleo." << endl;
                    break;
                }
                cout << "Seleccione Clase (1: Juggernaut | 2: Ejecutor | 3: Espectro): ";
                cin >> clase;
                if(clase < 1 || clase > 3) { cout << "Clase invalida." << endl; break; }
                cout << "Seleccione Bando (1: Neon (N) | 2: OMEGA (O)): ";
                cin >> bando;
                if(bando < 1 || bando > 2) { cout << "Bando invalido." << endl; break; }

                insertarenArbol(yggdrasil, crearOperativo(clase, id, bando));
                cout << "[EXITO] Operativo inyectado en la estructura." << endl;
                break;

            case 2:
                cout << "\n--- AUDITORIA VISUAL DE ARBOL B-4 ---" << endl;
                if (yggdrasil.raiz == nullptr) {
                    cout << "El arbol esta vacio." << endl;
                } else {
                    mostrarArbol(yggdrasil.raiz, 0);
                }
                break;

            case 3:
                cout << "\n--- INTELIGENCIA DE BUSQUEDA ---" << endl;
                cout << "Ingrese ID a rastrear: ";
                cin >> id;
                buscarOperativo(yggdrasil.raiz, id, 0);
                break;

            case 4:
                cout << "\n--- AUDITORIA DE SUMINISTROS ---" << endl;
                cout << "Ingrese ID del Operativo: ";
                cin >> id;
                {
                    Operativo* op = buscarYRetornarPersonaje(yggdrasil.raiz, id);
                    if (op != nullptr) {
                        inspeccionarSuministrosporID(op);
                    } else {
                        cout << "[ERROR] Operativo no encontrado." << endl;
                    }
                }
                break;

            case 5:
                cout << "\n--- EXTIRPACION MANUAL (ELIMINAR) ---" << endl;
                cout << "Ingrese ID del operativo a dar de baja: ";
                cin >> id;
                if (buscarYRetornarPersonaje(yggdrasil.raiz, id) == nullptr) {
                    cout << "[ERROR] El ID no existe en el sistema." << endl;
                } else {
                    // 1. Invoca tu algoritmo de eliminación con rebalanceo automático
                    eliminarDelNodo(yggdrasil.raiz, id);
                    
                    // 2. Invoca tu ciclo de limpieza por si la baja desencadena otra muerte encadenada
                    limpiarOperativosMuertos(yggdrasil);
                    
                    // 3. Verificación de contracción de raíz (Validación de Gabriel)
                    if (yggdrasil.raiz != nullptr && yggdrasil.raiz->cantidad_actual == 0) {
                        NodoBTree4* viejaRaiz = yggdrasil.raiz;
                        if (yggdrasil.raiz->esHoja) {
                            yggdrasil.raiz = nullptr;
                        } else {
                            yggdrasil.raiz = yggdrasil.raiz->hijos[0];
                        }
                        delete viejaRaiz; // Liberación estructural limpia
                        cout << "\t[SISTEMA]: Contraccion de Raiz ejecutada tras extirpacion manual." << endl;
                    }
                    cout << "[EXITO] Registro borrado y memoria interna liberada de forma eficiente." << endl;
                }
                break;

            case 6:
                cout << "\n--- EJECUTANDO BANCO DE PRUEBAS AUTOMATICO ---" << endl;
                insertarenArbol(yggdrasil, crearOperativo(1, 10, 1)); 
                insertarenArbol(yggdrasil, crearOperativo(3, 20, 1)); 
                insertarenArbol(yggdrasil, crearOperativo(2, 30, 2)); 
                cout << "1. Insercion ordenada completada de IDs: 10, 20, 30." << endl;

                insertarenArbol(yggdrasil, crearOperativo(1, 25, 2));
                cout << "2. Insercion de ID 25 forzo Split. Estado actual:" << endl;
                mostrarArbol(yggdrasil.raiz, 0);

                cout << "3. Buscando Ruta de ID 25:" << endl;
                buscarOperativo(yggdrasil.raiz, 25, 0);

                insertarenArbol(yggdrasil, crearOperativo(2, 5, 1));
                insertarenArbol(yggdrasil, crearOperativo(1, 15, 2));
                cout << "4. Preparando reestructuraciones adicionales..." << endl;

                eliminarDelNodo(yggdrasil.raiz, 10);
                cout << "5. Extirpacion del ID 10 completada con rebalanceo estructural." << endl;

                cout << "6. Verificacion profunda de suministros dinamicos (ID 20):" << endl;
                {
                    Operativo* carter = buscarYRetornarPersonaje(yggdrasil.raiz, 20);
                    if (carter != nullptr) inspeccionarSuministrosporID(carter);
                }
                break;

            case 7:
                ejecutarSimulacion(yggdrasil);
                break;

            case 0:
                cout << "\nCerrando Operacion Yggdrasil. Destruyendo estructuras..." << endl;
                // EXTREMADAMENTE IMPORTANTE: Liberación de TODA la memoria dinámica restante antes de finalizar el proceso
                liberarArbolBinario(yggdrasil.raiz);
                yggdrasil.raiz = nullptr;
                cout << "[MEMORIA HEAP LIMPIA] No quedan fugas. Adios." << endl;
                break;

            default:
                cout << "Opcion incorrecta." << endl;
        }
    } while (opcion != 0);

    return 0;
}
