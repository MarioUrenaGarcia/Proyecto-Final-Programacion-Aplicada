/**
 * @file kubo.c
 * @brief En este archivo se almacenan las funciones de kubo.c
 * @date 04/04/2025
 * @author Mario Ureña García, Ricardo Ponce de León Vargas y Emiliano Cisneros Cervantes
 */

#include "tiposGTK.h"

// Funciones del árbol binario ----------------------------------------------------------
/**
 *@brief Función que inserta un nuevo nodo en el árbol
 *@param pt puntero al nodo padre
 *@param numCta número de cuenta del cliente
 *@param nombreCliente nombre del cliente
 *@param pizzas cantidad de pizzas
 *@param tacos cantidad de tacos
 *@param total total de la compra
 *@return puntero al nodo insertado
 */
tipoHoja *insertarHoja(tipoHoja *pt, int numCta, char nombreCliente[], int pizzas, int tacos, float total)
{
    tipoHoja *aux, *nuevo;
    int salir = 1;

    aux = pt;

    nuevo = (tipoHoja *)malloc(sizeof(tipoHoja));
    if (nuevo == NULL)
    {
        printf("\nNo hay memoria disponible\n");
        exit(1);
    }

    // Como es un cliente recién ingresado, se le asignan sus datos de la primera compra
    nuevo->numCuenta = numCta;
    strcpy(nuevo->nombre, nombreCliente);
    nuevo->numTacos = tacos;
    nuevo->numPizzas = pizzas;
    nuevo->compraAcumulada = total;

    nuevo->izq = NULL;
    nuevo->der = NULL;

    if (aux == NULL) // Árbol vacío
    {
        pt = nuevo;
    }

    while ((aux != NULL) && (salir != 0))
    {
        if (numCta > aux->numCuenta) // mover a la derecha
        {
            if (aux->der != NULL)
            {
                aux = aux->der;
            }
            else
            {
                aux->der = nuevo;
                salir = 0;
            }
        }
        if (numCta <= aux->numCuenta) // mover a la izquierda
        {
            if (aux->izq != NULL)
            {
                aux = aux->izq;
            }
            else
            {
                aux->izq = nuevo;
                salir = 0;
            }
            if (numCta == aux->numCuenta)
            {
                salir = 0;
            }
        }
    }
    return pt;
}

tipoHoja historialCliente(tipoHoja *aux, int numCta)
{
    tipoHoja cliente;
    int encontrado = 0;

    while (aux != NULL && encontrado == 0)
    {
        if (numCta == aux->numCuenta)
        {
            cliente.numCuenta = aux->numCuenta;
            strcpy(cliente.nombre, aux->nombre);
            cliente.numPizzas = aux->numPizzas;
            cliente.numTacos = aux->numTacos;
            cliente.compraAcumulada = aux->compraAcumulada;
            encontrado = 1;
        }
        else if (numCta < aux->numCuenta)
        {
            aux = aux->izq;
        }
        else
        {
            aux = aux->der;
        }
    }

    return cliente;
}

/**
 *@brief Función que busca un cliente en el árbol
 *@param aux puntero al nodo raiz del árbol
 *@param numCta número de cuenta del cliente
 *@return 1 si el cliente se encuentra en el árbol, 0 si no
 */
int buscarCliente(tipoHoja *aux, int numCta)
{
    int encontrado = 0;

    while (aux != NULL && encontrado == 0)
    {
        if (numCta == aux->numCuenta)
        {
            encontrado = 1;
        }
        else if (numCta < aux->numCuenta)
        {
            aux = aux->izq;
        }
        else
        {
            aux = aux->der;
        }
    }

    return encontrado;
}

/**
 *@brief Función que actualiza un cliente en el árbol
 *@param aux puntero al nodo raiz del árbol
 *@param numCta número de cuenta del cliente
 *@param pizzas cantidad de pizzas
 *@param tacos cantidad de tacos
 *@param total total de la compra
 *@return void
 */
