#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///////////////////ESTRUCTURAS/////////////////////
///general
typedef struct
{
    int idSector;
    char nombreYape [30];
    char sector[30];
    int dni;
    int ventasDiarias;
} stRegistroVendedor;

///estructura vendedores y arbol de vendedores
typedef struct
{
    int dni;
    char nombreYape[30];
    int ventasDiarias;
} stVendedor;

typedef struct nodoArbol
{
    stVendedor datos;
    struct nodoArbol * izq;
    struct nodoArbol * der;
} nodoArbol;

///estructura sectores, lista de sectores
typedef struct
{
    int idSector;
    char sector[30];
} stSector;

typedef struct nodoLista
{
    stSector datos;
    nodoArbol * arbolVendedores;
    struct nodoLista * sig;
} nodoLista;

nodoArbol * crearNodoA (stVendedor vendedor);
nodoArbol * insertarOrdenado (nodoArbol * arbol, stVendedor vendedor);
void mostrarArbol (nodoArbol * arbol);
nodoLista * crearNodoL (stSector sector);
nodoLista * agregarAlprin (nodoLista * lista, stSector sector);
nodoLista * buscarSector (nodoLista * lista, int idSector);
nodoLista * pasarDatos (const char * archivo);
void mostrarLDA (nodoLista * lista);
int ventaVendedores (nodoArbol * arbol);
int ventasTotales (nodoLista * lista);
nodoArbol * buscarVendedor (nodoArbol * arbol, const char * nombreBuscado);
void buscarVendedorEnSector (nodoLista * lista, const char * nombre);




int main()
{

    const char *archivoVendedores = "archivoRegistrosVendedor.bin";
    nodoLista *lista = NULL;
    nodoArbol *arbol = NULL;

    lista = pasarDatos(archivoVendedores);

    if (lista != NULL)
    {
        printf("-----VENDEDORES Y SECTORES------\n");
        mostrarLDA(lista);

        int totalVentas = ventasTotales(lista);
        printf("Ventas diarias totales de la empresa: %i\n", totalVentas);

        char nombreBuscado[30];
        printf("Ingrese el nombre que desea buscar:\n");
        fflush(stdin);
        gets(nombreBuscado);
        buscarVendedorEnSector(lista, nombreBuscado);
    }
    else
    {
        printf("Error al cargar los datos.\n");
    }


    return 0;
}


//////////// FUNCIONES /////////////////

//PUNTO 1.
/*****************ARBOL***********************/
nodoArbol * crearNodoA (stVendedor vendedor)
{
    nodoArbol * nuevoNodo = (nodoArbol*)malloc(sizeof(nodoArbol));
    nuevoNodo->datos.dni = vendedor.dni;
    nuevoNodo->datos.ventasDiarias = vendedor.ventasDiarias;
    strcpy(nuevoNodo->datos.nombreYape, vendedor.nombreYape);
    nuevoNodo->der = NULL;
    nuevoNodo->izq = NULL;
    return nuevoNodo;
}

nodoArbol * insertarOrdenado (nodoArbol * arbol, stVendedor vendedor)
{
    nodoArbol * nuevo = crearNodoA(vendedor);
    if(arbol==NULL)
    {
        arbol = nuevo;
    }
    else if (strcmp(vendedor.nombreYape, arbol->datos.nombreYape)<0)
    {
        arbol->izq = insertarOrdenado(arbol->izq,vendedor);
    }
    else
    {
        arbol->der = insertarOrdenado(arbol->der, vendedor);
    }

    return arbol;
}

void mostrarArbol (nodoArbol * arbol)
{
    if(arbol!=NULL)
    {
        mostrarArbol(arbol->izq);
        printf("Nombre del vendedor: %s\n", arbol->datos.nombreYape);
        printf("DNI %i\n", arbol->datos.dni);
        printf("Ventas diarias %i\n", arbol->datos.ventasDiarias);
        mostrarArbol(arbol->der);
    }
}
/**********************LISTAS*****************************/
nodoLista * crearNodoL (stSector sector)
{
    nodoLista * nuevoNodo = (nodoLista*)malloc(sizeof(nodoLista));
    nuevoNodo->datos.idSector = sector.idSector;
    strcpy(nuevoNodo->datos.sector, sector.sector);
    nuevoNodo->arbolVendedores = NULL;
    nuevoNodo->sig = NULL;
    return nuevoNodo;
}

