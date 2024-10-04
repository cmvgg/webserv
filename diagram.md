                                  [Iniciar Servidor]
                                         |
                          [Cargar archivo de configuración]
                                         |
                          [¿Archivo de configuración existe?]
                                    /             \
                                  No               Sí
                                  |                 |
                      [Mostrar mensaje de error]    [Leer configuraciones]
                                  |                 |
                      [Finalizar programa]      [¿Configuración válida?]
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
                                                      [¿Conexión recibida?]
                                                             /       \
                                                           No         Sí
                                                           |           |
                                          [Esperar en poll() o select()]  [Aceptar conexión]
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
                                                              |               |
                                                   [Fin de la respuesta]      |
                                                                           [¿Método HTTP?]
                                                                               /   |    \
                                                                            GET  POST DELETE OPTIONS PATCH
                                                                            |     |      |      |      |
                                                        [Extraer ruta del archivo] [Extraer datos del cuerpo]
                                                                            |      |
                                                          [¿Archivo existe?]     [¿Datos de tamaño válido?]
                                                                   /    \               /     \
                                                                 No      Sí           No      Sí
                                                                 |        |           |       |
                                                      [Enviar respuesta 404]   [Enviar respuesta 413]
                                                                 |        |           |       |
                                                      [Fin de la respuesta]     [Fin de la respuesta]
                                                                 |        |
                                                      [Enviar respuesta 200] 
                                                                 |        
                                                      [¿Archivo enviado correctamente?]
                                                                 /     \
                                                               No       Sí
                                                               |         |
                                                   [Enviar respuesta 500]  [Enviar respuesta 200]
                                                               |         |
                                                   [Fin de la respuesta]   [Retornar a cliente]
                                                                 |
                                                       [¿Método DELETE?]
                                                                 /    \
                                                               No      Sí
                                                               |        |
                                                       [¿Método PATCH?]   [Eliminar archivo]
                                                                /     \
                                                              No       Sí
                                                              |         |
                                                      [Fin de la respuesta]   [Validar actualización parcial]
                                                                               |
                                                                 [¿Archivo existe?]
                                                                            /     \
                                                                          No       Sí
                                                                          |         |
                                                              [Enviar respuesta 404]  [Actualizar archivo]
                                                                          |         |
                                                              [Fin de la respuesta] [¿Actualización exitosa?]
                                                                                     /     \
                                                                                   No       Sí
                                                                                   |         |
                                                                   [Enviar respuesta 500] [Enviar respuesta 200 o 204]
                                                                                   |         |
                                                                   [Fin de la respuesta]   [Retornar a cliente]
                                                                                   |
                                                                            [Cerrar conexión]
                                                                                   |
                                                                       [Volver a esperar conexión]
