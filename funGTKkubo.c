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
int verificarColas(nodoD *aux);
void crearArchivoFacturas(char nombreArchivo[], nodoD *aux);
void crearArchivoClientes(char nombreArchivo[], nodoD *aux);
void guardarArbol(tipoHoja *aux, char nombreArchivo[]);
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

extern gboolean closeTheApp(GtkWidget *window, GdkEvent *event, gpointer estructura)
{
  inter *pt = (inter *)estructura;

  gboolean salir = FALSE;

  // Para el nombre del archivo de facturas y el comando para moverlo
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  char nombreArchivo[50];
  char comando[120];

  // No se puede cerrar el programa porque hay clientes en cola
  if (verificarColas(pt->inicio) == 1)
  {
    gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "No puedes cerrar el programa, hay clientes en cola");
    salir = TRUE;
  }
  else
  {
    // Cuando se sale del programa, se almacenan los datos de las facturas en un archivo
    // Se crea el archivo de facturas, el nombre debe tener la fecha y hora actual
    sprintf(nombreArchivo, "facturas_%04d-%02d-%02d_%02d-%02d-%02d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    crearArchivoFacturas(nombreArchivo, pt->inicio);

    // Crear carpeta para almacenar el archivo de facturas si no existe
    if (system("mkdir -p facturas") == -1)
    {
      printf(RED "\n\n\tError al crear la carpeta de facturas\n\n" RESET);
      exit(1);
    }
    // Mover el archivo de facturas a la carpeta de facturas
    sprintf(comando, "mv %s facturas/%s", nombreArchivo, nombreArchivo);
    if (system(comando) == -1)
    {
      printf(RED "\n\n\tError al mover el archivo de facturas\n\n" RESET);
      exit(1);
    }
    else
    {
      printf(GREEN "\n\nArchivo de facturas creado con éxito\n\n" RESET);
      printf(GREEN "El archivo de facturas se ha creado con el nombre " YELLOW "%s" RESET, nombreArchivo);
    }

    // Se crea el archivo de clientes restantes
    crearArchivoClientes("clientes.txt", pt->inicio);
    printf(GREEN "\n\nLos clientes restantes se han almacenado en \"clientes.txt\"\n\n" RESET);

    // Se guarda el árbol binario en un archivo
    guardarArbol(pt->raiz, pt->nombreArchivoArbol);
    printf(GREEN "\n\nEl árbol binario se ha guardado en \"%s\"\n\n" RESET, pt->nombreArchivoArbol);

    // Finalizar el programa
    printf("\nPrograma Finalizado\n");
    salir = FALSE;
    gtk_main_quit();
  }

  return salir;
}

