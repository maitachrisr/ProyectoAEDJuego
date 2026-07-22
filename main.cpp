/* PROYECTO DE JUEGO - ALGORITMOS Y ESTRUCTURAS DE DATOS
    REALIZADA POR:
    + CHRISTOPHER SYDUEL MAITA TOVAR - 31_761_223 
    + GABRIEL JESÚS KHAJIKIAN RODRÍGUEZ - 32_461_173
    + DANIELA LILIANA PALMA MATA - 30_459_494    
*/

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
    cout << "7. Ejecutar Simulacion Manual (10 Turnos)" << endl;
    cout << "0. Salir" << endl;
    cout << "========================================" << endl;
    cout << "Seleccione una opcion: ";
}

Operativo* crearOperativo(int clase, int id, int bando);

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

void ejecutarSimulacionManualConDado(ArbolB4& arbol) {
    if (arbol.raiz != nullptr) {
        cout << "\n[SISTEMA]: Reiniciando red cuántica... Liberando estructuras previas." << endl;
        liberarArbolBinario(arbol.raiz);
        arbol.raiz = nullptr;
    }
    
    cout << "\n=============================================" << endl;
    cout << "     SIMULADOR INTERACTIVO YGGDRASIL         " << endl;
    cout << "     (MODO MANUAL POR TURNOS CON DADO)       " << endl;
    cout << "=============================================" << endl;
    
    int turnosTotales = 10;
    int totalInyectados = 0;
    int bandoControlRaizActual = 0;
    int turnosConsecutivosRaiz = 0;
    bool victoriaPorDominioRaiz = false;
    string faccionDominanteRaiz = "";
    
    for (int t = 1; t <= turnosTotales; t++) {
        int bando = (t % 2 == 1) ? 1 : 2;
        string nombreBando = (bando == 1) ? "Neón (N)" : "OMEGA (O)";
        
        cout << "\n=============================================" << endl;
        cout << "TURNO " << t << ": Equipo " << nombreBando << endl;
        cout << "=============================================" << endl;
        
        // --- AQUÍ SE APLICA EL DADO ---
        int dado = rand() % 3 + 1;
        cout << "[PASO 1] Tirada de dado de inyección: " << dado << endl;
        cout << "  -> El equipo " << nombreBando << " debe inyectar " << dado << " operativo(s) este turno." << endl;
        
        // --- PASO 2: INYECCIÓN MANUAL SEGÚN EL RESULTADO DEL DADO ---
        for (int i = 0; i < dado; i++) {
            if (totalInyectados >= 60) {
                cout << "[LÍMITE DE PARTIDA] Se alcanzó el máximo de 60 inyecciones globales." << endl;
                break;
            }
            
            int idOp, claseOp;
            cout << "\n  --- Inyección " << (i + 1) << " de " << dado << " (Manual) ---" << endl;
            cout << "  Ingrese ID del Operativo: ";
            cin >> idOp;
            
            if (buscarYRetornarPersonaje(arbol.raiz, idOp) != nullptr) {
                cout << "  -> [ERROR] El ID " << idOp << " ya existe en la red. Inyección fallida (se pierde el intento)." << endl;
            } else {
                cout << "  Seleccione Clase (1: Juggernaut/Tanque | 2: Ejecutor/Asesino | 3: Espectro/Hacker): ";
                cin >> claseOp;
                if (claseOp < 1 || claseOp > 3) claseOp = 1; // Valor por defecto
                
                Operativo* nuevoOp = crearOperativo(claseOp, idOp, bando);
                insertarenArbol(arbol, nuevoOp);
                cout << "  -> [ÉXITO] Operativo ID " << idOp << " inyectado en el árbol." << endl;
                totalInyectados++;
            }
        }
        
        cout << "\n[ESTADO DEL ÁRBOL TRAS INYECCIONES]:" << endl;
        mostrarArbol(arbol.raiz, 0);
        
        // --- PASO 3: FASE DE COMBATE AUTOMÁTICA DEL TURNO ---
        cout << "\nPresione ENTER para ejecutar la Fase de Combate...";
        cin.ignore(10000, '\n');
        cin.get();
        
        resolverFaseCombate(arbol);
        
        // Colapso de Raíz por bajas masivas
        if (arbol.raiz != nullptr && arbol.raiz->cantidad_actual == 0) {
            NodoBTree4* viejaRaiz = arbol.raiz;
            if (arbol.raiz->esHoja) {
                arbol.raiz = nullptr;
            } else {
                arbol.raiz = arbol.raiz->hijos[0];
            }
            delete viejaRaiz;
            cout << "\n\t[ALERTA CRÍTICA]: Toda la red Yggdrasil ha colapsado por bajas masivas." << endl;
        }
        
        // --- CONDICIÓN 1: ANIQUILACIÓN ---
        int neonVivos = 0, omegaVivos = 0;
        contarVivosEnTurno(arbol.raiz, neonVivos, omegaVivos);
        if (t > 1 && (neonVivos == 0 || omegaVivos == 0)) {
            cout << "\n=============================================" << endl;
            if (neonVivos > 0) cout << "Aniquilación total por La Resistencia Neón" << endl;
            else cout << "Aniquilación total por La Corporación OMEGA" << endl;
            cout << "=============================================" << endl;
            break; 
        }

        // --- CONDICIÓN 2: DOMINIO DE LA RAÍZ ---
        if (arbol.raiz != nullptr && arbol.raiz->cantidad_actual > 0) {
            int bandoRaizPrimerElemento = arbol.raiz->ocupantes[0]->Bando;
            bool raizExclusiva = true;
            for (int k = 1; k < arbol.raiz->cantidad_actual; k++) {
                if (arbol.raiz->ocupantes[k]->Bando != bandoRaizPrimerElemento) {
                    raizExclusiva = false;
                    break;
                }
            }
            if (raizExclusiva) {
                if (bandoControlRaizActual == bandoRaizPrimerElemento) {
                    turnosConsecutivosRaiz++;
                } else {
                    bandoControlRaizActual = bandoRaizPrimerElemento;
                    turnosConsecutivosRaiz = 1;
                }
            } else {
                bandoControlRaizActual = 0;
                turnosConsecutivosRaiz = 0;
            }
            if (turnosConsecutivosRaiz >= 3) {
                victoriaPorDominioRaiz = true;
                faccionDominanteRaiz = (bandoControlRaizActual == 1) ? "La Resistencia Neón" : "La Corporación OMEGA";
                cout << "\n=============================================" << endl;
                cout << "¡" << faccionDominanteRaiz << " ha hackeado Yggdrasil desde la Raíz!" << endl;
                cout << "=============================================" << endl;
                break;
            }
        } else {
            bandoControlRaizActual = 0;
            turnosConsecutivosRaiz = 0;
        }
        
        if (t < turnosTotales) {
            cout << "\nPresione ENTER para avanzar al Turno " << (t + 1) << "...";
            cin.get();
        }
    }

    if (victoriaPorDominioRaiz) {
        liberarArbolBinario(arbol.raiz);
        arbol.raiz = nullptr;
        return;
    }

    // RECORRIDO Y CONTEO FINAL
    cout << "\n=============================================" << endl;
    cout << "     SIMULACIÓN COMPLETADA" << endl;
    cout << "=============================================" << endl;

    int vivosNeon = 0, vivosOmega = 0;
    int totalVidaNeon = 0, totalVidaOmega = 0;

    cout << "\n>>> INICIANDO RECORRIDO CUÁNTICO FINAL <<<" << endl;
    if (arbol.raiz == nullptr) {
        cout << "No quedó ningún operativo en pie. La red colapsó." << endl;
    } else {
        realizarConteoFinalInOrder(arbol.raiz, vivosNeon, vivosOmega, totalVidaNeon, totalVidaOmega);
    }

    cout << "\n=============================================" << endl;
    cout << "          REPORTE DE DAÑOS FINALES" << endl;
    cout << "=============================================" << endl;
    cout << "  RESISTENCIA NEÓN:  " << vivosNeon << " vivos. HP Total: " << totalVidaNeon << endl;
    cout << "  CORPORACIÓN OMEGA: " << vivosOmega << " vivos. HP Total: " << totalVidaOmega << endl;
    cout << "=============================================" << endl;

    if (vivosNeon > vivosOmega) {
        cout << "¡VICTORIA PARA LA RESISTENCIA NEÓN!" << endl;
    } else if (vivosOmega > vivosNeon) {
        cout << "¡VICTORIA PARA LA CORPORACIÓN OMEGA!" << endl;
    } else {
        cout << "EMPATE EN OPERATIVOS. Evaluando ID de la Raíz..." << endl;
        int idMaxNeonRaiz = -1, idMaxOmegaRaiz = -1;
        if (arbol.raiz != nullptr) {
            for (int i = 0; i < arbol.raiz->cantidad_actual; i++) {
                if (arbol.raiz->ocupantes[i] != nullptr && arbol.raiz->ocupantes[i]->HP_Base > 0) {
                    if (arbol.raiz->ocupantes[i]->Bando == 1 && arbol.raiz->ocupantes[i]->ID_Clave > idMaxNeonRaiz)
                        idMaxNeonRaiz = arbol.raiz->ocupantes[i]->ID_Clave;
                    else if (arbol.raiz->ocupantes[i]->Bando == 2 && arbol.raiz->ocupantes[i]->ID_Clave > idMaxOmegaRaiz)
                        idMaxOmegaRaiz = arbol.raiz->ocupantes[i]->ID_Clave;
                }
            }
        }
        if (idMaxNeonRaiz > idMaxOmegaRaiz) cout << "¡VICTORIA PARA LA RESISTENCIA NEÓN por ID en Raíz (" << idMaxNeonRaiz << ")!" << endl;
        else if (idMaxOmegaRaiz > idMaxNeonRaiz) cout << "¡VICTORIA PARA LA CORPORACIÓN OMEGA por ID en Raíz (" << idMaxOmegaRaiz << ")!" << endl;
        else cout << "EMPATE ABSOLUTO." << endl;
    }
    liberarArbolBinario(arbol.raiz);
    arbol.raiz = nullptr; // se libera la memoria para evitar acumulaciones innecesarias
    cout << "=============================================" << endl;
}

