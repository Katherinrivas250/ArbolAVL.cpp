//Katherin Tatiana Rivas Naranjo
//Cristian David Ortiz Valencia
//Dennis Dariana Riascos Valencia
#include <iostream>
#include <cstring>
using namespace std;

// Estructura para almacenar los datos de cada película
struct Pelicula {
    char nombre[100];    // Nombre de la película
    int anio;            // Año de lanzamiento
    char genero[50];     // Género de la película
    float recaudacion;   // Recaudación en millones
    Pelicula *izq;       // Puntero al hijo izquierdo (menores años)
    Pelicula *der;       // Puntero al hijo derecho (mayores años)
    int altura;          // Altura del nodo para balanceo AVL
};

Pelicula* raiz = NULL;   // Raíz del árbol AVL (inicialmente vacía)

// Función para obtener la altura de un nodo
// Retorna 0 si el nodo es NULL (no existe)
int altura(Pelicula* nodo) {
    if (nodo == NULL)
        return 0;
    return nodo->altura;
}

// Función que devuelve el máximo entre dos enteros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Función para pedir al usuario los datos de una nueva película
// Se inicializan los punteros y la altura
void crearPelicula(Pelicula &nueva) {
    cout << "Nombre: ";
    cin.ignore();
    cin.getline(nueva.nombre, 100);

    cout << "Año: ";
    cin >> nueva.anio;

    cout << "Género: ";
    cin.ignore();
    cin.getline(nueva.genero, 50);

    cout << "Recaudación: ";
    cin >> nueva.recaudacion;

    nueva.izq = NULL;
    nueva.der = NULL;
    nueva.altura = 1; // Nuevo nodo hoja tiene altura 1
}

// Rotación derecha para corregir desequilibrios tipo Izquierda-Izquierda
Pelicula* rotacionDerecha(Pelicula* y) {
    Pelicula* x = y->izq;
    Pelicula* t2 = x->der;

    // Realizamos la rotación
    x->der = y;
    y->izq = t2;

    // Actualizamos alturas después de la rotación
    y->altura = max(altura(y->izq), altura(y->der)) + 1;
    x->altura = max(altura(x->izq), altura(x->der)) + 1;

    return x; // Nuevo nodo raíz después de la rotación
}

// Rotación izquierda para corregir desequilibrios tipo Derecha-Derecha
Pelicula* rotacionIzquierda(Pelicula* x) {
    Pelicula* y = x->der;
    Pelicula* t2 = y->izq;

    // Realizamos la rotación
    y->izq = x;
    x->der = t2;

    // Actualizamos alturas después de la rotación
    x->altura = max(altura(x->izq), altura(x->der)) + 1;
    y->altura = max(altura(y->izq), altura(y->der)) + 1;

    return y; // Nuevo nodo raíz después de la rotación
}

// Función para obtener el factor de balance de un nodo
// Se calcula como altura del hijo izquierdo menos altura del hijo derecho
int balance(Pelicula* nodo) {
    if (nodo == NULL) return 0;
    return altura(nodo->izq) - altura(nodo->der);
}

// Función para insertar una nueva película en el árbol AVL
// Se ordena por el año de la película (clave de ordenación)
Pelicula* insertarAVL(Pelicula* nodo, Pelicula* nueva) {
    // Caso base: si el nodo es NULL, se inserta aquí
    if (nodo == NULL)
        return nueva;

    // Decidir en qué subárbol insertar según el año
    if (nueva->anio < nodo->anio)
        nodo->izq = insertarAVL(nodo->izq, nueva);
    else if (nueva->anio > nodo->anio)
        nodo->der = insertarAVL(nodo->der, nueva);
    else
        nodo->izq = insertarAVL(nodo->izq, nueva); // Si mismo año, se va por la izquierda

    // Actualizamos la altura del nodo actual
    nodo->altura = 1 + max(altura(nodo->izq), altura(nodo->der));

    // Calculamos el factor de balance para verificar desequilibrio
    int bal = balance(nodo);

    // Caso 1: Desequilibrio Izquierda-Izquierda
    if (bal > 1 && nueva->anio < nodo->izq->anio)
        return rotacionDerecha(nodo);

    // Caso 2: Desequilibrio Derecha-Derecha
    if (bal < -1 && nueva->anio > nodo->der->anio)
        return rotacionIzquierda(nodo);

    // Caso 3: Desequilibrio Izquierda-Derecha
    if (bal > 1 && nueva->anio > nodo->izq->anio) {
        nodo->izq = rotacionIzquierda(nodo->izq);
        return rotacionDerecha(nodo);
    }

    // Caso 4: Desequilibrio Derecha-Izquierda
    if (bal < -1 && nueva->anio < nodo->der->anio) {
        nodo->der = rotacionDerecha(nodo->der);
        return rotacionIzquierda(nodo);
    }

    // Si no hay desequilibrio, retornamos el nodo sin cambios
    return nodo;
}

// Recorrido Inorden para mostrar las películas ordenadas por año (de menor a mayor)
void inorden(Pelicula* nodo) {
    if (nodo == NULL) return;
    inorden(nodo->izq);
    cout << nodo->nombre << " (" << nodo->anio << ") - " << nodo->genero << " - " << nodo->recaudacion << "M\n";
    inorden(nodo->der);
}

int main() {
    Pelicula peliculas[100]; // Arreglo para almacenar películas
    int cont = 0;            // Contador de películas ingresadas
    int opcion;

    do {
        cout << "\n1. Agregar película\n2. Mostrar películas\n8. Salir\nOpción: ";
        cin >> opcion;

        switch(opcion) {
            case 1:
                if (cont >= 100) {
                    cout << "No hay más espacio para películas.\n";
                    break;
                }
                crearPelicula(peliculas[cont]);          // Pedir datos al usuario
                raiz = insertarAVL(raiz, &peliculas[cont]); // Insertar en árbol AVL
                cont++;
                break;

            case 2:
                cout << "\nPelículas ordenadas por año:\n";
                inorden(raiz); // Mostrar todas las películas ordenadas
                break;

            case 8:
                cout << "¡Hasta luego, mi amor! Que tengas un día genial.\n";
                break;

            default:
                cout << "Opción inválida. Intenta de nuevo.\n";
        }
    } while(opcion != 8);

    return 0;
}