// Función que avanza a la siguiente terminal
extern void avanzarTerminal(GtkWidget *button, gpointer estructura)
{
  inter *pt = (inter *)estructura;
  char auxString[120];

  // Si la terminal actual no es la última, avanza a la siguiente
  if (pt->terminalActual->next != NULL)
  {
    pt->terminalActual = pt->terminalActual->next;

    // Actualiza la etiqueta de la terminal
    sprintf(auxString, "Terminal %s, %d clientes, $ %.2f", pt->terminalActual->terminal, pt->terminalActual->clientes, pt->terminalActual->montoAcumulado);
    gtk_label_set_text(GTK_LABEL(pt->terminalInfoLbl), auxString);

    // Si la terminal es facturación
    if (strcmp(pt->terminalActual->terminal, "Facturacion") == 0)
    {
      pt->facturaActual = pt->terminalActual->top;
      // Si la pila de facturas está vacía, no hay clientes
      if (pt->facturaActual == NULL)
      {
        gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), "No hay clientes en la fila");

        // Actualiza la imagen del cliente
        sprintf(auxString, "imagenes/exito.jpg");
        gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);
      }
      else
      {
        // Actualiza la etiqueta del cliente
        sprintf(auxString, "Factura: %d, Nombre: %s, Compra: %s, Total: %.2f", pt->facturaActual->numFactura, pt->facturaActual->nombre, pt->facturaActual->compra, pt->facturaActual->totalFacturado);
        gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), auxString);

        // Actualiza imagen del cliente
        sprintf(auxString, "imagenes/%s.jpg", pt->facturaActual->nombre);
        gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);
      }
    }
    else
    {
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
  char auxString[120];

  // Si la terminal actual no es la última, avanza a la siguiente
  if (pt->terminalActual->back != NULL)
  {
    pt->terminalActual = pt->terminalActual->back;

    // Actualiza la etiqueta de la terminal
    sprintf(auxString, "Terminal %s, %d clientes, $ %.2f", pt->terminalActual->terminal, pt->terminalActual->clientes, pt->terminalActual->montoAcumulado);
    gtk_label_set_text(GTK_LABEL(pt->terminalInfoLbl), auxString);

    // Si la terminal es facturación
    if (strcmp(pt->terminalActual->terminal, "Facturacion") == 0)
    {
      pt->facturaActual = pt->terminalActual->top;
      // Si la pila de facturas está vacía, no hay clientes
      if (pt->facturaActual == NULL)
      {
        gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), "No hay clientes en la fila");

        // Actualiza la imagen del cliente
        sprintf(auxString, "imagenes/exito.jpg");
        gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);
      }
      else
      {
        // Actualiza la etiqueta del cliente
        sprintf(auxString, "Factura: %d, Nombre: %s, Compra: %s, Total: %.2f", pt->facturaActual->numFactura, pt->facturaActual->nombre, pt->facturaActual->compra, pt->facturaActual->totalFacturado);
        gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), auxString);

        // Actualiza imagen del cliente
        sprintf(auxString, "imagenes/%s.jpg", pt->facturaActual->nombre);
        gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);
      }
    }
    else
    {
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
    }
    // Notifica que se avanzó a la siguiente terminal
    gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Avanzaste a la siguiente terminal");
  }

  return;
}

