/**
 * @file
 * @brief
 * @date
 * @author
 */

#include "tiposGTK.h"

// Funciones ----------------------------------------------------------------------------

/**
 * @brief
 * @date
 * @author
 * @param
 * @return
 * @Ejemplo
 */

void closeTheApp(GtkWidget *window)
{
  printf("\nPrograma Finalizado\n");
  gtk_main_quit();
  return;
}

// Función que avanza a la siguiente terminal
extern void avanzarTerminal(GtkWidget *button, gpointer estructura)
{
  inter *pt = (inter *)estructura;
  char label[100];

  // Si la terminal actual no es la última, avanza a la siguiente
  if (pt->terminalActual->next != NULL)
  {
    pt->terminalActual = pt->terminalActual->next;

    // Actualiza la etiqueta de la terminal
    sprintf(label, "Terminal %s, %d clientes, $ %.2f", pt->terminalActual->terminal, pt->terminalActual->clientes, pt->terminalActual->montoAcumulado);
    gtk_label_set_text(GTK_LABEL(pt->terminalInfoLbl), label);

    // Actualiza la fila de clientes donde se encuentra el usuario
    pt->clienteActual = pt->terminalActual->primero;

    // Si la fila está vacía, no hay clientes
    if (pt->clienteActual == NULL)
    {
      gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), "No hay clientes en la fila");
    }
    else
    {
      // Actualiza la etiqueta del cliente
      sprintf(label, "Cuenta: %d, Nombre: %s, Monedero: %.2f", pt->clienteActual->numCuenta, pt->clienteActual->nombre, pt->clienteActual->monedero);
      gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), label);
    }

    // Notifica que se avanzó a la siguiente terminal
    gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Avanzaste a la siguiente terminal");
  }

  return;
}

// Función que retrocede a la terminal anterior
extern void retrocederTerminal(GtkWidget *button, gpointer estructura)
{
  inter *pt = (inter *)estructura;
  char label[100];

  // Si la terminal actual no es la primera, retrocede a la anterior
  if (pt->terminalActual->back != NULL)
  {
    pt->terminalActual = pt->terminalActual->back;

    // Actualiza la etiqueta de la terminal
    sprintf(label, "Terminal %s, %d clientes, $ %.2f", pt->terminalActual->terminal, pt->terminalActual->clientes, pt->terminalActual->montoAcumulado);
    gtk_label_set_text(GTK_LABEL(pt->terminalInfoLbl), label);

    // Actualiza la fila de clientes donde se encuentra el usuario
    pt->clienteActual = pt->terminalActual->primero;

    // Si la fila está vacía, no hay clientes
    if (pt->clienteActual == NULL)
    {
      gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), "No hay clientes en la fila");
    }
    else
    {
      // Actualiza la etiqueta del cliente
      sprintf(label, "Cuenta: %d, Nombre: %s, Monedero: %.2f", pt->clienteActual->numCuenta, pt->clienteActual->nombre, pt->clienteActual->monedero);
      gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), label);
    }

    // Notifica que se retrocedió a la terminal anterior
    gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Retrocediste a la terminal anterior");
  }

  return;
}

// Función que avanza al siguiente cliente
extern void avanzarCliente(GtkWidget *button, gpointer estructura)
{
  inter *pt = (inter *)estructura;
  char label[100];

  // Si la fila de clientes no está vacía
  if (pt->clienteActual != NULL)
  {
    // Si el cliente actual no es el último, avanza al siguiente
    if (pt->clienteActual->next != NULL)
    {
      // Avanza al siguiente cliente
      pt->clienteActual = pt->clienteActual->next;

      // Actualiza la etiqueta del cliente
      sprintf(label, "Cuenta: %d, Nombre: %s, Monedero: %.2f", pt->clienteActual->numCuenta, pt->clienteActual->nombre, pt->clienteActual->monedero);
      gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), label);

      // Notifica que se avanzó al siguiente cliente
      gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Avanzaste al siguiente cliente");
    }
    else
    {
      // Si el cliente actual es el último, regresa al inicio
      pt->clienteActual = pt->terminalActual->primero;

      // Actualiza la etiqueta del cliente
      sprintf(label, "Cuenta: %d, Nombre: %s, Monedero: %.2f", pt->clienteActual->numCuenta, pt->clienteActual->nombre, pt->clienteActual->monedero);
      gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), label);

      // Si el cliente actual es el último, notifica que no hay más clientes en la fila y regresa al inicio
      gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "No hay más clientes en la fila, iniciando de nuevo");
    }
  }

  return;
}
