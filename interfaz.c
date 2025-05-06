#include <gtk-2.0/gtk/gtk.h>

int main(int argc, char *argv[])
{
  // Declaración Objetos GTK-----------------------------------
  
  // Ventana Principal--------------------
  GtkWidget *windowHome;
  GtkWidget *mainboxHome;
  //3 cajas horizontales
  GtkWidget *terminales;
  GtkWidget *cliente;
  GtkWidget *acciones;
  //Terminales
  GtkWidget *terminalBackBtn; // Botón
  GtkWidget *terminalNextBtn; // Botón
  GtkWidget *terminalInfoLbl; // Label
  GtkWidget *terminalEndSeparator;
  // Cliente
  GtkWidget *clienteBackBtn; // Botón
  GtkWidget *clienteNextBtn; // Botón
  GtkWidget *clienteImg; // Imagen
  GtkWidget *clienteEndSeparator;
  // Datos Cliente
  GtkWidget *clienteInfoLbl; // Label
  GtkWidget *clienteInfoSeparator;
  //Acciones
  GtkWidget *atenderBtn; // Botón
  GtkWidget *buscarBtn; // Botón
  GtkWidget *accionesSeparator; // Separador
  
  /* 1. Inicializar el ambiente */
  gtk_init(&argc, &argv);

  /* 1b. Definir Colores */
  
  /*2. Creación de los objetos */
  /*2b. Ventana principal y atributos*/
  windowHome = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(windowHome),600,200);
  gtk_window_set_title(GTK_WINDOW(windowHome),"El Kubo");
  gtk_container_set_border_width(GTK_CONTAINER(windowHome),10);
  /*2b. Imagen*/
  clienteImg = gtk_image_new_from_file("test250.jpg");
  /*2b. Contenedores*/
  mainboxHome = gtk_vbox_new(FALSE,10);
  terminales = gtk_hbox_new(FALSE,5);
  cliente = gtk_hbox_new(FALSE,5);
  acciones = gtk_hbox_new(FALSE,5);
  /*2b. Etiquetas*/
  terminalInfoLbl = gtk_label_new("Terminal, # clientes, $ acumulado");
  clienteInfoLbl = gtk_label_new("# cuenta, nombre, $ monedero");
  /*2b. Botones*/
  terminalBackBtn = gtk_button_new_with_label("<-");;
  terminalNextBtn = gtk_button_new_with_label("->");;
  clienteBackBtn = gtk_button_new_with_label("<-");;
  clienteNextBtn = gtk_button_new_with_label("->");;
  atenderBtn = gtk_button_new_with_label("ATENDER");;
  buscarBtn = gtk_button_new_with_label("BUSCAR");;
  /*2b. Separadores*/
  accionesSeparator = gtk_hseparator_new( );
  clienteEndSeparator = gtk_hseparator_new( );
  terminalEndSeparator = gtk_hseparator_new( );
  clienteInfoSeparator = gtk_hseparator_new( );

  /*2b. Modificar colores*/
  
  /*3. Registro de los Callbacks */

  /* 4. Define jerarquía de instancias (pack the widgets)*/
  //Terminales
  gtk_box_pack_start_defaults(GTK_BOX(terminales), terminalBackBtn);
  gtk_box_pack_start_defaults(GTK_BOX(terminales), terminalInfoLbl);
  gtk_box_pack_start_defaults(GTK_BOX(terminales), terminalNextBtn);
  //Clientes
  gtk_box_pack_start_defaults(GTK_BOX(cliente), clienteBackBtn);
  gtk_box_pack_start_defaults(GTK_BOX(cliente), clienteImg);
  gtk_box_pack_start_defaults(GTK_BOX(cliente), clienteNextBtn);
  //Acciones
  gtk_box_pack_start_defaults(GTK_BOX(acciones), atenderBtn);
  gtk_box_pack_start_defaults(GTK_BOX(acciones), accionesSeparator);
  gtk_box_pack_start_defaults(GTK_BOX(acciones), buscarBtn);

  //ORDEN WINDOW HOME
  gtk_box_pack_start_defaults(GTK_BOX(mainboxHome), terminales);
  gtk_box_pack_start_defaults(GTK_BOX(mainboxHome), terminalEndSeparator);
  gtk_box_pack_start_defaults(GTK_BOX(mainboxHome), cliente);
  gtk_box_pack_start_defaults(GTK_BOX(mainboxHome), clienteEndSeparator);
  gtk_box_pack_start_defaults(GTK_BOX(mainboxHome), clienteInfoLbl);
  gtk_box_pack_start_defaults(GTK_BOX(mainboxHome), clienteInfoSeparator);
  gtk_box_pack_start_defaults(GTK_BOX(mainboxHome), acciones);
  
  gtk_container_add(GTK_CONTAINER(windowHome),mainboxHome); // Caja principal en ventana
  /* 5. Mostrar los widgets */
  gtk_widget_show_all(windowHome);
  
  /* 6. El programa se queda en loop */
  gtk_main();
  
  return 0;
}