nodoLista * agregarAlprin (nodoLista * lista, stSector sector)
{
    nodoLista * nuevo = crearNodoL(sector);
    if (lista == NULL)
    {
        lista = nuevo;
    }
    else
    {
        nuevo->sig = lista;
        lista = nuevo;
    }
    return lista;
}

nodoLista * buscarSector (nodoLista * lista, int idSector)
{
    while(lista!=NULL && lista->datos.idSector != idSector)
    {
        lista = lista->sig;
    }

    return lista;
}

nodoLista * pasarDatos (const char * archivo)
{
    nodoLista * lista = NULL;
    stRegistroVendedor registro;
    FILE * archivito = fopen(archivo, "rb");

    if (archivito!=NULL)
    {
        while(fread(&registro, sizeof(stRegistroVendedor),1,archivito)>0)
        {
            stSector sector;
            sector.idSector = registro.idSector;
            strcpy(sector.sector, registro.sector);
            nodoLista * encontrado = buscarSector(lista, sector.idSector);
            if (encontrado == NULL)
            {
                lista = agregarAlprin(lista, sector);
                encontrado = lista;
            }
            stVendedor vendedor;
            vendedor.dni = registro.dni;
            vendedor.ventasDiarias = registro.ventasDiarias;
            strcpy(vendedor.nombreYape, registro.nombreYape);
            encontrado->arbolVendedores = insertarOrdenado(encontrado->arbolVendedores, vendedor);
        }
        fclose(archivito);
    }
    return lista;
}

//PUNTO 2.
void mostrarLDA (nodoLista * lista)
{
    while(lista!=NULL)
    {
        printf("Nombre del sector %s\n", lista->datos.sector);
        printf("Id del sector %i\n", lista->datos.idSector);
        mostrarArbol(lista->arbolVendedores);
        lista = lista->sig;
    }
}

//PUNTO 3.
int ventaVendedores (nodoArbol * arbol)
{
    if (arbol==NULL)
    {
        return 0;
    }
    return arbol->datos.ventasDiarias + ventaVendedores(arbol->izq) + ventaVendedores(arbol->der);
}

int ventasTotales (nodoLista * lista)
{
    int total = 0;
    while(lista!=NULL)
    {
        total += ventaVendedores(lista->arbolVendedores);
        lista = lista->sig;
    }
    return total;
}

//PUNTO 4.

nodoArbol * buscarVendedor (nodoArbol * arbol, const char * nombreBuscado)
{
    if(arbol==NULL)
    {
        return NULL;
    }

    int comparacion = strcmp(nombreBuscado, arbol->datos.nombreYape);
    if (comparacion==0)
    {
        return arbol; ///LO ENCONTRO
    }
    else if (comparacion <0)
    {
        return buscarVendedor(arbol->izq, nombreBuscado);
    }
    else
    {
        return buscarVendedor(arbol->der, nombreBuscado);
    }
}

//buscar vendedor en la lista
void buscarVendedorEnSector (nodoLista * lista, const char * nombre)
{
    int encontrado = 0;
    while(lista!=NULL)
    {
        nodoArbol * resultado = buscarVendedor(lista->arbolVendedores, nombre);
        if(resultado!=NULL)
        {
            printf("Vendedor encontrado en el sector %s, con el id %i\n", lista->datos.sector, lista->datos.idSector);
            printf("Nombre %s\n", resultado->datos.nombreYape);
            printf("DNI %i\n", resultado->datos.dni);
            printf("Ventas diarias %i\n", resultado->datos.ventasDiarias);
            encontrado = 1;
            break;
        }
        lista = lista->sig;
    }

    printf("Vendedor no encontrado");
}