void poblarArbolConCatálogos(ArbolB4& arbol) {
    // 1. Inyectar los personajes normales de personajes.txt al Bando 1 (Neón)
    for (int i = 0; i < totalPersonajes; i++) {
        int clase = 1; // Juggernaut por defecto
        // Blindaje: si tiene una salud baja característica (80) o rapidez alta, es Espectro (Hacker)
        if (catalogoPersonajes[i].salud == 80 || catalogoPersonajes[i].id == 1) {
            clase = 3; // El Hacker de Combate es clase 3 (Espectro)
        } 
        // Si no aplica fortaleza (-1), es un enemigo atacante puro (Ejecutor/Asesino)
        else if (catalogoPersonajes[i].fortaleza == -1) {
            clase = 2; // Si no tiene fortaleza, es un enemigo (Ejecutor/Asesino)
        }

        Operativo* op = crearOperativo(clase, catalogoPersonajes[i].id, 1);
        op->HP_Base = catalogoPersonajes[i].salud; // Usar su salud real del archivo
        insertarenArbol(arbol, op);
    }
    // 2. Inyectar los héroes de heroes.txt al Bando 2 (OMEGA)
    for (int i = 0; i < totalHeroes; i++) {
        int clase = 2; // Ejecutor/Asesino por defecto para héroes equilibrados
        // Blindaje lúdico: si su fortaleza es muy alta, es Tanque (Juggernaut)
        if (catalogoHeroes[i].fortaleza > 150) {
            clase = 1; 
        } 
        // Si tiene el nombre "Oráculo de Datos" o su ID original en el archivo es 4, es Hacker
        else if (catalogoHeroes[i].id == 4 || catalogoHeroes[i].nombre.find("Oráculo") != string::npos) { // Oráculo de Datos es Hacker
            clase = 3; 
        }
        // Le sumamos un offset (+100) para que OMEGA no choque IDs con Neón
        Operativo* op = crearOperativo(clase, catalogoHeroes[i].id + 100, 2);
        op->HP_Base = catalogoHeroes[i].salud; // Usar su salud real del archivo
        insertarenArbol(arbol, op);
    }
    cout << "[YGGDRASIL]: Estructura balanceada e inyectada con unidades de los archivos." << endl;
}

int main() {
    srand(time(0)); // para evitar lanzadas de dados consistentes
    ArbolB4 yggdrasil;
    
    // ==========================================
    // LECTURA ANTES DEL JUEGO (Mapeo de los 5 TXT)
    // ==========================================
    cout << "[SISTEMA] Conectando con los archivos de red cuántica..." << endl;
    cargarBaseDeDatosYggdrasil(); // 1. Lee los 5 archivos planos y llena las matrices
    poblarArbolConCatálogos(yggdrasil); // 2. Transfiere esos datos a nodos reales de tu Árbol B-4
    
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
                    eliminarDelNodo(yggdrasil.raiz, id);
                    
                    limpiarOperativosMuertos(yggdrasil);
                    
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
                ejecutarSimulacionManualConDado(yggdrasil);
                break;

            case 0:
                cout << "\nCerrando Operacion Yggdrasil. Destruyendo estructuras..." << endl;
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