void actualizarCliente(tipoHoja **aux, int numCta, int pizzas, int tacos, float total)
{
    tipoHoja *temp;

    temp = *aux;

    while (temp != NULL)
    {
        if (numCta == temp->numCuenta)
        {
            temp->numTacos += tacos;
            temp->numPizzas += pizzas;
            temp->compraAcumulada += total;
            break;
        }
        else if (numCta < temp->numCuenta)
        {
            temp = temp->izq;
        }
        else
        {
            temp = temp->der;
        }
    }

    return;
}

/**
 * @brief Función que guarda un nodo del árbol binario en un archivo
 * @param fp puntero al archivo
 * @param aux puntero al nodo a guardar
 * @return void
 */
void guardarNodo(FILE *fp, tipoHoja *aux)
{
    if (aux != NULL)
    {
        fprintf(fp, "%d\t%s\t%d\t%d\t%.2f\n", aux->numCuenta, aux->nombre, aux->numPizzas, aux->numTacos, aux->compraAcumulada);
        guardarNodo(fp, aux->izq);
        guardarNodo(fp, aux->der);
    }
    return;
}

/**
 * @brief Función que guarda el árbol binario en un archivo
 * @param aux puntero al nodo raiz del árbol
 * @return void
 */
extern void guardarArbol(tipoHoja *aux, char nombreArchivo[])
{
    FILE *fp;
    fp = fopen(nombreArchivo, "w");

    if (fp == NULL)
    {
        printf(RED "\nERROR: No se pudo abrir el archivo arbol.txt\n" RESET);
        exit(1);
    }

    guardarNodo(fp, aux);
    fclose(fp);

    return;
}

/**
 * @brief Función que carga el árbol binario desde un archivo
 * @param aux puntero al nodo raiz del árbol
 * @param nombreArchivo nombre del archivo que contiene el árbol
 * @return void
 */
extern void cargarArbol(tipoHoja **aux, char nombreArchivo[])
{
    FILE *fp;
    int numCta, pizzas, tacos;
    float total;
    char nombreCliente[30];

    fp = fopen(nombreArchivo, "r");
    if (fp == NULL)
    {
        // Si no se puede abrir el archivo, se crea uno nuevo
        fp = fopen(nombreArchivo, "w");
        if (fp == NULL)
        {
            printf(RED "\nERROR: No se pudo crear el archivo arbol.txt\n" RESET);
            exit(1);
        }
    }

    while (fscanf(fp, "%d\t%s\t%d\t%d\t%f", &numCta, nombreCliente, &pizzas, &tacos, &total) == 5)
    {
        *aux = insertarHoja(*aux, numCta, nombreCliente, pizzas, tacos, total);
    }
    fclose(fp);

    return;
}

/**
 *@brief Función que imprime el árbol binario
 *@param aux puntero al nodo raiz del árbol
 *@return void
 */
extern void imprimirArb(tipoHoja *aux)
{
    printf("\n");
    if (aux != NULL)
    {
        imprimirArb(aux->izq);
        printf("\n");
        printf(YELLOW "Cuenta:" RESET " %d\t" YELLOW "Nombre:" RESET " %s\n", aux->numCuenta, aux->nombre);
        printf(YELLOW "Tacos:" RESET " %d\t" YELLOW "Pizzas:" RESET " %d\n", aux->numTacos, aux->numPizzas);
        printf(YELLOW "Compra acumulada:" RESET " $%.2f", aux->compraAcumulada);
        imprimirArb(aux->der);
    }
    return;
}

// Funciones Kubo ----------------------------------------------------------------------------
/**
 * @brief Función que busca un nodo en la lista doble usando el nombre del nodo.
 * @param *aux Puntero a la lista doble que contiene los puestos de comida.
 * @param nombreCola[] Nombre de la cola a buscar.
 * @return *aux Puntero al nodo encontrado o NULL si no se encuentra.
 */
extern nodoD *buscarNodo(nodoD *aux, char nombreCola[])
{
    // Buscar el nodo con el nombre de la cola
    while (aux != NULL)
    {
        if (strcmp(aux->terminal, nombreCola) == 0)
        {
            break; // Se encontró el nodo
        }
        aux = aux->next;
    }
    return aux; // Si no se encuentra el nodo, se devuelve NULL
}

