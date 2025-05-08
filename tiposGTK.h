#include <gtk-2.0/gtk/gtk.h>

struct interfazPrincipal
{
  // Objetos GTK-----------------------------------
  
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
};
typedef struct interfazPrincipal inter;
