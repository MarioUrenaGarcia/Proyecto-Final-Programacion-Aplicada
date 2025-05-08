/**
 *
 * @file Nombre del archivo
 * @brief Breve Descripción
 * @details Descripción extendida
 * @date
 * @author Mario Ureña García
 *
 */

#include "tiposGTK.h"

// Prototipos de C -----------------------------------------------------------------------
// Funciones de la lista doble -----------------------------------------------------------
void insertarColaD(nodoD **first, nodoD **last, char nombreCola[], int tickets, float total);
int menuOpciones(nodoD *aux);
void insertarCaja(nodoD **first, int numCuenta, char nombreCliente[], float monedero);
void existenTerminalesFundamentales(nodoD *aux);
void atenderCaja(nodoD **caja, tipoHoja **arbol);
void borrarCliente(nodoD **terminal);
void crearArchivoFacturas(char nombreArchivo[], nodoD *aux);
int verificarColas(nodoD *aux);
void crearArchivoClientes(char nombreArchivo[], nodoD *aux);
nodoD *buscarNodo(nodoD *aux, char nombreCola[]);
void imprimirListaD(nodoD *aux);
// Funciones del árbol binario ----------------------------------------------------------
void guardarArbol(tipoHoja *aux, char nombreArchivo[]);
void cargarArbol(tipoHoja **aux, char nombreArchivo[]);
void imprimirArb(tipoHoja *aux);
// Prototipos de GTK ---------------------------------------------------------------------
void closeTheApp(GtkWidget *window);
void avanzarTerminal(GtkWidget *button, gpointer estructura);
void retrocederTerminal(GtkWidget *button, gpointer estructura);
void avanzarCliente(GtkWidget *button, gpointer estructura);
// Main ----------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  // Variables C -------------------------------------------------------------------------
  // Variables para la lectura de datos de negocio.txt
  char nombreCola[20];
  int tickets;
  float total;

  // Variables para la lectura de datos de clientes.txt
  int numCuenta;
  char nombreCliente[20];
  float monedero;

  // Para el nombre del archivo de facturas y el comando para moverlo
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  char nombreArchivo[50];
  char comando[120];

  FILE *fp;

  // Variable de apoyo para iniciar labels
  char label[100];

  // Variables Gtk -----------------------------------------------------------------------

  // Super struct-------------------------------------------------------------------------
  inter principal;

  // Procesos C --------------------------------------------------------------------------

  // Inicialización de la lista doble
  principal.inicio = NULL;
  principal.fin = NULL;
  // Inicialización del árbol binario
  principal.raiz = NULL;

  // Carga de datos desde el archivo de negocio
  fp = fopen(argv[2], "r");
  if (fp == NULL)
  {
    printf(RED "\n\n\tError al abrir el archivo de negocio\n\n" RESET);
    exit(1);
  }
  // Lectura de datos del archivo negocio.txt
  printf(YELLOW "Cargando datos de negocio.txt...\n\n" RESET);
  while (fscanf(fp, "%s\t%d\t%f", nombreCola, &tickets, &total) == 3)
  {
    // Se inserta en la lista doble
    printf(CYAN "Creando nodo de %s\n" RESET, nombreCola);
    insertarColaD(&principal.inicio, &principal.fin, nombreCola, tickets, total);
    printf(GREEN "Nodo de %s creado con éxito\n" RESET, nombreCola);
  }
  fclose(fp);
  printf(GREEN "Lista doble creada con éxito\n" RESET);
  imprimirListaD(principal.inicio); // Imprimir lista doble para verificar que se cargaron los datos correctamente

  // Verificar que existan las terminales fundamentales
  printf(YELLOW "Verificando que existan las terminales fundamentales\n\n" RESET);
  existenTerminalesFundamentales(principal.inicio);
  printf(GREEN "Terminales fundamentales verificadas con éxito\n" RESET);
  printf(GREEN "Pila de facturas inicializada con éxito\n" RESET);

  // Carga de datos desde el archivo de clientes a la terminal de la caja
  fp = fopen(argv[1], "r");
  if (fp == NULL)
  {
    printf(RED "\n\n\tError al abrir el archivo de clientes\n\n" RESET);
    exit(1);
  }
  // Lectura de datos del archivo clientes.txt
  printf(YELLOW "Cargando datos de clientes.txt...\n\n" RESET);
  while (fscanf(fp, "%d\t%s\t%f", &numCuenta, nombreCliente, &monedero) == 3)
  {
    printf(CYAN "Insertando cliente %s con cuenta %d y monedero %.2f\n" RESET, nombreCliente, numCuenta, monedero);
    insertarCaja(&principal.inicio, numCuenta, nombreCliente, monedero);
    printf(GREEN "Cliente %s insertado con éxito\n" RESET, nombreCliente);
  }
  printf(GREEN "\nClientes cargados con éxito\n" RESET);
  fclose(fp);

  // Cargar el árbol binario desde el archivo arbol.txt
  cargarArbol(&principal.raiz, argv[3]);
  if (principal.raiz == NULL)
  {
    printf(GREEN "\nEl árbol binario está vacío\n" RESET);
  }
  else
  {
    printf(GREEN "\nEl árbol binario se ha cargado con éxito" RESET);
    printf(YELLOW "\nImprimiendo árbol binario" RESET);
    imprimirArb(principal.raiz);
  }

  // Posicionar el puntero a la terminal de caja
  principal.terminalActual = principal.inicio;
  // Posicionar el puntero al cliente de la terminal de caja
  principal.clienteActual = principal.terminalActual->primero;

  printf("\n\tInicio de la aplicación\n\t");

  /**
   *
   *
   *
   *
   *
   */

  // Procesos GTK ------------------------------------------------------------------------
  /* 1. Inicializar el ambiente */
  gtk_init(&argc, &argv);

  /*2. Creación de los objetos */
  /*2b. Ventana principal y atributos*/
  principal.windowHome = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(principal.windowHome), 600, 200);
  gtk_window_set_title(GTK_WINDOW(principal.windowHome), "El Kubo");
  gtk_container_set_border_width(GTK_CONTAINER(principal.windowHome), 10);
  /*2b. Imagen*/
  principal.clienteImg = gtk_image_new_from_file("test250.jpg");
  /*2b. Contenedores*/
  principal.mainboxHome = gtk_vbox_new(FALSE, 10);
  principal.terminales = gtk_hbox_new(FALSE, 5);
  principal.cliente = gtk_hbox_new(FALSE, 5);
  principal.acciones = gtk_hbox_new(FALSE, 5);
  /*2b. Etiquetas*/
  sprintf(label, "Terminal %s, %d clientes, $ %.2f", principal.terminalActual->terminal, principal.terminalActual->clientes, principal.terminalActual->montoAcumulado);
  principal.terminalInfoLbl = gtk_label_new(label);
  sprintf(label, "Cuenta: %d, Nombre: %s, Monedero: %.2f", principal.terminalActual->primero->numCuenta, principal.terminalActual->primero->nombre, principal.terminalActual->primero->monedero);
  principal.clienteInfoLbl = gtk_label_new(label);
  principal.notificacionesLbl = gtk_label_new("Notificaciones");
  /*2b. Botones*/
  principal.terminalBackBtn = gtk_button_new_with_label("<-");
  principal.terminalNextBtn = gtk_button_new_with_label("->");
  principal.clienteBackBtn = gtk_button_new_with_label("<-");
  principal.clienteNextBtn = gtk_button_new_with_label("->");
  principal.atenderBtn = gtk_button_new_with_label("ATENDER");
  principal.buscarBtn = gtk_button_new_with_label("BUSCAR");
  /*2b. Separadores*/
  principal.accionesSeparator = gtk_vseparator_new();
  principal.clienteEndSeparator = gtk_hseparator_new();
  principal.terminalEndSeparator = gtk_hseparator_new();
  principal.clienteInfoSeparator = gtk_hseparator_new();
  /*2b. Entries*/
  principal.buscarEntry = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(principal.buscarEntry), "Introduzca # de cuenta del cliente");

  /*3. Registro de los Callbacks */
  g_signal_connect(GTK_OBJECT(principal.windowHome), "destroy", GTK_SIGNAL_FUNC(closeTheApp), NULL);
  g_signal_connect(GTK_OBJECT(principal.terminalNextBtn), "clicked", GTK_SIGNAL_FUNC(avanzarTerminal), &principal);
  g_signal_connect(GTK_OBJECT(principal.terminalBackBtn), "clicked", GTK_SIGNAL_FUNC(retrocederTerminal), &principal);
  g_signal_connect(GTK_OBJECT(principal.clienteNextBtn), "clicked", GTK_SIGNAL_FUNC(avanzarCliente), &principal);
  /* 4. Define jerarquía de instancias (pack the widgets)*/
  // Terminales
  gtk_box_pack_start_defaults(GTK_BOX(principal.terminales), principal.terminalBackBtn);
  gtk_box_pack_start_defaults(GTK_BOX(principal.terminales), principal.terminalInfoLbl);
  gtk_box_pack_start_defaults(GTK_BOX(principal.terminales), principal.terminalNextBtn);
  // Clientes
  gtk_box_pack_start_defaults(GTK_BOX(principal.cliente), principal.clienteBackBtn);
  gtk_box_pack_start_defaults(GTK_BOX(principal.cliente), principal.clienteImg);
  gtk_box_pack_start_defaults(GTK_BOX(principal.cliente), principal.clienteNextBtn);
  // Acciones
  gtk_box_pack_start_defaults(GTK_BOX(principal.acciones), principal.atenderBtn);
  gtk_box_pack_start_defaults(GTK_BOX(principal.acciones), principal.accionesSeparator);
  gtk_box_pack_start_defaults(GTK_BOX(principal.acciones), principal.buscarEntry);
  gtk_box_pack_start_defaults(GTK_BOX(principal.acciones), principal.buscarBtn);

  // ORDEN WINDOW HOME
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxHome), principal.terminales);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxHome), principal.terminalEndSeparator);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxHome), principal.cliente);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxHome), principal.clienteEndSeparator);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxHome), principal.clienteInfoLbl);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxHome), principal.clienteInfoSeparator);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxHome), principal.acciones);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxHome), principal.notificacionesLbl);

  gtk_container_add(GTK_CONTAINER(principal.windowHome), principal.mainboxHome); // Caja principal en ventana
  /* 5. Mostrar los widgets */
  gtk_widget_show_all(principal.windowHome);

  /* 6. El programa se queda en loop */
  gtk_main();

  return 0;
}