/**
 * @brief Función que inserta un cliente en la cola de comida.
 * @param **terminal Puntero a la lista doble que contiene los puestos de comida.
 * @param *cliente Puntero a la cola que contiene los datos del cliente.
 * @return void
 */
void insertarClienteComida(nodoD **terminal, nodoCola *cliente)
{
    nodoCola *nuevo;

    nuevo = (nodoCola *)malloc(sizeof(nodoCola));
    if (nuevo == NULL)
    {
        printf(RED "\nERROR: No hay memoria disponible\n" RESET);
        exit(1);
    }
    nuevo->numCuenta = cliente->numCuenta;
    strcpy(nuevo->nombre, cliente->nombre);
    nuevo->monedero = cliente->monedero;
    nuevo->next = NULL;

    // Caso: Cola vacía
    if (((*terminal)->primero == NULL) && ((*terminal)->ultimo == NULL))
    {
        (*terminal)->primero = nuevo;
        (*terminal)->ultimo = nuevo;
    }
    else // Caso: Cola no vacía
    {
        (*terminal)->ultimo->next = nuevo;
        (*terminal)->ultimo = nuevo;
    }
    // Actualizar el número de clientes y el monto acumulado
    (*terminal)->clientes++;
    (*terminal)->montoAcumulado += nuevo->monedero;
    return;
}

/**
 * @brief Función que crea una factura y la inserta en la pila de facturas.
 * @param **facturacion Puntero a la lista doble que contiene la terminal de facturacion.
 * @param nombreCliente[] Nombre del cliente.
 * @param nombreTerminal[] Nombre de la terminal donde se realizó la compra.
 * @param total Total de la compra.
 * @return void
 */
void crearFactura(nodoD **facturacion, char nombreCliente[], char nombreTerminal[], float total)
{
    static int numeroFactura = 1;
    nodoFactura *nuevo;

    nuevo = (nodoFactura *)malloc(sizeof(nodoFactura));
    if (nuevo == NULL)
    {
        printf(RED "\nERROR: No hay memoria disponible\n" RESET);
        exit(1);
    }

    // Crear la factura
    nuevo->numFactura = numeroFactura;
    strcpy(nuevo->nombre, nombreCliente);
    strcpy(nuevo->compra, nombreTerminal);
    nuevo->totalFacturado = total;
    nuevo->next = (*facturacion)->top;
    (*facturacion)->top = nuevo;
    numeroFactura++;

    // Actualizar el número de clientes y el monto acumulado
    (*facturacion)->clientes++;
    (*facturacion)->montoAcumulado += total;
    return;
}

/**
 * @brief Función que crea un archivo de texto con los datos de los clientes en la cola de caja.
 * @param nombreArchivo[] Nombre del archivo a crear.
 * @param *aux Puntero a la lista doble que contiene los puestos de comida.
 * @return void
 */
extern void crearArchivoClientes(char nombreArchivo[], nodoD *aux)
{
    FILE *fp;
    nodoCola *clientesInicio, *clientesFin;

    // Buscar la terminal de caja
    while (aux != NULL && (strcmp(aux->terminal, "Caja") != 0))
    {
        aux = aux->next;
    }
    // Cuando se encuentra la terminal de caja, se asigna a clientes
    clientesInicio = aux->primero;
    clientesFin = aux->ultimo;
    // Crear el archivo de clientes
    fp = fopen(nombreArchivo, "w");
    if (fp == NULL)
    {
        printf(RED "\nERROR: No se pudo crear el archivo de clientes\n" RESET);
        exit(1);
    }
    // Imprimir la lista de clientes
    // Verificar que la cola no esté vacía
    if (clientesInicio == NULL && clientesFin == NULL)
    {
        fprintf(fp, "0\n");
    }
    else
    {
        do
        {
            fprintf(fp, "%d\t%s\t%.2f\n", clientesInicio->numCuenta, clientesInicio->nombre, clientesInicio->monedero);
            clientesInicio = clientesInicio->next;
        } while (clientesInicio != clientesFin->next);
    }

    return;
}

/**
 * @brief Función que verifica si hay clientes en las colas de comida.
 * @param *aux Puntero a la lista doble que contiene los puestos de comida.
 * @return clientes 1 si hay clientes en las colas de comida, 0 si no hay clientes.
 */
