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

int main() {
    srand(time(0)); // para evitar lanzadas de dados consistentes
    // PROGRAMA DE PRUEBA
    ArbolB4 yggdrasil;
    cout << "--- INICIANDO OPERACION YGGDRASIL ---" << endl;

    // se insertan valores predeterminados del pdf del proyecto
    cout << "\n1 - PRUEBA DE INSERCION ORDENADA:" << endl;
    insertarenArbol(yggdrasil, crearOperativo(1, 10, 1)); // borin (equipo neon)
    insertarenArbol(yggdrasil, crearOperativo(3, 20, 1)); // dra. carter (equipo neon)
    insertarenArbol(yggdrasil, crearOperativo(2, 30, 2)); // omega genérico
    
    cout << "IDs en raiz: ";
    for(int i=0; i < yggdrasil.raiz->cantidad_actual; i++) {
        cout << "[" << yggdrasil.raiz->ocupantes[i]->ID_Clave << "] ";
    }
    cout << endl;

    // prueba de split (rompe al insertar el 25)
    cout << endl;
    cout << "2 - PRUEBA DE SPLIT " << endl;
    insertarenArbol(yggdrasil, crearOperativo(1, 25, 2));
    mostrarArbol(yggdrasil.raiz, 0);

    // busqueda
    cout << endl;
    cout << "3 - PRUEBA DE BUSQUEDA (Ruta al ID 25):" << endl;
    buscarOperativo(yggdrasil.raiz, 25, 0);

    // insercion para el borrado
    cout << endl;
    cout << "4 - PREPARANDO EL ARBOL PARA ELIMINACION..." << endl;
    insertarenArbol(yggdrasil, crearOperativo(2, 5, 1));
    insertarenArbol(yggdrasil, crearOperativo(1, 15, 2));
    mostrarArbol(yggdrasil.raiz, 0);

    // prueba de borrado manual
    cout << endl;
    cout << "5 - PRUEBA DE ELIMINACION (Eliminando ID 10):" << endl;
    // deberia forzar una reestructuración con <1 operativos
    eliminarDelNodo(yggdrasil.raiz, 10);
    
    cout << "Arbol tras extirpacion del ID 10:" << endl;
    mostrarArbol(yggdrasil.raiz, 0);

    // verificacion final
    cout << endl;
    cout << "6. VERIFICACION DE SUMINISTROS (ID 20):" << endl;
    // se busca el operativo por su ID desde la raiz
    Operativo* carter = buscarYRetornarPersonaje(yggdrasil.raiz, 20);

    if (carter != nullptr) {
        inspeccionarSuministrosporID(carter);
    } else {
        cout << "Error: No se encontro al operativo 20 (carter) para la inspeccion." << endl;
    }

    cout << endl;
    cout << "--- PRUEBA DE ESTRUCTURAS COMPLETADA ---" << endl;

    return 0;
}