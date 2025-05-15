/**
 * @file defKubo.h
 * @brief Este es el header file de kubo.c
 * @date 04/04/2025
 * @author Mario Ureña García, Ricardo Ponce de León Vargas y Emiliano Cisneros Cervantes
 */

// Bibliotecas --------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gtk-2.0/gtk/gtk.h>

// Definiciones -------------------------------------------------------------------------

#define RESET "\x1b[0m"

#define RED "\x1b[31m"
#define WHITE "\x1B[37m"
#define BLUE "\x1b[34m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define CYAN "\x1b[36m"
#define MAGENTA "\x1b[35m"
#define ORANGE "\x1B[38;2;255;128;0m"
#define ROSE "\x1B[38;2;255;151;203m"
#define GRAY "\x1B[38;2;176;174;174m"
#define BLACK "\x1B[30m"

#define BG_RED "\x1B[41m"
#define BG_WHITE "\x1B[47m"
#define BG_BLUE "\x1B[44m"
#define BG_GREEN "\x1B[42m"
#define BG_YELLOW "\x1B[43m"
#define BG_CYAN "\x1B[46m"
#define BG_MAGENTA "\x1B[45m"
#define BG_ORANGE "\x1B[48;2;255;128;0m"
#define BG_ROSE "\x1B[48;2;255;151;203m"
#define BG_GRAY "\x1B[48;2;176;174;174m"
#define BG_BLACK "\x1B[40m"
// Declaración de estructuras -------------------------------------------------------------
struct factura
{
  int numFactura;
  char nombre[30];
  char compra[30];
  float totalFacturado;
  struct factura *next;
};
typedef struct factura nodoFactura;

struct cola
{
  int numCuenta;
  char nombre[30];
  float monedero;
  struct cola *next;
};
typedef struct cola nodoCola;

struct listaDoble
{
  char terminal[30];
  int clientes;
  float montoAcumulado;
  struct listaDoble *back, *next;

  nodoCola *primero, *ultimo;
  nodoFactura *top;
};
typedef struct listaDoble nodoD;

struct hoja
{
  int numCuenta;          // Número de cuenta del cliente
  char nombre[30];        // Nombre del cliente
  int numPizzas;          // Total de pizzas compradas históricamente
  int numTacos;           // Total de tacos comprados históricamente
  float compraAcumulada;  // Total gastado históricamente
  struct hoja *izq, *der; // Punteros a los hijos izquierdo y derecho
};
typedef struct hoja tipoHoja;

struct interfazPrincipal
{
  // Objetos GTK-----------------------------------

  // Ventana Principal--------------------
  GtkWidget *windowHome;
  // Contenedor principal
  GtkWidget *mainboxHome;
  // 4 cajas horizontales
  GtkWidget *terminales;
  GtkWidget *cliente;
  GtkWidget *acciones;
  // Terminales
  GtkWidget *terminalBackBtn; // Botón
  GtkWidget *terminalNextBtn; // Botón
  GtkWidget *terminalInfoLbl; // Label
  GtkWidget *terminalEndSeparator;
  // Cliente
  GtkWidget *clienteNextBtn; // Botón
  GtkWidget *clienteMidSeparator;
  GtkWidget *clienteImg; // Imagen
  GtkWidget *clienteEndSeparator;
  // Datos Cliente
  GtkWidget *clienteInfoLbl; // Label
  GtkWidget *clienteInfoSeparator;
  // Acciones
  GtkWidget *atenderBtn;        // Botón
  GtkWidget *buscarBtn;         // Botón
  GtkWidget *accionesSeparator; // Separador
  GtkWidget *buscarEntry;       // Entry
  // Notificaciones
  GtkWidget *notificacionesLbl; // Label

  // Ventana de búsqueda--------------------
  GtkWidget *windowSearch;
  // Contenedor principal
  GtkWidget *mainboxSearch;

  GtkWidget *searchImg;       // Imagen
  GtkWidget *searchInfoLbl;   // Label
  GtkWidget *searchInfoLbl2;  // Label
  GtkWidget *searchInfoLbl3;  // Label
  GtkWidget *searchSeparator; // Separador
  GtkWidget *searchBtn;       // Botón

  // Ventana de atención--------------------
  GtkWidget *windowAtencion;
  GtkWidget *mainboxAtencion;
  GtkWidget *atencionComida;
  GtkWidget *atencionCantidad;
  GtkWidget *atencionLbl;       // Label
  GtkWidget *atencionLbl2;      // Label
  GtkWidget *atencionEntry;     // Entry
  GtkWidget *atencionEntry2;    // Entry
  GtkWidget *atencionBtn;       // Botón
  GtkWidget *atencionSeparator; // Separador

  // Variables de C --------------------------------
  nodoD *inicio, *fin; // Punteros a la lista doble
  tipoHoja *raiz;      // Puntero a la raíz del árbol binario

  // Variable que definirá en que terminal se encuentra el usuario posicionado en la interfaz
  nodoD *terminalActual;
  
  // Variable que definirá en que cliente se encuentra el usuario posicionado en la interfaz
  nodoCola *clienteActual;

  nodoFactura *facturaActual;

  char nombreArchivoArbol[30]; // Nombre del archivo donde se guardará el árbol binario
};
typedef struct interfazPrincipal inter;
