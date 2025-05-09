/**
 * @file
 * @brief
 * @date
 * @author
 */

#include "tiposGTK.h"

int buscarCliente(tipoHoja *aux, int numCta);
tipoHoja historialCliente(tipoHoja *aux, int numCta);
void atenderCaja(nodoD **caja, tipoHoja **arbol, int cantidad, char comida[]);
void borrarCliente(nodoD **terminal);
// Funciones ----------------------------------------------------------------------------

/**
 * @brief
 * @date
 * @author
 * @param
 * @return
 * @Ejemplo
 */

extern void ocultarSearch(GtkWidget *button, gpointer estructura)
{
  inter *pt = (inter *)estructura;

  gtk_widget_hide_all(pt->windowSearch);

  return;
}

extern void closeTheApp(GtkWidget *window)
{
  printf("\nPrograma Finalizado\n");
  gtk_main_quit();
  return;
}

// Función que avanza a la siguiente terminal
extern void avanzarTerminal(GtkWidget *button, gpointer estructura)
{
  inter *pt = (inter *)estructura;
  char auxString[100];

  // Si la terminal actual no es la última, avanza a la siguiente
  if (pt->terminalActual->next != NULL)
  {
    pt->terminalActual = pt->terminalActual->next;

    // Actualiza la etiqueta de la terminal
    sprintf(auxString, "Terminal %s, %d clientes, $ %.2f", pt->terminalActual->terminal, pt->terminalActual->clientes, pt->terminalActual->montoAcumulado);
    gtk_label_set_text(GTK_LABEL(pt->terminalInfoLbl), auxString);

    // Actualiza la fila de clientes donde se encuentra el usuario
    pt->clienteActual = pt->terminalActual->primero;

    // Si la fila está vacía, no hay clientes
    if (pt->clienteActual == NULL)
    {
      gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), "No hay clientes en la fila");

      // Actualiza la imagen del cliente
      sprintf(auxString, "imagenes/exito.jpg");
      gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);
    }
    else
    {
      // Actualiza la etiqueta del cliente
      sprintf(auxString, "Cuenta: %d, Nombre: %s, Monedero: %.2f", pt->clienteActual->numCuenta, pt->clienteActual->nombre, pt->clienteActual->monedero);
      gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), auxString);

      // Actualiza imagen del cliente
      sprintf(auxString, "imagenes/%s.jpg", pt->clienteActual->nombre);
      gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);
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
  char auxString[100];

  // Si la terminal actual no es la primera, retrocede a la anterior
  if (pt->terminalActual->back != NULL)
  {
    pt->terminalActual = pt->terminalActual->back;

    // Actualiza la etiqueta de la terminal
    sprintf(auxString, "Terminal %s, %d clientes, $ %.2f", pt->terminalActual->terminal, pt->terminalActual->clientes, pt->terminalActual->montoAcumulado);
    gtk_label_set_text(GTK_LABEL(pt->terminalInfoLbl), auxString);

    // Actualiza la fila de clientes donde se encuentra el usuario
    pt->clienteActual = pt->terminalActual->primero;

    // Si la fila está vacía, no hay clientes
    if (pt->clienteActual == NULL)
    {
      gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), "No hay clientes en la fila");

      // Actualiza la imagen del cliente
      sprintf(auxString, "imagenes/exito.jpg");
      gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);
    }
    else
    {
      // Actualiza la etiqueta del cliente
      sprintf(auxString, "Cuenta: %d, Nombre: %s, Monedero: %.2f", pt->clienteActual->numCuenta, pt->clienteActual->nombre, pt->clienteActual->monedero);
      gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), auxString);

      // Actualiza imagen del cliente
      sprintf(auxString, "imagenes/%s.jpg", pt->clienteActual->nombre);
      gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);
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
  char auxString[100];

  // Si la fila de clientes no está vacía
  if (pt->clienteActual != NULL)
  {
    // Si el cliente actual no es el último, avanza al siguiente
    if (pt->clienteActual->next != NULL)
    {
      // Avanza al siguiente cliente
      pt->clienteActual = pt->clienteActual->next;

      // Actualiza la etiqueta del cliente
      sprintf(auxString, "Cuenta: %d, Nombre: %s, Monedero: %.2f", pt->clienteActual->numCuenta, pt->clienteActual->nombre, pt->clienteActual->monedero);
      gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), auxString);

      // Actualiza imagen del cliente
      sprintf(auxString, "imagenes/%s.jpg", pt->clienteActual->nombre);
      gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);

      // Notifica que se avanzó al siguiente cliente
      gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Avanzaste al siguiente cliente");
    }
    else
    {
      // Si el cliente actual es el último, regresa al inicio
      pt->clienteActual = pt->terminalActual->primero;

      // Actualiza la etiqueta del cliente
      sprintf(auxString, "Cuenta: %d, Nombre: %s, Monedero: %.2f", pt->clienteActual->numCuenta, pt->clienteActual->nombre, pt->clienteActual->monedero);
      gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), auxString);

      // Actualiza imagen del cliente
      sprintf(auxString, "imagenes/%s.jpg", pt->clienteActual->nombre);
      gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);

      // Si el cliente actual es el último, notifica que no hay más clientes en la fila y regresa al inicio
      gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "No hay más clientes en la fila, iniciando de nuevo");
    }
  }

  return;
}

