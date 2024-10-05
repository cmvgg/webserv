                    [Iniciar Servidor]
                           |
           [Cargar archivo de configuración]
                            |
            [¿Archivo de configuración existe y es valido?]
                     /              \
                    No               Sí
                    |                 |
[ cargar conf default e ir]    [Leer configuraciones]
                        |             |
                  [¿Configuración válida?]
                    /                 \
                   No                   Sí
                   |                     |
  [Mostrar mensaje de error]    [Leer puerto de escucha]
                   |                     |
       [Finalizar programa]       [¿Puerto de escucha válido?]
                    /                  \
                  No                    Sí
                  |                      |
       [Mostrar mensaje de error]  [Crear socket]
                 |                      |
     [Finalizar programa]        [¿Socket creado?]
                                   /      \
                                No        Sí
                                |          |
        [Mostrar mensaje de error]  [Establecer opciones del socket]
                      |                   |
      [Finalizar programa]       [Configurar tiempo de espera]
                                        |
                         [Configurar tamaño de buffer]
                                         |
                  [Configurar comportamiento de cierre]
                                      |
               [Establecer opciones de reuso de socket]
                              |
                    [Hacer bind del socket]
                              |
                     [¿Bind exitoso?]
                         /      \
                       No        Sí
                       |          |
  [Mostrar mensaje de error]  [Escuchar conexiones]
              |                   |
  [Finalizar programa]     [Esperar conexión de cliente]
                                    |
                          [Esperar en poll()]
                         /       \
                        No         Sí
                        |           |
        [Esperar en poll()]  [Aceptar conexión]
                                    |
                  [Crear nuevo socket para la conexión]
                                    |
                       [Leer solicitud del cliente]
                                    |
                          [¿Solicitud válida?]
                            /           \
                         No             Sí
                         |               |
         [Enviar respuesta 400]   [Extraer método HTTP]
                       |                     |
       [Fin de la respuesta]  [_________¿Método HTTP?____________________]
                              /   |                        \    \      \
                          GET  POST                         DELETE OPTIONS PATCH
                         |     |                               |      |      |
[Extraer ruta del archivo] [Extraer datos del cuerpo] [Eliminar archivo]   [¿Método PATCH?]
  |                          |                                             /          \
[¿Archivo existe?]     [¿Datos de tamaño válido?]                         No          Sí
  /              \               /        \                               |            |
 No              Sí           No          Sí                            [Fin] [Validar actualización parcial]
  |                |           |            |                                  |
[Enviar 404] [Enviar 413] [Enviar 200] [¿Archivo enviado?]                    [¿Archivo existe?]
                                        /            \                        /      \
                                       No            Sí                      No       Sí
                                       |              |                      |         |
                                [Enviar 500]  [Enviar 200]         [Enviar 404] [Actualizar archivo]
                                 |                                               |
                [Retornar a cliente]                       [¿Actualización exitosa?]
                                                           /        \
                                                          No         Sí
                                                         |          |
                                                  [Enviar 500] [Enviar 200 o 204]
                                                                    |
                                                              [Retornar a cliente]
                                                                    |
                                                            [Cerrar conexión]
                                                                 |
                                                       [Volver a esperar conexión]