extern int verificarColas(nodoD *aux)
{
    int clientes = 0;
    // Verificar que no haya clientes en las colas de comida
    while (aux != NULL)
    {
        if (strcmp(aux->terminal, "Caja") != 0 && strcmp(aux->terminal, "Facturacion") != 0)
        {
            if (aux->primero != NULL)
            {
                printf(RED "\n\n\tError: Aún hay clientes en la cola de %s, no puedes cerrar el programa!\n\n" RESET, aux->terminal);
                clientes = 1;
            }
        }
        aux = aux->next;
    }

    return clientes;
}

/**
 * @brief Función que crea un archivo de texto con los datos de las facturas.
 * @param nombreArchivo[] Nombre del archivo a crear.
 * @param aux Puntero a la lista doble que contiene los puestos de comida.
 * @return void
 */
extern void crearArchivoFacturas(char nombreArchivo[], nodoD *aux)
{
    FILE *fp;
    nodoFactura *facturas = NULL;

    fp = fopen(nombreArchivo, "w");
    if (fp == NULL)
    {
        printf(RED "\nERROR: No se pudo crear el archivo de facturas\n" RESET);
        exit(1);
    }
    // Buscar la terminal de facturacion
    while (aux != NULL && (strcmp(aux->terminal, "Facturacion") != 0))
    {
        aux = aux->next;
    }

    // Imprimir la pila de facturas
    facturas = aux->top;

    // Si no hay facturas, se almacena como 0
    if (facturas == NULL)
    {
        fprintf(fp, "0\n");
    }
    // Imprimir la lista de facturas
    while (facturas != NULL)
    {
        fprintf(fp, "%d\t%s\t%s\t%.2f\n", facturas->numFactura, facturas->nombre, facturas->compra, facturas->totalFacturado);
        facturas = facturas->next;
    }
    fclose(fp);

    return;
}

/**
 * @brief Función que elimina un cliente de la cola.
 * @param **terminal Puntero a la lista doble que contiene los puestos de comida.
 * @return void
 */
extern void borrarCliente(nodoD **terminal)
{
    nodoCola *borra;

    // Verificar si la cola está vacía
    if ((*terminal)->primero == NULL && (*terminal)->ultimo == NULL)
    {
        printf(RED "\nLa cola %s está vacía" RESET, (*terminal)->terminal);
    }
    else
    {
        borra = (*terminal)->primero;

        // Caso: Un solo cliente en la cola
        if ((*terminal)->primero == (*terminal)->ultimo)
        {
            (*terminal)->primero = NULL;
            (*terminal)->ultimo = NULL;
        }
        else // Caso: Más de un cliente en la cola
        {
            (*terminal)->primero = (*terminal)->primero->next;
            (*terminal)->ultimo->next = (*terminal)->primero; // Mantener la circularidad
        }

        free(borra); // Liberar la memoria del cliente eliminado

        // Actualizar el número de clientes y el monto acumulado
        (*terminal)->clientes--;
        (*terminal)->montoAcumulado -= borra->monedero;

        printf(GREEN "\nCliente eliminado de la cola %s" RESET, (*terminal)->terminal);
    }
    return;
}

/**
 * @brief Función que inserta un nodoD en la lista doble.
 * @param first Puntero a la cabeza de la lista doble.
 * @param last Puntero a la cola de la lista doble.
 * @param nombreCola Nombre de la cola a insertar.
 * @param tickets Número de tickets en la cola.
 * @param total Monto acumulado en la cola.
 */
extern void insertarColaD(nodoD **first, nodoD **last, char nombreCola[], int tickets, float total)
{
    nodoD *nuevo;

    nuevo = (nodoD *)malloc(sizeof(nodoD));
    if (nuevo == NULL)
    {
        printf(RED "\nERROR: No hay memoria disponible\n" RESET);
        exit(1);
    }
    strcpy(nuevo->terminal, nombreCola);
    nuevo->clientes = tickets;
    nuevo->montoAcumulado = total;

    // Inicializar subcolas
    nuevo->primero = NULL;
    nuevo->ultimo = NULL;
    nuevo->top = NULL;

    // Caso: Lista vacía
    if ((*first == NULL) && (*last == NULL))
    {
        nuevo->back = NULL;
        nuevo->next = NULL;
        *first = nuevo;
        *last = nuevo;
    }
    else // Caso: Lista no vacía
    {
        nuevo->back = *last;
        nuevo->next = NULL;
        (*last)->next = nuevo;
        *last = nuevo;
    }

    return;
}

