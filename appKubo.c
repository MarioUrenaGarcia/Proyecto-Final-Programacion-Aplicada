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

// Prototipos de GTK ---------------------------------------------------------------------
void closeTheApp(GtkWidget *window);
// Main ----------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  // Variables C -------------------------------------------------------------------------
  
  // Variables Gtk -----------------------------------------------------------------------
  
  // Super struct-------------------------------------------------------------------------
  inter principal;

  // Procesos C --------------------------------------------------------------------------
  
  // Procesos GTK ------------------------------------------------------------------------
  /* 1. Inicializar el ambiente */
  gtk_init(&argc, &argv);
  
  /*2. Creación de los objetos */
  /*2b. Ventana principal y atributos*/
  principal.windowHome = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(principal.windowHome),600,200);
  gtk_window_set_title(GTK_WINDOW(principal.windowHome),"El Kubo");
  gtk_container_set_border_width(GTK_CONTAINER(principal.windowHome),10);
  /*2b. Imagen*/
  principal.clienteImg = gtk_image_new_from_file("test250.jpg");
  /*2b. Contenedores*/
  principal.mainboxHome = gtk_vbox_new(FALSE,10);
  principal.terminales = gtk_hbox_new(FALSE,5);
  principal.cliente = gtk_hbox_new(FALSE,5);
  principal.acciones = gtk_hbox_new(FALSE,5);
  /*2b. Etiquetas*/
  principal.terminalInfoLbl = gtk_label_new("Terminal, # clientes, $ acumulado");
  principal.clienteInfoLbl = gtk_label_new("# cuenta, nombre, $ monedero");
  /*2b. Botones*/
  principal.terminalBackBtn = gtk_button_new_with_label("<-");;
  principal.terminalNextBtn = gtk_button_new_with_label("->");;
  principal.clienteBackBtn = gtk_button_new_with_label("<-");;
  principal.clienteNextBtn = gtk_button_new_with_label("->");;
  principal.atenderBtn = gtk_button_new_with_label("ATENDER");;
  principal.buscarBtn = gtk_button_new_with_label("BUSCAR");;
  /*2b. Separadores*/
  principal.accionesSeparator = gtk_hseparator_new( );
  principal.clienteEndSeparator = gtk_hseparator_new( );
  principal.terminalEndSeparator = gtk_hseparator_new( );
  principal.clienteInfoSeparator = gtk_hseparator_new( );
  
  /*3. Registro de los Callbacks */
  g_signal_connect(GTK_OBJECT(principal.windowHome), "destroy", GTK_SIGNAL_FUNC(closeTheApp), NULL);
  
  /* 4. Define jerarquía de instancias (pack the widgets)*/
  //Terminales
  gtk_box_pack_start_defaults(GTK_BOX(principal.terminales), principal.terminalBackBtn);
  gtk_box_pack_start_defaults(GTK_BOX(principal.terminales), principal.terminalInfoLbl);
  gtk_box_pack_start_defaults(GTK_BOX(principal.terminales), principal.terminalNextBtn);
  //Clientes
  gtk_box_pack_start_defaults(GTK_BOX(principal.cliente), principal.clienteBackBtn);
  gtk_box_pack_start_defaults(GTK_BOX(principal.cliente), principal.clienteImg);
  gtk_box_pack_start_defaults(GTK_BOX(principal.cliente), principal.clienteNextBtn);
  //Acciones
  gtk_box_pack_start_defaults(GTK_BOX(principal.acciones), principal.atenderBtn);
  gtk_box_pack_start_defaults(GTK_BOX(principal.acciones), principal.accionesSeparator);
  gtk_box_pack_start_defaults(GTK_BOX(principal.acciones), principal.buscarBtn);

  //ORDEN WINDOW HOME
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxHome), principal.terminales);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxHome), principal.terminalEndSeparator);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxHome), principal.cliente);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxHome), principal.clienteEndSeparator);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxHome), principal.clienteInfoLbl);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxHome), principal.clienteInfoSeparator);
  gtk_box_pack_start_defaults(GTK_BOX(principal.mainboxHome), principal.acciones);
  
  gtk_container_add(GTK_CONTAINER(principal.windowHome),principal.mainboxHome); // Caja principal en ventana
  /* 5. Mostrar los widgets */
  gtk_widget_show_all(principal.windowHome);
  
  /* 6. El programa se queda en loop */
  gtk_main();
  
  return 0;
}