// Función que busca un cliente en el árbol binario usando el número de cuenta
extern void buscarHistorial(GtkWidget *button, gpointer estructura)
{
  inter *pt = (inter *)estructura;
  char auxString[100];
  int numCta;
  tipoHoja cliente;

  // Obtiene el número de cuenta del entry
  sscanf(gtk_entry_get_text(GTK_ENTRY(pt->buscarEntry)), "%d", &numCta);

  printf(YELLOW "\nBuscando cliente con número de cuenta: %d\n" RESET, numCta);
  // Notifica que se está buscando el cliente
  gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Buscando cliente...");

  // Busca el cliente en el árbol binario
  if (buscarCliente(pt->raiz, numCta) == 0)
  {
    // Notifica que no se encontró el cliente
    gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Cliente no encontrado");
  }
  else
  {
    // Si se encontró el cliente, obtiene su historial
    cliente = historialCliente(pt->raiz, numCta);
    // Actualiza la etiqueta del cliente
    sprintf(auxString, "Cuenta: %d, Nombre: %s", cliente.numCuenta, cliente.nombre);
    gtk_label_set_text(GTK_LABEL(pt->searchInfoLbl), auxString);
    sprintf(auxString, "Tacos: %d, Pizzas: %d", cliente.numTacos, cliente.numPizzas);
    gtk_label_set_text(GTK_LABEL(pt->searchInfoLbl2), auxString);
    sprintf(auxString, "Compra acumulada: $%.2f", cliente.compraAcumulada);
    gtk_label_set_text(GTK_LABEL(pt->searchInfoLbl3), auxString);

    // Actualiza la imagen del cliente
    // La imagen se encuentra en la carpeta "imagenes" y se llama "nombre.jpg"
    sprintf(auxString, "imagenes/%s.jpg", cliente.nombre);
    gtk_image_set_from_file(GTK_IMAGE(pt->searchImg), auxString);

    gtk_widget_show_all(pt->windowSearch);

    // Notifica que se encontró el cliente
    gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Cliente encontrado");
  }

  return;
}