/**
 * @brief Función que inserta un cliente en la cola de caja.
 * @param **first Puntero a la cabeza de la lista doble.
 * @param numCuenta Número de cuenta del cliente.
 * @param nombreCliente[] Nombre del cliente.
 * @param monedero Monto del monedero del cliente.
 * @return void
 */
extern void insertarCaja(nodoD **first, int numCuenta, char nombreCliente[], float monedero)
{
    nodoCola *nuevo;
    nodoD *aux;

    // Puede que la caja no esté en la primera posición del archivo negocios.txt
    // Por lo que se debe buscar en toda la lista doble y almacenarla en una variable caja
    nodoD **caja = NULL;

    // Buscar la cola con nombre Caja
    aux = *first;
    while (aux != NULL)
    {
        if (strcmp(aux->terminal, "Caja") == 0)
        {
            caja = &aux;
            break;
        }
        aux = aux->next;
    }

    nuevo = (nodoCola *)malloc(sizeof(nodoCola));
    if (nuevo == NULL)
    {
        printf(RED "\nERROR: No hay memoria disponible\n" RESET);
        exit(1);
    }
    nuevo->numCuenta = numCuenta;
    strcpy(nuevo->nombre, nombreCliente);
    nuevo->monedero = monedero;
    nuevo->next = NULL;

    // Caso: Cola vacía
    if (((*caja)->primero == NULL) && ((*caja)->ultimo == NULL))
    {
        (*caja)->primero = nuevo;
        (*caja)->ultimo = nuevo;
    }
    else // Caso: Cola no vacía
    {
        (*caja)->ultimo->next = nuevo;
        (*caja)->ultimo = nuevo;
    }

    // Actualizar el número de clientes y el monto acumulado
    (*caja)->clientes++;
    (*caja)->montoAcumulado += nuevo->monedero;

    return;
}

/**
 * @brief Función que atiende la cola de caja.
 * @param **caja Puntero a la lista doble que contiene la terminal de caja.
 * @return void
 */
