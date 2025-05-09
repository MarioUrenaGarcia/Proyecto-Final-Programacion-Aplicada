/**
 *
 * @file Nombre del archivo
 * @brief Breve Descripción
 * @details Descripción extendida
 * @date
 * @author
 *
 */

#include "tiposGTK.h"

// Prototipos de C -----------------------------------------------------------------------
// Funciones de la lista doble -----------------------------------------------------------
void insertarColaD(nodoD **first, nodoD **last, char nombreCola[], int tickets, float total);
int menuOpciones(nodoD *aux);
void insertarCaja(nodoD **first, int numCuenta, char nombreCliente[], float monedero);
void existenTerminalesFundamentales(nodoD *aux);
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

void avanzarTerminal(GtkWidget *button, gpointer estructura);
void retrocederTerminal(GtkWidget *button, gpointer estructura);
void avanzarCliente(GtkWidget *button, gpointer estructura);
void buscarHistorial(GtkWidget *button, gpointer estructura);
void ocultarSearch(GtkWidget *button, gpointer estructura);
void atender(GtkWidget *button, gpointer estructura);
void atenderCajaGTK(GtkWidget *button, gpointer estructura);
void ocultarVentanaAtencion(GtkWidget *button, gpointer estructura);

gboolean closeTheApp(GtkWidget *window, GdkEvent *event, gpointer estructura);
gboolean on_window_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);
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

  FILE *fp;

  // Variable de apoyo para iniciar labels
  char auxString[100];

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

  // Procesos GTK ------------------------------------------------------------------------
  /* 1. Inicializar el ambiente */
  gtk_init(&argc, &argv);

  /*2. Creación de los objetos */
  /*2b. Ventana principal y atributos*/
  principal.windowHome = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(principal.windowHome), 600, 200);
  gtk_window_set_title(GTK_WINDOW(principal.windowHome), "El Kubo");
  gtk_container_set_border_width(GTK_CONTAINER(principal.windowHome), 10);

  // Ventana de búsqueda
  principal.windowSearch = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(principal.windowSearch), 100, 300);
  gtk_window_set_title(GTK_WINDOW(principal.windowSearch), "Busqueda de cliente");
  gtk_container_set_border_width(GTK_CONTAINER(principal.windowSearch), 10);

  // Ventana de atención
  principal.windowAtencion = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(principal.windowAtencion), 50, 100);
  gtk_window_set_title(GTK_WINDOW(principal.windowAtencion), "Atención de cliente");
  gtk_container_set_border_width(GTK_CONTAINER(principal.windowAtencion), 10);

  /*2b. Imagen*/
  sprintf(auxString, "imagenes/%s.jpg", principal.terminalActual->primero->nombre);
  principal.clienteImg = gtk_image_new_from_file(auxString);
  principal.searchImg = gtk_image_new_from_file(auxString);
  /*2b. Contenedores*/
  principal.mainboxHome = gtk_vbox_new(FALSE, 10);
  principal.terminales = gtk_hbox_new(FALSE, 5);
  principal.cliente = gtk_hbox_new(FALSE, 5);
  principal.acciones = gtk_hbox_new(FALSE, 5);

  principal.mainboxSearch = gtk_vbox_new(FALSE, 10);

  principal.mainboxAtencion = gtk_vbox_new(FALSE, 10);
  principal.atencionComida = gtk_hbox_new(FALSE, 1);
  principal.atencionCantidad = gtk_hbox_new(FALSE, 1);
  /*2b. Etiquetas*/
  sprintf(auxString, "Terminal %s, %d clientes, $ %.2f", principal.terminalActual->terminal, principal.terminalActual->clientes, principal.terminalActual->montoAcumulado);
  principal.terminalInfoLbl = gtk_label_new(auxString);
  sprintf(auxString, "Cuenta: %d, Nombre: %s, Monedero: %.2f", principal.terminalActual->primero->numCuenta, principal.terminalActual->primero->nombre, principal.terminalActual->primero->monedero);
  principal.clienteInfoLbl = gtk_label_new(auxString);
  principal.notificacionesLbl = gtk_label_new("Notificaciones");

  principal.searchInfoLbl = gtk_label_new("Datos de Cliente");
  principal.searchInfoLbl2 = gtk_label_new("Datos de Compras");
  principal.searchInfoLbl3 = gtk_label_new("Compra Acumulada");

  principal.atencionLbl = gtk_label_new("Pedir: ");
  principal.atencionLbl2 = gtk_label_new("Cantidad: ");

  /*2b. Botones*/
  principal.terminalNextBtn = gtk_button_new_with_label("->");
  principal.terminalBackBtn = gtk_button_new_with_label("<-");
  principal.clienteNextBtn = gtk_button_new_with_label("->");
  principal.atenderBtn = gtk_button_new_with_label("ATENDER");
  principal.buscarBtn = gtk_button_new_with_label("BUSCAR");

  principal.searchBtn = gtk_button_new_with_label("OK");

  principal.atencionBtn = gtk_button_new_with_label("ATENDER");
  /*2b. Separadores*/
  principal.accionesSeparator = gtk_vseparator_new();
  principal.clienteMidSeparator = gtk_vseparator_new();
  principal.clienteEndSeparator = gtk_hseparator_new();
  principal.terminalEndSeparator = gtk_hseparator_new();
  principal.clienteInfoSeparator = gtk_hseparator_new();

  principal.searchSeparator = gtk_hseparator_new();

  principal.atencionSeparator = gtk_hseparator_new();
  /*2b. Entries*/
  principal.buscarEntry = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(principal.buscarEntry), "Introduzca # de cuenta del cliente");

  principal.atencionEntry = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(principal.atencionEntry), "Pizzas / Tacos");
  principal.atencionEntry2 = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(principal.atencionEntry2), "");

  /*3. Registro de los Callbacks */
  g_signal_connect(GTK_OBJECT(principal.windowHome), "delete-event", GTK_SIGNAL_FUNC(closeTheApp), &principal);

  g_signal_connect(GTK_OBJECT(principal.terminalNextBtn), "clicked", GTK_SIGNAL_FUNC(avanzarTerminal), &principal);
  g_signal_connect(GTK_OBJECT(principal.terminalBackBtn), "clicked", GTK_SIGNAL_FUNC(retrocederTerminal), &principal);
  g_signal_connect(GTK_OBJECT(principal.clienteNextBtn), "clicked", GTK_SIGNAL_FUNC(avanzarCliente), &principal);
  g_signal_connect(GTK_OBJECT(principal.buscarBtn), "clicked", GTK_SIGNAL_FUNC(buscarHistorial), &principal);
  g_signal_connect(GTK_OBJECT(principal.searchBtn), "clicked", GTK_SIGNAL_FUNC(ocultarSearch), &principal);
  g_signal_connect(GTK_OBJECT(principal.atenderBtn), "clicked", GTK_SIGNAL_FUNC(atender), &principal);
  g_signal_connect(GTK_OBJECT(principal.atencionBtn), "clicked", GTK_SIGNAL_FUNC(atenderCajaGTK), &principal);

  // Evitar destrucción de widgets al presionar la X
  g_signal_connect(GTK_WIDGET(principal.windowSearch), "delete-event", G_CALLBACK(on_window_delete_event), NULL);
  g_signal_connect(GTK_WIDGET(principal.windowAtencion), "delete-event", G_CALLBACK(on_window_delete_event), NULL);
  /* 4. Define jerarquía de instancias (pack the widgets)*/
  // Terminales
  gtk_box_pack_start_defaults(GTK_BOX(principal.terminales), principal.terminalBackBtn);
  gtk_box_pack_start_defaults(GTK_BOX(principal.terminales), principal.terminalInfoLbl);
  gtk_box_pack_start_defaults(GTK_BOX(principal.terminales), principal.terminalNextBtn);
  // Clientes
  gtk_box_pack_start_defaults(GTK_BOX(principal.cliente), principal.clienteMidSeparator);
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

  // ORDEN WINDOW SEARCH
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxSearch), principal.searchImg);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxSearch), principal.searchInfoLbl);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxSearch), principal.searchInfoLbl2);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxSearch), principal.searchInfoLbl3);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxSearch), principal.searchSeparator);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxSearch), principal.searchBtn);
  gtk_container_add(GTK_CONTAINER(principal.windowSearch), principal.mainboxSearch); // Caja principal en ventana

  // ORDEN WINDOW ATENCION
  // Caja de comida
  gtk_box_pack_start_defaults(GTK_BOX(principal.atencionComida), principal.atencionLbl);
  gtk_box_pack_start_defaults(GTK_BOX(principal.atencionComida), principal.atencionEntry);
  // Caja de cantidad
  gtk_box_pack_start_defaults(GTK_BOX(principal.atencionCantidad), principal.atencionLbl2);
  gtk_box_pack_start_defaults(GTK_BOX(principal.atencionCantidad), principal.atencionEntry2);
  // Principal
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxAtencion), principal.atencionComida);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxAtencion), principal.atencionCantidad);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxAtencion), principal.atencionSeparator);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxAtencion), principal.atencionBtn);
  gtk_container_add(GTK_CONTAINER(principal.windowAtencion), principal.mainboxAtencion); // Caja principal en ventana

  /* 5. Mostrar los widgets */
  gtk_widget_show_all(principal.windowHome);

  /* 6. El programa se queda en loop */
  gtk_main();

  return 0;
}