// Función que avanza al siguiente cliente
extern void avanzarCliente(GtkWidget *button, gpointer estructura)
{
  inter *pt = (inter *)estructura;
  char auxString[120];

  // Si la terminal actual es facturación
  if (strcmp(pt->terminalActual->terminal, "Facturacion") == 0)
  {
    // Si la pila de facturas está vacía, no hay clientes
    if (pt->facturaActual == NULL)
    {
      gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), "No hay clientes en la fila");

      // Actualiza la imagen del cliente
      sprintf(auxString, "imagenes/exito.jpg");
      gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);
    }
    else
    {
      // Si no es la última factura, avanza la siguiente factura
      if (pt->facturaActual->next != NULL)
      {
        // Si la pila de facturas no está vacía, avanza al siguiente cliente
        pt->facturaActual = pt->facturaActual->next;

        // Actualiza la etiqueta del cliente
        sprintf(auxString, "Factura: %d, Nombre: %s, Compra: %s, Total: %.2f", pt->facturaActual->numFactura, pt->facturaActual->nombre, pt->facturaActual->compra, pt->facturaActual->totalFacturado);
        gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), auxString);

        // Actualiza imagen del cliente
        sprintf(auxString, "imagenes/%s.jpg", pt->facturaActual->nombre);
        gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);

        // Notifica que se avanzó al siguiente cliente
        gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Avanzaste a la siguiente factura");
      }
      else
      {
        // Si la factura actual es la última, regresa al inicio
        pt->facturaActual = pt->terminalActual->top;

        // Actualiza la etiqueta del cliente
        sprintf(auxString, "Factura: %d, Nombre: %s, Compra: %s, Total: %.2f", pt->facturaActual->numFactura, pt->facturaActual->nombre, pt->facturaActual->compra, pt->facturaActual->totalFacturado);
        gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), auxString);

        // Actualiza imagen del cliente
        sprintf(auxString, "imagenes/%s.jpg", pt->facturaActual->nombre);
        gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);

        // Si el cliente actual es el último, notifica que no hay más clientes en la fila y regresa al inicio
        gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "No hay más facturas en la pila, iniciando de nuevo");
      }
    }
  }
  else
  {
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
      // Si el cliente actual es NULL
      else if (pt->clienteActual == NULL)
      {
        // Actualiza la etiqueta del cliente
        gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), "No hay clientes en la fila");

        // Actualiza la imagen del cliente
        sprintf(auxString, "imagenes/exito.jpg");
        gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);
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
    else
    {
      // Si la fila de clientes está vacía, no hay clientes
      gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), "No hay clientes en la fila");

      // Actualiza la imagen del cliente
      sprintf(auxString, "imagenes/exito.jpg");
      gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);

      // Actualiza la etiqueta de la terminal
      sprintf(auxString, "Terminal %s, %d clientes, $ %.2f", pt->terminalActual->terminal, pt->terminalActual->clientes, pt->terminalActual->montoAcumulado);
      gtk_label_set_text(GTK_LABEL(pt->terminalInfoLbl), auxString);
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
    if ((strcmp(gtk_entry_get_text(GTK_ENTRY(pt->atencionEntry)), "Tacos") == 0) || (strcmp(gtk_entry_get_text(GTK_ENTRY(pt->atencionEntry)), "Pizzas") == 0)) // El Entry 1 debe de ser Tacos o Pizzas
    {

      // Obtiene la comida y la cantidad del entry
      sscanf(gtk_entry_get_text(GTK_ENTRY(pt->atencionEntry)), "%s", comida);
      sscanf(gtk_entry_get_text(GTK_ENTRY(pt->atencionEntry2)), "%d", &cantidad);

      if (cantidad <= 0)
      {
        // Notifica que la cantidad es inválida
        gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Cantidad inválida");
      }
      else
      {
        // Si no la entrada de cantudad está vacía
        if (strcmp(gtk_entry_get_text(GTK_ENTRY(pt->atencionEntry2)), "") == 0)
        {
          // Notifica que la cantidad es inválida
          gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Cantidad inválida");
        }
        else
        {
          // Un cliente en la caja
          if (pt->terminalActual->primero == pt->terminalActual->ultimo)
          {
            // Atender la caja
            atenderCaja(&pt->terminalActual, &pt->raiz, cantidad, comida);
            borrarCliente(&pt->terminalActual);

            pt->clienteActual = NULL;

            // Actualiza la etiqueta del cliente notificando que no hay más clientes
            gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), "No hay clientes en la fila");
            // Actualiza la imagen del cliente
            sprintf(auxString, "imagenes/exito.jpg");
            gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);
            // Actualiza la etiqueta de la terminal
            sprintf(auxString, "Terminal %s, %d clientes, $ %.2f", pt->terminalActual->terminal, pt->terminalActual->clientes, pt->terminalActual->montoAcumulado);
            gtk_label_set_text(GTK_LABEL(pt->terminalInfoLbl), auxString);
          }
          // Si hay más de un cliente
          else
          {
            // En caso de que el cliente actual sea el atendido
            if (pt->clienteActual == pt->terminalActual->primero)
            {
              pt->clienteActual = pt->clienteActual->next;
            }

            // Atender la caja
            atenderCaja(&pt->terminalActual, &pt->raiz, cantidad, comida);
            borrarCliente(&pt->terminalActual);

            // Actualiza la etiqueta del cliente
            sprintf(auxString, "Cuenta: %d, Nombre: %s, Monedero: %.2f", pt->clienteActual->numCuenta, pt->clienteActual->nombre, pt->clienteActual->monedero);
            gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), auxString);

            // Actualiza imagen del cliente
            sprintf(auxString, "imagenes/%s.jpg", pt->clienteActual->nombre);
            gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);

            // Actualiza la etiqueta de la terminal
            sprintf(auxString, "Terminal %s, %d clientes, $ %.2f", pt->terminalActual->terminal, pt->terminalActual->clientes, pt->terminalActual->montoAcumulado);
            gtk_label_set_text(GTK_LABEL(pt->terminalInfoLbl), auxString);
          }

          // Notifica que se atendió la caja
          gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Caja atendida");
        }
      }
    }
    else // Opción inválida
    {
      // Notifica que la opción es inválida
      gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Comida inválida");
    }
  }
  // Oculta la ventana de atención
  gtk_widget_hide_all(pt->windowAtencion);

  // Reinicia los entries
  gtk_entry_set_text(GTK_ENTRY(pt->atencionEntry), "Pizzas / Tacos");
  gtk_entry_set_text(GTK_ENTRY(pt->atencionEntry2), "");
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

      // Actualiza la imagen del cliente
      sprintf(auxString, "imagenes/exito.jpg");
      gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);
    }
    else // Si la fila de clientes no está vacía
    {

      // Si la fila de clientes está vacía
      if ((pt->terminalActual->primero == NULL) && (pt->terminalActual->ultimo == NULL))
      {
        // Actualiza la etiqueta del cliente
        borrarCliente(&pt->terminalActual);
        pt->clienteActual = NULL;

        gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), "No hay clientes en la fila");

        // Actualiza la etiqueta de la terminal
        sprintf(auxString, "Terminal %s, %d clientes, $ %.2f", pt->terminalActual->terminal, pt->terminalActual->clientes, pt->terminalActual->montoAcumulado);
        gtk_label_set_text(GTK_LABEL(pt->terminalInfoLbl), auxString);

        // Actualiza la imagen del cliente
        sprintf(auxString, "imagenes/exito.jpg");
        gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);

        // Notifica que atendió la comida
        gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Terminal atendida");
      }
      else
      {
        // Si hay un solo cliente
        if (pt->terminalActual->primero == pt->terminalActual->ultimo)
        {
          borrarCliente(&pt->terminalActual);
          pt->clienteActual = NULL;

          // Actualiza la etiqueta del cliente
          gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), "No hay clientes en la fila");

          // Actualiza imagen del cliente
          gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), "imagenes/exito.jpg");

          // Notifica que se atendió la comida
          gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Comida atendida");

          // Actualiza la etiqueta de la terminal
          sprintf(auxString, "Terminal %s, %d clientes, $ %.2f", pt->terminalActual->terminal, pt->terminalActual->clientes, pt->terminalActual->montoAcumulado);
          gtk_label_set_text(GTK_LABEL(pt->terminalInfoLbl), auxString);
        }
        else
        {
          // Si el cliente actual es el atendido
          if (pt->clienteActual == pt->terminalActual->primero)
          {
            pt->clienteActual = pt->clienteActual->next;
          }

          borrarCliente(&pt->terminalActual);

          // Actualiza la etiqueta del cliente
          sprintf(auxString, "Cuenta: %d, Nombre: %s, Monedero: %.2f", pt->clienteActual->numCuenta, pt->clienteActual->nombre, pt->clienteActual->monedero);
          gtk_label_set_text(GTK_LABEL(pt->clienteInfoLbl), auxString);

          // Actualiza imagen del cliente
          sprintf(auxString, "imagenes/%s.jpg", pt->clienteActual->nombre);
          gtk_image_set_from_file(GTK_IMAGE(pt->clienteImg), auxString);

          // Actualiza la etiqueta de la terminal
          sprintf(auxString, "Terminal %s, %d clientes, $ %.2f", pt->terminalActual->terminal, pt->terminalActual->clientes, pt->terminalActual->montoAcumulado);
          gtk_label_set_text(GTK_LABEL(pt->terminalInfoLbl), auxString);
        }

        // Notifica que se avanzó al siguiente cliente
        gtk_label_set_text(GTK_LABEL(pt->notificacionesLbl), "Comida Atendida");
      }
    }
  }

  return;
}

// Función que evita la destrucción de widgets
gboolean on_window_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
  gtk_widget_hide(widget); // Oculta la ventana
  return TRUE;             // Evita que la ventana se destruya
}