extern void atenderCaja(nodoD **caja, tipoHoja **arbol, int cantidad, char comida[])
{
    int compraValida = 0;
    float pagar;
    nodoD *nodoCompra;      // Para ubicar la terminal de la compra
    nodoD *nodoFacturacion; // Para ubicar la terminal de facturacion
    nodoCola *clienteActual;

    // Verificar que la cola no esté vacía
    if (((*caja)->primero == NULL) && ((*caja)->ultimo == NULL))
    {
        printf(GREEN "\nLa caja está vacía\n" RESET);
    }
    else // Cola con clientes
    {
        clienteActual = (*caja)->primero;

        // Atender a la caja eliminara al ultimo cliente de la cola FIFO y se copiaran sus datos a la terminal donde haya comprado
        printf("Atendiendo a %s " GREEN "$%.2f" RESET, clienteActual->nombre, clienteActual->monedero);

        if (strcmp(comida, "Pizzas") == 0) // Pizzas
        {
            pagar = cantidad * 89;
            if (pagar <= clienteActual->monedero)
            {
                clienteActual->monedero -= pagar;
                printf(GREEN "\nCompra exitosa\n" RESET);
                compraValida = 1;
                nodoCompra = buscarNodo(*caja, "Pizzas");
            }
            else
            {
                printf(RED "\nNo tiene suficiente dinero\n" RESET);
                compraValida = -1;
            }
        }
        else if (strcmp(comida, "Tacos") == 0) // Tacos
        {
            pagar = cantidad * 25;
            if (pagar <= clienteActual->monedero)
            {
                clienteActual->monedero -= pagar;
                printf(GREEN "\nCompra exitosa" RESET);
                compraValida = 1;
                nodoCompra = buscarNodo(*caja, "Tacos");
            }
            else
            {
                printf(RED "\nNo tiene suficiente dinero\n" RESET);
                compraValida = -1;
            }
        }

        // Si la compraValida == 1, se elimina el cliente de la cola y se enviará a la colaActual
        // Si la compraValida == -1, solamente se eliminará al cliente de la cola
        if (compraValida == 1)
        {
            insertarClienteComida(&nodoCompra, clienteActual);

            // Buscar la terminal de facturacion
            nodoFacturacion = buscarNodo(*caja, "Facturacion");

            printf(GREEN "\nNodo de facturacion creado" RESET);
            crearFactura(&nodoFacturacion, clienteActual->nombre, nodoCompra->terminal, pagar);

            // Agregar al cliente al árbol binario
            // Si el cliente no existe en el árbol
            if (buscarCliente(*arbol, clienteActual->numCuenta) == 0)
            {
                // Insertar el cliente en el árbol
                printf("\nInsertando cliente en árbol");
                if (strcmp(nodoCompra->terminal, "Pizzas") == 0)
                {
                    *arbol = insertarHoja(*arbol, clienteActual->numCuenta, clienteActual->nombre, cantidad, 0, pagar);
                }
                else if (strcmp(nodoCompra->terminal, "Tacos") == 0)
                {
                    *arbol = insertarHoja(*arbol, clienteActual->numCuenta, clienteActual->nombre, 0, cantidad, pagar);
                }
                printf(GREEN "\nCliente insertado en árbol" RESET);
            }
            else // Si el cliente ya existe en el árbol
            {
                // Actualizar el cliente en el árbol
                printf("\nActualizando cliente en árbol");
                if (strcmp(nodoCompra->terminal, "Pizzas") == 0)
                {
                    actualizarCliente(arbol, clienteActual->numCuenta, cantidad, 0, pagar);
                }
                else if (strcmp(nodoCompra->terminal, "Tacos") == 0)
                {
                    actualizarCliente(arbol, clienteActual->numCuenta, 0, cantidad, pagar);
                }
                printf(GREEN "\nCliente actualizado en árbol" RESET);
            }
        }
        // Actualizar valores de terminal
        (*caja)->clientes--;
        (*caja)->montoAcumulado -= clienteActual->monedero;
    }

    return;
}

/**
 * @brief Función que verifica si existen las terminales fundamentales (Caja y Facturacion).
 * @param *aux Puntero a la lista doble que contiene los puestos de comida.
 * @return void
 */
extern void existenTerminalesFundamentales(nodoD *aux) // Esta función inicia la pila de facturas también
{
    // Debe de existir la terminal Caja y la terminal Facturacion
    int caja = 0, facturacion = 0;
    while (aux != NULL)
    {
        if (strcmp(aux->terminal, "Caja") == 0)
        {
            caja = 1;
        }
        if (strcmp(aux->terminal, "Facturacion") == 0)
        {
            facturacion = 1;
            aux->top = NULL; // Inicializar la pila de facturas
        }
        aux = aux->next;
    }
    if (caja == 0)
    {
        printf(RED "\n\n\tError: No existe la terminal Caja\n\n" RESET);
        exit(1);
    }
    if (facturacion == 0)
    {
        printf(RED "\n\n\tError: No existe la terminal Facturacion\n\n" RESET);
        exit(1);
    }
    return;
}

/**
 * @brief Función que imprime el estado de todas las colas.
 * @param *aux Puntero a la lista doble que contiene los puestos de comida.
 * @return void
 */
extern void imprimirListaD(nodoD *aux)
{
    printf(YELLOW "\nEstado de Cajas\n" RESET);

    if (aux == NULL)
    {
        printf(RED "\nLa lista está vacía\n" RESET);
        return;
    }
    else
    {
        while (aux != NULL)
        {
            printf(YELLOW "\nNombre de la cola:" RESET " %s\n", aux->terminal);
            printf(GREEN "Número de clientes:" RESET " %d\n", aux->clientes);
            printf(GREEN "Monto acumulado:" RESET " $%.2f\n", aux->montoAcumulado);
            aux = aux->next;
        }
    }

    return;
}