extern void atenderCajaGTK(GtkWidget *button, gpointer estructura)
{
  inter *pt = (inter *)estructura;

  char auxString[100];

  char comida[20];
  int cantidad;
  // Si la caja está vacía
  if (pt->terminalActual->primero == NULL)
  {
    // Notifica que no hay clientes en la fila
    gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "No hay clientes en la fila");
  }
  else // Si la caja no está vacía
  {
    // Obtiene la comida y la cantidad del entry
    sscanf(gtk_entry_get_text(GTK_ENTRY(pt->atencionEntry)), "%s", comida);
    sscanf(gtk_entry_get_text(GTK_ENTRY(pt->atencionEntry2)), "%d", &cantidad);

    // Si la comida es invalida
    if ((strcmp(comida, "Pizzas") != 0) && (strcmp(comida, "Tacos") != 0))
    {
      // Notifica que la comida es inválida
      gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Comida inválida");
    }
    // Si la cantidad es inválida
    else if (cantidad <= 0)
    {
      // Notifica que la cantidad es inválida
      gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Cantidad inválida");
    }
    else
    {
      atenderCaja(&pt->terminalActual, &pt->raiz, cantidad, comida);
      borrarCliente(&pt->terminalActual);

      pt->clienteActual = pt->clienteActual->next;

      // Actualiza la etiqueta del cliente
      sprintf(auxString, "Cuenta: %d, Nombre: %s, Monedero: %.2f", pt->clienteActual->numCuenta, pt->clienteActual->nombre, pt->clienteActual->monedero);
      gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), auxString);

      // Actualiza imagen del cliente
      sprintf(auxString, "imagenes/%s.jpg", pt->clienteActual->nombre);
      gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);

      // Notifica que se avanzó al siguiente cliente
      gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Caja Atendida");

      printf(GREEN "\nCaja atendida\n" RESET);

      // Reiniciar entries
      gtk_entry_set_text(GTK_ENTRY(pt->atencionEntry), "Pizzas / Tacos");
      gtk_entry_set_text(GTK_ENTRY(pt->atencionEntry2), "Cantidad");
      // Oculta la ventana de atención
      gtk_widget_hide_all(pt->windowAtencion);
    }
  }
}
// Función para atender terminales
extern void atender(GtkWidget *button, gpointer estructura)
{
  inter *pt = (inter *)estructura;

  char auxString[100];

  // Si la terminal actual es caja
  if (strcmp(pt->terminalActual->terminal, "Caja") == 0)
  {
    if (pt->terminalActual->primero != NULL) // Si la fila de clientes no está vacía
    {
      gtk_widget_show_all(pt->windowAtencion);

      // Notifica que se está atendiendo la caja
      gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Atendiendo caja");
    }
    else // Si la fila de clientes está vacía
    {
      // Oculta la ventana de atención
      gtk_widget_hide_all(pt->windowAtencion);
      // Notifica que no hay clientes en la fila
      gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "No hay clientes en la fila");
    }
  }
  // Si la terminal actual es de comida
  else if ((strcmp(pt->terminalActual->terminal, "Pizzas") == 0) || (strcmp(pt->terminalActual->terminal, "Tacos") == 0))
  {
    // Si la fila de clientes está vacía
    if (pt->terminalActual->primero == NULL)
    {
      // Notifica que no hay clientes en la fila
      gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "No hay clientes en la fila");
    }
    else // Si la fila de clientes no está vacía
    {

      // Atender el cliente
      pt->clienteActual = pt->clienteActual->next;
      borrarCliente(&pt->terminalActual);

      // Si la fila de clientes está vacía
      if (pt->clienteActual == NULL)
      {
        // Actualiza la etiqueta del cliente
        gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), "No hay clientes en la fila");

        // Actualiza la imagen del cliente
        sprintf(auxString, "imagenes/exito.jpg");
        gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);
      }
      else
      {
        // Actualiza la etiqueta del cliente
        sprintf(auxString, "Cuenta: %d, Nombre: %s, Monedero: %.2f", pt->clienteActual->numCuenta, pt->clienteActual->nombre, pt->clienteActual->monedero);
        gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), auxString);

        // Actualiza imagen del cliente
        sprintf(auxString, "imagenes/%s.jpg", pt->clienteActual->nombre);
        gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);

        // Notifica que se avanzó al siguiente cliente
        gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Comida Atendida");
      }
    }
  }
  // Si la terminal actual es de facturación

  return;
}
