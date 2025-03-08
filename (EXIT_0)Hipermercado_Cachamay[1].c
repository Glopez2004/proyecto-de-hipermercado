#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ========================= Definiciones =========================

// Definiciones de limites del sistema
#define MAX_PRODUCTOS 100
#define MAX_CLIENTES 100
#define MAX_TRABAJADORES 20
#define MAX_CAJAS 5
#define MAX_FACTURAS 100
#define MAX_PRODUCTOS_EN_FACTURA 10

// Tasa de cambio fija para demostracion (a mejorar con tasa del dia real)
#define TASA_USD 35.0
#define TASA_COP 0.01
#define TASA_EUR 40.0

// Nombre del hipermercado y datos fiscales
#define NOMBRE_HIPERMERCADO "Hipermercado Cachamay"
#define RIF_HIPERMERCADO "J-123456789"
#define TELEFONO_HIPERMERCADO "0414-0463588"

// Tasa de IVA Actualizada al 16% (Solicitado por el usuario)
#define IVA_PORCENTAJE 16.0

// Estructuras de datos (definicion de registros)
typedef struct {
    int codigo;
    char nombre[50];
    int cantidad_disponible;
    float precio_bs;
    float precio_usd;
    float precio_cop;
    float precio_eur;
} Producto;

typedef struct {
    int cedula;
    char nombre[50];
    char apellido[50];
    char telefono[15];
    char direccion[100];
} Cliente;

typedef struct {
    int numero_empleado;
    char nombre[50];
    char apellido[50];
    int cedula;
    char telefono[15];
    char direccion[100];
} Trabajador;

typedef struct {
    int numero_caja;
    int numero_empleado; // Empleado responsable
    char nombre_empleado[50];
    float total_bs_facturado;
    float total_usd_facturado;
    float total_cop_facturado;
    float total_eur_facturado;
} Caja;

typedef struct {
    int numero_factura;
    time_t fecha;
    Cliente cliente;
    int numero_caja; // Caja asociada a la factura
    int numero_empleado_caja; // Empleado responsable de la caja en la factura
    struct ProductoFactura {
        Producto producto;
        int cantidad;
    } productos_factura[MAX_PRODUCTOS_EN_FACTURA];
    int cantidad_productos_factura;
    float monto_total;
} Factura;

// ========================= Variables Globales =========================

// Arrays para almacenar los datos - Inicialmente en memoria, mejora futura: persistencia en archivos
Producto productos[MAX_PRODUCTOS];
Cliente clientes[MAX_CLIENTES];
Trabajador trabajadores[MAX_TRABAJADORES];
Caja cajas[MAX_CAJAS];
Factura facturas[MAX_FACTURAS];

int cantidad_productos = 0;
int cantidad_clientes = 0;
int cantidad_trabajadores = 0;
int cantidad_cajas = 0;
int cantidad_facturas = 0;
int numero_factura_actual = 1; // Inicia en 1 y aumenta secuencialmente

// ========================= Prototipos de Funciones =========================

// Prototipos de funciones de modulos principales
void mostrar_menu_principal();
void modulo_productos();
void modulo_clientes();
void modulo_facturas();
void modulo_ventas();
void modulo_cajas();
void modulo_trabajadores();
void mostrar_ayuda_usuario(); // Prototipo para la ventana de ayuda

// Prototipos de funciones de modulo Productos
void crear_producto();
void modificar_producto();
void eliminar_producto();
void mostrar_producto();
void mostrar_todos_productos();

// Prototipos de funciones de modulo Clientes
void crear_cliente();
void modificar_cliente();
void eliminar_cliente();
void mostrar_cliente();
void mostrar_todos_clientes();

// Prototipos de funciones de modulo Trabajadores
void crear_trabajador();
void modificar_trabajador();
void eliminar_trabajador();
void mostrar_trabajador();
void mostrar_todos_trabajadores();

// Prototipos de funciones de modulo Cajas
void crear_caja();
void modificar_caja();
void eliminar_caja();
void mostrar_caja();
void mostrar_todas_cajas();

// Prototipos de funciones de modulo Facturas
void crear_factura();
void mostrar_factura();
void mostrar_ultimas_facturas();
void mostrar_factura_detalle(Factura *factura); // Prototipo funcion auxiliar mostrar detalle factura

// Prototipos de funciones de modulo Ventas y Reportes
void consultar_ventas_diarias_caja();
void consultar_ventas_mensuales_caja();
void mostrar_total_ventas_diarias();
void mostrar_total_ventas_mensuales();
void mostrar_producto_mas_vendido();
void mostrar_cantidad_trabajadores_hipermercado();
void mostrar_cantidad_clientes_con_compras();

// Prototipos de funciones auxiliares
int obtener_entero(const char *mensaje);
float obtener_flotante(const char *mensaje);
void obtener_cadena(const char *mensaje, char *cadena, int longitud_maxima);
void presionar_enter_para_continuar();
int buscar_producto_por_codigo(int codigo);
int buscar_cliente_por_cedula(int cedula);
int buscar_trabajador_por_numero_empleado(int numero_empleado);
int buscar_caja_por_numero(int numero_caja);
int buscar_factura_por_numero(int numero_factura);
void calcular_precios_monedas(Producto *producto);
void limpiar_consola();
void guardar_reporte_txt(const char *nombre_archivo, const char *contenido); // Prototipo para guardar reportes en archivos TXT
char* obtener_fecha_hora_actual(); // Prototipo para obtener fecha y hora actual como cadena

// ========================= Funcion Principal =========================

int main() {
    mostrar_menu_principal(); // Inicia mostrando el menu principal
    return 0; // Finaliza el programa
}

// ========================= Funciones de Menu Principal y Modulos =========================

void mostrar_menu_principal() {
    int opcion;
    do {
        limpiar_consola(); // Limpia la consola en cada iteracion del menu
        printf("=========================================================\n");
        printf("              %s - MENU PRINCIPAL     \n", NOMBRE_HIPERMERCADO);
        printf("=========================================================\n");
        printf(" RIF: %s - Telefono: %s               \n", RIF_HIPERMERCADO, TELEFONO_HIPERMERCADO);
        printf("=========================================================\n");
        printf("1. Modulo Productos\n");
        printf("2. Modulo Clientes\n");
        printf("3. Modulo Facturas\n");
        printf("4. Modulo Ventas y Reportes\n");
        printf("5. Modulo Cajas\n");
        printf("6. Modulo Trabajadores\n");
        printf("7. Ayuda al Usuario\n");
        printf("8. Salir\n");
        printf("=========================================================\n");
        opcion = obtener_entero("Seleccione una opcion: "); // Solicita opcion al usuario

        switch (opcion) {
            case 1: modulo_productos(); break;
            case 2: modulo_clientes(); break;
            case 3: modulo_facturas(); break;
            case 4: modulo_ventas(); break;
            case 5: modulo_cajas(); break;
            case 6: modulo_trabajadores(); break;
            case 7: mostrar_ayuda_usuario(); break; // Invoca la funcion de ayuda
            case 8: printf("Saliendo del sistema. ¡Gracias por usar %s!\n", NOMBRE_HIPERMERCADO); exit(0); break; // Mensaje de salida
            default: printf("Opcion no valida. Intente de nuevo.\n"); presionar_enter_para_continuar(); break;
        }
    } while (opcion != 8); // Repite el menu hasta que se seleccione la opcion 8 (Salir)
}

void modulo_productos() {
    int opcion_producto;
    do {
        limpiar_consola();
        printf("==================== Modulo Productos ====================\n");
        printf("1. Crear Producto\n");
        printf("2. Modificar Producto\n");
        printf("3. Eliminar Producto\n");
        printf("4. Mostrar Producto\n");
        printf("5. Mostrar Todos los Productos\n");
        printf("6. Regresar al Menu Principal\n");
        printf("=========================================================\n");
        opcion_producto = obtener_entero("Seleccione una opcion: ");

        switch (opcion_producto) {
            case 1: crear_producto(); break;
            case 2: modificar_producto(); break;
            case 3: eliminar_producto(); break;
            case 4: mostrar_producto(); break;
            case 5: mostrar_todos_productos(); break;
            case 6: break; // Regresa al menu principal
            default: printf("Opcion no valida. Intente de nuevo.\n"); presionar_enter_para_continuar(); break;
        }
    } while (opcion_producto != 6);
}

void modulo_clientes() {
    int opcion_cliente;
    do {
        limpiar_consola();
        printf("==================== Modulo Clientes =====================\n");
        printf("1. Crear Cliente\n");
        printf("2. Modificar Cliente\n");
        printf("3. Eliminar Cliente\n");
        printf("4. Mostrar Cliente\n");
        printf("5. Mostrar Todos los Clientes\n");
        printf("6. Regresar al Menu Principal\n");
        printf("=========================================================\n");
        opcion_cliente = obtener_entero("Seleccione una opcion: ");

        switch (opcion_cliente) {
            case 1: crear_cliente(); break;
            case 2: modificar_cliente(); break;
            case 3: eliminar_cliente(); break;
            case 4: mostrar_cliente(); break;
            case 5: mostrar_todos_clientes(); break;
            case 6: break; // Regresa al menu principal
            default: printf("Opcion no valida. Intente de nuevo.\n"); presionar_enter_para_continuar(); break;
        }
    } while (opcion_cliente != 6);
}

void modulo_trabajadores() {
    int opcion_trabajador;
    do {
        limpiar_consola();
        printf("================== Modulo Trabajadores ===================\n");
        printf("1. Crear Trabajador\n");
        printf("2. Modificar Trabajador\n");
        printf("3. Eliminar Trabajador\n");
        printf("4. Mostrar Trabajador\n");
        printf("5. Mostrar Todos los Trabajadores\n");
        printf("6. Regresar al Menu Principal\n");
        printf("=========================================================\n");
        opcion_trabajador = obtener_entero("Seleccione una opcion: ");

        switch (opcion_trabajador) {
            case 1: crear_trabajador(); break;
            case 2: modificar_trabajador(); break;
            case 3: eliminar_trabajador(); break;
            case 4: mostrar_trabajador(); break;
            case 5: mostrar_todos_trabajadores(); break;
            case 6: break; // Regresa al menu principal
            default: printf("Opcion no valida. Intente de nuevo.\n"); presionar_enter_para_continuar(); break;
        }
    } while (opcion_trabajador != 6);
}


void modulo_cajas() {
    int opcion_caja;
    do {
        limpiar_consola();
        printf("===================== Modulo Cajas =======================\n");
        printf("1. Crear Caja\n");
        printf("2. Modificar Caja\n");
        printf("3. Eliminar Caja\n");
        printf("4. Mostrar Caja\n");
        printf("5. Mostrar Todas las Cajas\n");
        printf("6. Regresar al Menu Principal\n");
        printf("=========================================================\n");
        opcion_caja = obtener_entero("Seleccione una opcion: ");

        switch (opcion_caja) {
            case 1: crear_caja(); break;
            case 2: modificar_caja(); break;
            case 3: eliminar_caja(); break;
            case 4: mostrar_caja(); break;
            case 5: mostrar_todas_cajas(); break;
            case 6: break; // Regresa al menu principal
            default: printf("Opcion no valida. Intente de nuevo.\n"); presionar_enter_para_continuar(); break;
        }
    } while (opcion_caja != 6);
}


void modulo_facturas() {
    int opcion_factura;
    do {
        limpiar_consola();
        printf("==================== Modulo Facturas =====================\n");
        printf("1. Crear Factura\n");
        printf("2. Mostrar Factura\n");
        printf("3. Mostrar Ultimas 10 Facturas\n");
        printf("4. Regresar al Menu Principal\n");
        printf("=========================================================\n");
        opcion_factura = obtener_entero("Seleccione una opcion: ");

        switch (opcion_factura) {
            case 1: crear_factura(); break;
            case 2: mostrar_factura(); break;
            case 3: mostrar_ultimas_facturas(); break;
            case 4: break; // Regresa al menu principal
            default: printf("Opcion no valida. Intente de nuevo.\n"); presionar_enter_para_continuar(); break;
        }
    } while (opcion_factura != 4);
}


void modulo_ventas() {
    int opcion_ventas;
    do {
        limpiar_consola();
        printf("================== Modulo Ventas y Reportes =================\n");
        printf("1. Consultar Ventas Diarias por Caja\n");
        printf("2. Consultar Ventas Mensuales por Caja\n");
        printf("3. Mostrar Total Ventas Diarias\n");
        printf("4. Mostrar Total Ventas Mensuales\n");
        printf("5. Mostrar Producto Mas Vendido\n");
        printf("6. Mostrar Cantidad de Trabajadores del Hipermercado\n");
        printf("7. Mostrar Cantidad de Clientes con Compras Realizadas\n");
        printf("8. Regresar al Menu Principal\n");
        printf("=========================================================\n");
        opcion_ventas = obtener_entero("Seleccione una opcion: ");

        switch (opcion_ventas) {
            case 1: consultar_ventas_diarias_caja(); break;
            case 2: consultar_ventas_mensuales_caja(); break;
            case 3: mostrar_total_ventas_diarias(); break;
            case 4: mostrar_total_ventas_mensuales(); break;
            case 5: mostrar_producto_mas_vendido(); break;
            case 6: mostrar_cantidad_trabajadores_hipermercado(); break;
            case 7: mostrar_cantidad_clientes_con_compras(); break;
            case 8: break; // Regresa al menu principal
            default: printf("Opcion no valida. Intente de nuevo.\n"); presionar_enter_para_continuar(); break;
        }
    } while (opcion_ventas != 8);
}

void mostrar_ayuda_usuario() { // Ventana de Ayuda al Usuario
    limpiar_consola();
    printf("=========================================================================\n");
    printf("                          VENTANA DE AYUDA AL USUARIO                     \n");
    printf("========================= ================================================\n");
    printf("\nBienvenido al sistema de gestion de %s.\n", NOMBRE_HIPERMERCADO);
    printf("\nPara utilizar el programa de forma efectiva, especialmente para realizar ventas\n");
    printf("y obtener reportes precisos, es IMPORTANTE ingresar los datos en el\n");
    printf("siguiente orden logico.  Esto asegura que el sistema funcione correctamente\n");
    printf("y que todas las funcionalidades esten disponibles.\n");
    printf("\nOrden de Ingreso de Datos Recomendado:\n");
    printf("\n1.  Modulo TRABAJADORES:\n");
    printf("    -   Comience registrando a todos los trabajadores del Hipermercado.\n");
    printf("        Ejemplo de Numero de Empleado: 1, 2, 10, 25.  Formato: Numerico entero.\n");
    printf("        Los trabajadores son necesarios para operar las cajas y se les\n");
    printf("        asigna responsabilidad por las mismas.\n");
    printf("        Sin trabajadores, no podra crear cajas correctamente.\n");
    printf("\n2.  Modulo CAJAS:\n");
    printf("    -   Una vez que haya trabajadores registrados, cree las cajas operativas.\n");
    printf("        Ejemplo de Numero de Caja: 1, 2, 3, 4, 5.  Formato: Numerico entero.\n");
    printf("        Al crear una caja, el sistema le pedira que asigne un trabajador\n");
    printf("        responsable de entre los trabajadores que ya ha registrado.\n");
    printf("        Las cajas son esenciales para registrar las ventas.\n");
    printf("\n3.  Modulo PRODUCTOS:\n");
    printf("    -   Luego, registre todos los productos que se venden en el Hipermercado.\n");
    printf("        Ejemplo de Codigo de Producto: 101, 205, 350, 480.  Formato: Numerico entero.\n");
    printf("        Necesita tener productos en el sistema para poder venderlos\n");
    printf("        y generar facturas.  Ingrese detalles como codigo, nombre,\n");
    printf("        cantidad disponible y precio.\n");
    printf("\n4.  Modulo CLIENTES:\n");
    printf("    -   Finalmente, registre a sus clientes.\n");
    printf("        Ejemplo de Cedula de Cliente: 12345678, 98765432, 55554444. Formato: Numerico entero.\n");
    printf("        Aunque no son necesarios para registrar productos o cajas, los clientes son INDISPENSABLES para\n");
    printf("        emitir facturas de venta. Cada factura debe estar asociada a un cliente.\n");
    printf("\nSiguiendo este orden, se asegura de que cuando llegue el momento de:\n");
    printf("\n    -   Crear Facturas de Venta (Modulo FACTURAS)\n");
    printf("    -   Generar Reportes de Ventas (Modulo VENTAS Y REPORTES)\n");
    printf("\n...todos los datos necesarios (trabajadores, cajas, productos, clientes)\n");
    printf("estaran disponibles y el sistema funcionara de manera optima y sin errores\n");
    printf("debido a falta de informacion.\n");
    printf("\nSi tiene alguna duda, consulte nuevamente esta ayuda.  ¡Gracias por usar\n");
    printf("el sistema de %s!\n", NOMBRE_HIPERMERCADO);
    printf("\n=========================================================================\n");
    presionar_enter_para_continuar();
}


// ========================= Funciones de Productos =========================

void crear_producto() {
    if (cantidad_productos >= MAX_PRODUCTOS) {
        printf("¡Limite de productos alcanzado (%d)! No se pueden agregar mas productos.\n", MAX_PRODUCTOS);
        presionar_enter_para_continuar();
        return;
    }

    int cantidad_agregar;
    printf("Limite de productos: %d\n", MAX_PRODUCTOS);
    cantidad_agregar = obtener_entero("¿Cuantos productos desea agregar?: ");
    if (cantidad_agregar <= 0) {
        printf("Debe agregar al menos un producto.\n");
        presionar_enter_para_continuar();
        return;
    }
    if (cantidad_productos + cantidad_agregar > MAX_PRODUCTOS) {
        printf("No puede agregar %d productos. Limite maximo de productos: %d, Productos restantes disponibles: %d\n", cantidad_agregar, MAX_PRODUCTOS, MAX_PRODUCTOS - cantidad_productos);
        presionar_enter_para_continuar();
        return;
    }


    for (int i = 0; i < cantidad_agregar; i++) {
        limpiar_consola();
        printf("======== Nuevo Producto (%d de %d) ========\n", i + 1, cantidad_agregar);
        Producto nuevo_producto;

        nuevo_producto.codigo = obtener_entero("Ingrese el codigo del producto (Ejemplo: 101): ");
        if (buscar_producto_por_codigo(nuevo_producto.codigo) != -1) {
            printf("¡Ya existe un producto con el codigo %d! No se pueden agregar productos con codigos duplicados.\n", nuevo_producto.codigo);
            presionar_enter_para_continuar();
            i--; // Para repetir la iteracion del producto actual
            continue;
        }
        obtener_cadena("Ingrese el nombre del producto: ", nuevo_producto.nombre, sizeof(nuevo_producto.nombre));
        nuevo_producto.cantidad_disponible = obtener_entero("Ingrese la cantidad disponible: ");
        nuevo_producto.precio_bs = obtener_flotante("Ingrese el precio en Bolivares (Bs): ");

        calcular_precios_monedas(&nuevo_producto); // Calcula precios en otras monedas

        productos[cantidad_productos++] = nuevo_producto;
        printf("Producto '%s' agregado exitosamente.\n", nuevo_producto.nombre);
        presionar_enter_para_continuar();
    }
     printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
            mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}

void calcular_precios_monedas(Producto *producto) {
    producto->precio_usd = producto->precio_bs / TASA_USD;
    producto->precio_cop = producto->precio_bs / TASA_COP;
    producto->precio_eur = producto->precio_bs / TASA_EUR;
}


void modificar_producto() {
    if (cantidad_productos == 0) {
        printf("No hay productos registrados para modificar.\n");
        presionar_enter_para_continuar();
        return;
    }

    int codigo_modificar = obtener_entero("Ingrese el codigo del producto a modificar: ");
    int index = buscar_producto_por_codigo(codigo_modificar);

    if (index == -1) {
        printf("No se encontro ningun producto con el codigo %d.\n", codigo_modificar);
        presionar_enter_para_continuar();
        return;
    }

    limpiar_consola();
    printf("===== Modificar Producto =====\n");
    printf("Producto actual: %s (Codigo: %d)\n", productos[index].nombre, productos[index].codigo);

    obtener_cadena("Ingrese el nuevo nombre del producto (dejar en blanco para no modificar): ", productos[index].nombre, sizeof(productos[index].nombre));
    int cantidad = obtener_entero("Ingrese la nueva cantidad disponible (ingrese -1 para no modificar): ");
    if (cantidad != -1) {
        productos[index].cantidad_disponible = cantidad;
    }
    float precio_bs = obtener_flotante("Ingrese el nuevo precio en Bolivares (ingrese -1 para no modificar): ");
    if (precio_bs != -1) {
        productos[index].precio_bs = precio_bs;
        calcular_precios_monedas(&productos[index]); // Recalcular precios en otras monedas
    }


    printf("Producto modificado exitosamente.\n");
    presionar_enter_para_continuar();
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


void eliminar_producto() {
    if (cantidad_productos == 0) {
        printf("No hay productos registrados para eliminar.\n");
        presionar_enter_para_continuar();
        return;
    }

    int codigo_eliminar = obtener_entero("Ingrese el codigo del producto a eliminar: ");
    int index = buscar_producto_por_codigo(codigo_eliminar);

    if (index == -1) {
        printf("No se encontro ningun producto con el codigo %d.\n", codigo_eliminar);
        presionar_enter_para_continuar();
        return;
    }

    // Desplaza los productos siguientes una posicion hacia atras para eliminar el producto
    for (int i = index; i < cantidad_productos - 1; i++) {
        productos[i] = productos[i + 1];
    }
    cantidad_productos--;
    printf("Producto con codigo %d eliminado exitosamente.\n", codigo_eliminar);
    presionar_enter_para_continuar();
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}

void mostrar_producto() {
    if (cantidad_productos == 0) {
        printf("No hay productos registrados para mostrar.\n");
        presionar_enter_para_continuar();
        return;
    }

    int codigo_mostrar = obtener_entero("Ingrese el codigo del producto a mostrar: ");
    int index = buscar_producto_por_codigo(codigo_mostrar);

    if (index == -1) {
        printf("No se encontro ningun producto con el codigo %d.\n", codigo_mostrar);
        presionar_enter_para_continuar();
        return;
    }

    limpiar_consola();
    printf("========= Detalles del Producto =========\n");
    printf("Codigo: %d\n", productos[index].codigo);
    printf("Nombre: %s\n", productos[index].nombre);
    printf("Cantidad Disponible: %d\n", productos[index].cantidad_disponible);
    printf("Precio (Bs): %.2f\n", productos[index].precio_bs);
    printf("Precio (USD): %.2f\n", productos[index].precio_usd);
    printf("Precio (COP): %.2f\n", productos[index].precio_cop);
    printf("Precio (EUR): %.2f\n", productos[index].precio_eur);
    printf("===========================================\n");
    presionar_enter_para_continuar();
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}

void mostrar_todos_productos() {
    if (cantidad_productos == 0) {
        printf("No hay productos registrados para mostrar.\n");
        presionar_enter_para_continuar();
        return;
    }
    limpiar_consola();
    printf("========= Listado de Productos =========\n");
    printf("====================================================================================\n");
    printf("%-10s %-30s %-15s %-15s %-15s %-15s %-15s\n", "Codigo", "Nombre", "Cantidad", "Precio Bs", "Precio USD", "Precio COP", "Precio EUR");
    printf("====================================================================================\n");

    for (int i = 0; i < cantidad_productos; i++) {
        printf("%-10d %-30s %-15d %-15.2f %-15.2f %-15.2f %-15.2f\n",
               productos[i].codigo, productos[i].nombre, productos[i].cantidad_disponible,
               productos[i].precio_bs, productos[i].precio_usd, productos[i].precio_cop, productos[i].precio_eur);
    }
    printf("====================================================================================\n");
    presionar_enter_para_continuar();
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


// ========================= Funciones de Clientes =========================


void crear_cliente() {
    if (cantidad_clientes >= MAX_CLIENTES) {
        printf("¡Limite de clientes alcanzado (%d)! No se pueden agregar mas clientes.\n", MAX_CLIENTES);
        presionar_enter_para_continuar();
        return;
    }

    int cantidad_agregar;
    printf("Limite de clientes: %d\n", MAX_CLIENTES);
    cantidad_agregar = obtener_entero("¿Cuantos clientes desea agregar?: ");
    if (cantidad_agregar <= 0) {
        printf("Debe agregar al menos un cliente.\n");
        presionar_enter_para_continuar();
        return;
    }
    if (cantidad_clientes + cantidad_agregar > MAX_CLIENTES) {
        printf("No puede agregar %d clientes. Limite maximo de clientes: %d, Clientes restantes disponibles: %d\n", cantidad_agregar, MAX_CLIENTES, MAX_CLIENTES - cantidad_clientes);
        presionar_enter_para_continuar();
        return;
    }


    for (int i = 0; i < cantidad_agregar; i++) {
        limpiar_consola();
        printf("======== Nuevo Cliente (%d de %d) ========\n", i + 1, cantidad_agregar);
        Cliente nuevo_cliente;

        nuevo_cliente.cedula = obtener_entero("Ingrese la cedula del cliente (Ejemplo: 12345678): ");
        if (buscar_cliente_por_cedula(nuevo_cliente.cedula) != -1) {
            printf("¡Ya existe un cliente con la cedula %d! No se pueden agregar clientes con cedulas duplicadas.\n", nuevo_cliente.cedula);
            presionar_enter_para_continuar();
            i--; // Para repetir la iteracion del cliente actual
            continue;
        }
        obtener_cadena("Ingrese el nombre del cliente: ", nuevo_cliente.nombre, sizeof(nuevo_cliente.nombre));
        obtener_cadena("Ingrese el apellido del cliente: ", nuevo_cliente.apellido, sizeof(nuevo_cliente.apellido));
        obtener_cadena("Ingrese el telefono del cliente (Ejemplo: 04141234567): ", nuevo_cliente.telefono, sizeof(nuevo_cliente.telefono));
        obtener_cadena("Ingrese la direccion del cliente: ", nuevo_cliente.direccion, sizeof(nuevo_cliente.direccion));


        clientes[cantidad_clientes++] = nuevo_cliente;
        printf("Cliente '%s %s' agregado exitosamente.\n", nuevo_cliente.nombre, nuevo_cliente.apellido);
        presionar_enter_para_continuar();
    }
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


void modificar_cliente() {
    if (cantidad_clientes == 0) {
        printf("No hay clientes registrados para modificar.\n");
        presionar_enter_para_continuar();
        return;
    }

    int cedula_modificar = obtener_entero("Ingrese la cedula del cliente a modificar: ");
    int index = buscar_cliente_por_cedula(cedula_modificar);

    if (index == -1) {
        printf("No se encontro ningun cliente con la cedula %d.\n", cedula_modificar);
        presionar_enter_para_continuar();
        return;
    }

    limpiar_consola();
    printf("===== Modificar Cliente =====\n");
    printf("Cliente actual: %s %s (Cedula: %d)\n", clientes[index].nombre, clientes[index].apellido, clientes[index].cedula);

    obtener_cadena("Ingrese el nuevo nombre del cliente (dejar en blanco para no modificar): ", clientes[index].nombre, sizeof(clientes[index].nombre));
    obtener_cadena("Ingrese el nuevo apellido del cliente (dejar en blanco para no modificar): ", clientes[index].apellido, sizeof(clientes[index].apellido));
    obtener_cadena("Ingrese el nuevo telefono del cliente (dejar en blanco para no modificar): ", clientes[index].telefono, sizeof(clientes[index].telefono));
    obtener_cadena("Ingrese la nueva direccion del cliente (dejar en blanco para no modificar): ", clientes[index].direccion, sizeof(clientes[index].direccion));


    printf("Cliente modificado exitosamente.\n");
    presionar_enter_para_continuar();
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


void eliminar_cliente() {
    if (cantidad_clientes == 0) {
        printf("No hay clientes registrados para eliminar.\n");
        presionar_enter_para_continuar();
        return;
    }

    int cedula_eliminar = obtener_entero("Ingrese la cedula del cliente a eliminar: ");
    int index = buscar_cliente_por_cedula(cedula_eliminar);

    if (index == -1) {
        printf("No se encontro ningun cliente con la cedula %d.\n", cedula_eliminar);
        presionar_enter_para_continuar();
        return;
    }

    // Desplaza los clientes siguientes una posicion hacia atras para eliminar el cliente
    for (int i = index; i < cantidad_clientes - 1; i++) {
        clientes[i] = clientes[i + 1];
    }
    cantidad_clientes--;
    printf("Cliente con cedula %d eliminado exitosamente.\n", cedula_eliminar);
    presionar_enter_para_continuar();
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


void mostrar_cliente() {
    if (cantidad_clientes == 0) {
        printf("No hay clientes registrados para mostrar.\n");
        presionar_enter_para_continuar();
        return;
    }

    int cedula_mostrar = obtener_entero("Ingrese la cedula del cliente a mostrar: ");
    int index = buscar_cliente_por_cedula(cedula_mostrar);

    if (index == -1) {
        printf("No se encontro ningun cliente con la cedula %d.\n", cedula_mostrar);
        presionar_enter_para_continuar();
        return;
    }

    limpiar_consola();
    printf("========= Detalles del Cliente =========\n");
    printf("Cedula: %d\n", clientes[index].cedula);
    printf("Nombre: %s\n", clientes[index].nombre);
    printf("Apellido: %s\n", clientes[index].apellido);
    printf("Telefono: %s\n", clientes[index].telefono);
    printf("Direccion: %s\n", clientes[index].direccion);
    printf("===========================================\n");
    presionar_enter_para_continuar();
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


void mostrar_todos_clientes() {
    if (cantidad_clientes == 0) {
        printf("No hay clientes registrados para mostrar.\n");
        presionar_enter_para_continuar();
        return;
    }
    limpiar_consola();
    printf("========= Listado de Clientes =========\n");
    printf("====================================================================================\n");
    printf("%-15s %-25s %-25s %-15s %-30s\n", "Cedula", "Nombre", "Apellido", "Telefono", "Direccion");
    printf("====================================================================================\n");

    for (int i = 0; i < cantidad_clientes; i++) {
        printf("%-15d %-25s %-25s %-15s %-30s\n",
               clientes[i].cedula, clientes[i].nombre, clientes[i].apellido, clientes[i].telefono, clientes[i].direccion);
    }
    printf("====================================================================================\n");
    presionar_enter_para_continuar();
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


// ========================= Funciones de Trabajadores =========================


void crear_trabajador() {
    if (cantidad_trabajadores >= MAX_TRABAJADORES) {
        printf("¡Limite de trabajadores alcanzado (%d)! No se pueden agregar mas trabajadores.\n", MAX_TRABAJADORES);
        presionar_enter_para_continuar();
        return;
    }
    int cantidad_agregar;
     printf("Limite de trabajadores: %d\n", MAX_TRABAJADORES);
    cantidad_agregar = obtener_entero("¿Cuantos trabajadores desea agregar?: ");
    if (cantidad_agregar <= 0) {
        printf("Debe agregar al menos un trabajador.\n");
        presionar_enter_para_continuar();
        return;
    }
    if (cantidad_trabajadores + cantidad_agregar > MAX_TRABAJADORES) {
        printf("No puede agregar %d trabajadores. Limite maximo de trabajadores: %d, Trabajadores restantes disponibles: %d\n", cantidad_agregar, MAX_TRABAJADORES, MAX_TRABAJADORES - cantidad_trabajadores);
        presionar_enter_para_continuar();
        return;
    }


    for (int i = 0; i < cantidad_agregar; i++) {
        limpiar_consola();
        printf("======== Nuevo Trabajador (%d de %d) ========\n", i + 1, cantidad_agregar);
        Trabajador nuevo_trabajador;

        nuevo_trabajador.numero_empleado = obtener_entero("Ingrese el numero de empleado (Ejemplo: 1): ");
        if (buscar_trabajador_por_numero_empleado(nuevo_trabajador.numero_empleado) != -1) {
            printf("¡Ya existe un trabajador con el numero de empleado %d! No se pueden agregar trabajadores con numeros de empleado duplicados.\n", nuevo_trabajador.numero_empleado);
            presionar_enter_para_continuar();
            i--; // Para repetir la iteracion del trabajador actual
            continue;
        }
        nuevo_trabajador.cedula = obtener_entero("Ingrese la cedula del trabajador (Ejemplo: 12345678): ");
        obtener_cadena("Ingrese el nombre del trabajador: ", nuevo_trabajador.nombre, sizeof(nuevo_trabajador.nombre));
        obtener_cadena("Ingrese el apellido del trabajador: ", nuevo_trabajador.apellido, sizeof(nuevo_trabajador.apellido));
        obtener_cadena("Ingrese el telefono del trabajador (Ejemplo: 04141234567): ", nuevo_trabajador.telefono, sizeof(nuevo_trabajador.telefono));
        obtener_cadena("Ingrese la direccion del trabajador: ", nuevo_trabajador.direccion, sizeof(nuevo_trabajador.direccion));


        trabajadores[cantidad_trabajadores++] = nuevo_trabajador;
        printf("Trabajador '%s %s' agregado exitosamente.\n", nuevo_trabajador.nombre, nuevo_trabajador.apellido);
        presionar_enter_para_continuar();
    }
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


void modificar_trabajador() {
    if (cantidad_trabajadores == 0) {
        printf("No hay trabajadores registrados para modificar.\n");
        presionar_enter_para_continuar();
        return;
    }

    int numero_empleado_modificar = obtener_entero("Ingrese el numero de empleado del trabajador a modificar: ");
    int index = buscar_trabajador_por_numero_empleado(numero_empleado_modificar);

    if (index == -1) {
        printf("No se encontro ningun trabajador con el numero de empleado %d.\n", numero_empleado_modificar);
        presionar_enter_para_continuar();
        return;
    }

    limpiar_consola();
    printf("===== Modificar Trabajador =====\n");
    printf("Trabajador actual: %s %s (Numero de Empleado: %d)\n", trabajadores[index].nombre, trabajadores[index].apellido, trabajadores[index].numero_empleado);

    obtener_cadena("Ingrese el nuevo nombre del trabajador (dejar en blanco para no modificar): ", trabajadores[index].nombre, sizeof(trabajadores[index].nombre));
    obtener_cadena("Ingrese el nuevo apellido del trabajador (dejar en blanco para no modificar): ", trabajadores[index].apellido, sizeof(trabajadores[index].apellido));
    obtener_cadena("Ingrese el nuevo telefono del trabajador (dejar en blanco para no modificar): ", trabajadores[index].telefono, sizeof(trabajadores[index].telefono));
    obtener_cadena("Ingrese la nueva direccion del trabajador (dejar en blanco para no modificar): ", trabajadores[index].direccion, sizeof(trabajadores[index].direccion));
    int cedula = obtener_entero("Ingrese la nueva cedula del trabajador (ingrese -1 para no modificar): ");
    if (cedula != -1) {
        trabajadores[index].cedula = cedula;
    }


    printf("Trabajador modificado exitosamente.\n");
    presionar_enter_para_continuar();
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


void eliminar_trabajador() {
    if (cantidad_trabajadores == 0) {
        printf("No hay trabajadores registrados para eliminar.\n");
        presionar_enter_para_continuar();
        return;
    }

    int numero_empleado_eliminar = obtener_entero("Ingrese el numero de empleado del trabajador a eliminar: ");
    int index = buscar_trabajador_por_numero_empleado(numero_empleado_eliminar);

    if (index == -1) {
        printf("No se encontro ningun trabajador con el numero de empleado %d.\n", numero_empleado_eliminar);
        presionar_enter_para_continuar();
        return;
    }

    // Desplaza los trabajadores siguientes una posicion hacia atras para eliminar el trabajador
    for (int i = index; i < cantidad_trabajadores - 1; i++) {
        trabajadores[i] = trabajadores[i + 1];
    }
    cantidad_trabajadores--;
    printf("Trabajador con numero de empleado %d eliminado exitosamente.\n", numero_empleado_eliminar);
    presionar_enter_para_continuar();
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


void mostrar_trabajador() {
    if (cantidad_trabajadores == 0) {
        printf("No hay trabajadores registrados para mostrar.\n");
        presionar_enter_para_continuar();
        return;
    }

    int numero_empleado_mostrar = obtener_entero("Ingrese el numero de empleado del trabajador a mostrar: ");
    int index = buscar_trabajador_por_numero_empleado(numero_empleado_mostrar);

    if (index == -1) {
        printf("No se encontro ningun trabajador con el numero de empleado %d.\n", numero_empleado_mostrar);
        presionar_enter_para_continuar();
        return;
    }

    limpiar_consola();
    printf("========= Detalles del Trabajador =========\n");
    printf("Numero de Empleado: %d\n", trabajadores[index].numero_empleado);
    printf("Cedula: %d\n", trabajadores[index].cedula);
    printf("Nombre: %s\n", trabajadores[index].nombre);
    printf("Apellido: %s\n", trabajadores[index].apellido);
    printf("Telefono: %s\n", trabajadores[index].telefono);
    printf("Direccion: %s\n", trabajadores[index].direccion);
    printf("===========================================\n");
    presionar_enter_para_continuar();
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


void mostrar_todos_trabajadores() {
    if (cantidad_trabajadores == 0) {
        printf("No hay trabajadores registrados para mostrar.\n");
        presionar_enter_para_continuar();
        return;
    }
    limpiar_consola();
    printf("========= Listado de Trabajadores =========\n");
    printf("====================================================================================\n");
    printf("%-20s %-15s %-25s %-25s %-15s %-30s\n", "Numero de Empleado", "Cedula", "Nombre", "Apellido", "Telefono", "Direccion");
    printf("====================================================================================\n");

    for (int i = 0; i < cantidad_trabajadores; i++) {
        printf("%-20d %-15d %-25s %-25s %-15s %-30s\n",
               trabajadores[i].numero_empleado, trabajadores[i].cedula, trabajadores[i].nombre, trabajadores[i].apellido, trabajadores[i].telefono, trabajadores[i].direccion);
    }
    printf("====================================================================================\n");
    presionar_enter_para_continuar();
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


// ========================= Funciones de Cajas =========================

void crear_caja() {
    if (cantidad_cajas >= MAX_CAJAS) {
        printf("¡Limite de cajas alcanzado (%d)! No se pueden agregar mas cajas.\n", MAX_CAJAS);
        presionar_enter_para_continuar();
        return;
    }
     int cantidad_agregar;
      printf("Limite de cajas: %d\n", MAX_CAJAS);
    cantidad_agregar = obtener_entero("¿Cuantas cajas desea agregar?: ");
    if (cantidad_agregar <= 0) {
        printf("Debe agregar al menos una caja.\n");
        presionar_enter_para_continuar();
        return;
    }
    if (cantidad_cajas + cantidad_agregar > MAX_CAJAS) {
        printf("No puede agregar %d cajas. Limite maximo de cajas: %d, Cajas restantes disponibles: %d\n", cantidad_agregar, MAX_CAJAS, MAX_CAJAS - cantidad_cajas);
        presionar_enter_para_continuar();
        return;
    }


    for (int i = 0; i < cantidad_agregar; i++) {
        limpiar_consola();
        printf("======== Nueva Caja (%d de %d) ========\n", i + 1, cantidad_agregar);
        Caja nueva_caja;

        nueva_caja.numero_caja = obtener_entero("Ingrese el numero de caja (Ejemplo: 1): ");
        if (buscar_caja_por_numero(nueva_caja.numero_caja) != -1) {
            printf("¡Ya existe una caja con el numero %d! No se pueden agregar cajas con numeros duplicados.\n", nueva_caja.numero_caja);
            presionar_enter_para_continuar();
            i--; // Para repetir la iteracion de la caja actual
            continue;
        }
        nueva_caja.numero_empleado = obtener_entero("Ingrese el numero de empleado responsable de la caja: ");
        int trabajador_index = buscar_trabajador_por_numero_empleado(nueva_caja.numero_empleado);
        if (trabajador_index == -1) {
            printf("¡No existe un trabajador con el numero de empleado %d! Debe crear el trabajador primero.\n", nueva_caja.numero_empleado);
            presionar_enter_para_continuar();
            i--; // Para repetir la iteracion de la caja actual
            continue;
        }
        strcpy(nueva_caja.nombre_empleado, trabajadores[trabajador_index].nombre); //Asocia nombre del trabajador
        nueva_caja.total_bs_facturado = 0.0;
        nueva_caja.total_usd_facturado = 0.0;
        nueva_caja.total_cop_facturado = 0.0;
        nueva_caja.total_eur_facturado = 0.0;


        cajas[cantidad_cajas++] = nueva_caja;
        printf("Caja numero %d agregada exitosamente.\n", nueva_caja.numero_caja);
        presionar_enter_para_continuar();
    }
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


void modificar_caja() {
    if (cantidad_cajas == 0) {
        printf("No hay cajas registradas para modificar.\n");
        presionar_enter_para_continuar();
        return;
    }

    int numero_caja_modificar = obtener_entero("Ingrese el numero de caja a modificar: ");
    int index = buscar_caja_por_numero(numero_caja_modificar);

    if (index == -1) {
        printf("No se encontro ninguna caja con el numero %d.\n", numero_caja_modificar);
        presionar_enter_para_continuar();
        return;
    }

    limpiar_consola();
    printf("===== Modificar Caja =====\n");
    printf("Caja actual: Numero %d, Responsable: %s (Numero de Empleado: %d)\n", cajas[index].numero_caja, cajas[index].nombre_empleado, cajas[index].numero_empleado);

    int nuevo_numero_empleado = obtener_entero("Ingrese el nuevo numero de empleado responsable de la caja (ingrese -1 para no modificar): ");
    if (nuevo_numero_empleado != -1) {
        int trabajador_index = buscar_trabajador_por_numero_empleado(nuevo_numero_empleado);
        if (trabajador_index == -1) {
            printf("¡No existe un trabajador con el numero de empleado %d! Modificacion de caja cancelada.\n", nuevo_numero_empleado);
            presionar_enter_para_continuar();
            return;
        }
        cajas[index].numero_empleado = nuevo_numero_empleado;
        strcpy(cajas[index].nombre_empleado, trabajadores[trabajador_index].nombre); // Actualiza nombre del trabajador
    }


    printf("Caja modificada exitosamente.\n");
    presionar_enter_para_continuar();
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


void eliminar_caja() {
    if (cantidad_cajas == 0) {
        printf("No hay cajas registradas para eliminar.\n");
        presionar_enter_para_continuar();
        return;
    }

    int numero_caja_eliminar = obtener_entero("Ingrese el numero de caja a eliminar: ");
    int index = buscar_caja_por_numero(numero_caja_eliminar);

    if (index == -1) {
        printf("No se encontro ninguna caja con el numero %d.\n", numero_caja_eliminar);
        presionar_enter_para_continuar();
        return;
    }

    // Desplaza las cajas siguientes una posicion hacia atras para eliminar la caja
    for (int i = index; i < cantidad_cajas - 1; i++) {
        cajas[i] = cajas[i + 1];
    }
    cantidad_cajas--;
    printf("Caja con numero %d eliminada exitosamente.\n", numero_caja_eliminar);
    presionar_enter_para_continuar();
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


void mostrar_caja() {
    if (cantidad_cajas == 0) {
        printf("No hay cajas registradas para mostrar.\n");
        presionar_enter_para_continuar();
        return;
    }

    int numero_caja_mostrar = obtener_entero("Ingrese el numero de caja a mostrar: ");
    int index = buscar_caja_por_numero(numero_caja_mostrar);

    if (index == -1) {
        printf("No se encontro ninguna caja con el numero %d.\n", numero_caja_mostrar);
        presionar_enter_para_continuar();
        return;
    }

    limpiar_consola();
    printf("========= Detalles de la Caja =========\n");
    printf("Numero de Caja: %d\n", cajas[index].numero_caja);
    printf("Numero de Empleado Responsable: %d\n", cajas[index].numero_empleado);
    printf("Nombre del Empleado Responsable: %s\n", cajas[index].nombre_empleado);
    printf("Total Facturado (Bs): %.2f\n", cajas[index].total_bs_facturado);
    printf("Total Facturado (USD): %.2f\n", cajas[index].total_usd_facturado);
    printf("Total Facturado (COP): %.2f\n", cajas[index].total_cop_facturado);
    printf("Total Facturado (EUR): %.2f\n", cajas[index].total_eur_facturado);
    printf("===========================================\n");
    presionar_enter_para_continuar();
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


void mostrar_todas_cajas() {
    if (cantidad_cajas == 0) {
        printf("No hay cajas registradas para mostrar.\n");
        presionar_enter_para_continuar();
        return;
    }
    limpiar_consola();
    printf("========= Listado de Cajas =========\n");
    printf("==================================================================================================================\n");
    printf("%-15s %-25s %-30s %-20s %-20s %-20s %-20s\n", "Numero de Caja", "Empleado Responsable", "Nombre Empleado", "Total Bs Facturado", "Total USD Facturado", "Total COP Facturado", "Total EUR Facturado");
    printf("==================================================================================================================\n");

    for (int i = 0; i < cantidad_cajas; i++) {
        printf("%-15d %-25d %-30s %-20.2f %-20.2f %-20.2f %-20.2f\n",
               cajas[i].numero_caja, cajas[i].numero_empleado, cajas[i].nombre_empleado,
               cajas[i].total_bs_facturado, cajas[i].total_usd_facturado, cajas[i].total_cop_facturado, cajas[i].total_eur_facturado);
    }
    printf("==================================================================================================================\n");
    presionar_enter_para_continuar();
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


// ========================= Funciones de Facturas =========================

void crear_factura() {
    if (cantidad_facturas >= MAX_FACTURAS) {
        printf("¡Limite de facturas alcanzado (%d)! No se pueden generar mas facturas.\n", MAX_FACTURAS);
        presionar_enter_para_continuar();
        return;
    }
    if (cantidad_cajas == 0) {
        printf("No hay cajas registradas. Debe crear cajas para poder facturar.\n");
        presionar_enter_para_continuar();
        return;
    }
    if (cantidad_trabajadores == 0) {
        printf("No hay trabajadores registrados. Debe crear trabajadores y asignarlos a las cajas para poder facturar.\n");
        presionar_enter_para_continuar();
        return;
    }
    if (cantidad_clientes == 0) {
        printf("No hay clientes registrados. Debe registrar clientes para poder facturar.\n");
        presionar_enter_para_continuar();
        return;
    }
    if (cantidad_productos == 0) {
        printf("No hay productos registrados. Debe registrar productos para poder facturar.\n");
        presionar_enter_para_continuar();
        return;
    }

    limpiar_consola();
    printf("=============== CREAR NUEVA FACTURA ===============\n"); // Titulo mas amigable
    Factura nueva_factura;

    nueva_factura.numero_factura = numero_factura_actual++; // Asigna numero y aumenta para la siguiente factura
    nueva_factura.fecha = time(NULL); // Fecha y hora actual
    nueva_factura.cantidad_productos_factura = 0; // Inicia contador de productos en factura
    nueva_factura.monto_total = 0.0; // Inicializa monto total

    // ========================= 1. Seleccionar Caja =========================
    mostrar_todas_cajas(); // Muestra listado de cajas disponibles
    int numero_caja_factura = obtener_entero("1. Ingrese el NUMERO DE CAJA para esta factura: "); // Prompt mas claro
    int caja_index = buscar_caja_por_numero(numero_caja_factura);
    if (caja_index == -1) {
        printf("¡CAJA NO ENCONTRADA! No existe la caja numero %d. Factura cancelada.\n", numero_caja_factura); // Mensaje mas amigable
        presionar_enter_para_continuar();
        return;
    }
    nueva_factura.numero_caja = cajas[caja_index].numero_caja; // Asigna numero de caja a la factura
    nueva_factura.numero_empleado_caja = cajas[caja_index].numero_empleado; // Asigna numero de empleado de la caja a la factura
    printf("   Caja %d seleccionada. Cajero responsable: %s\n", cajas[caja_index].numero_caja, cajas[caja_index].nombre_empleado); // Feedback al usuario

    // ========================= 2. Seleccionar Cliente =========================
    printf("\n--- 2. DATOS DEL CLIENTE ---\n"); // Separador visual mas claro
    printf("   Ingrese la CEDULA del cliente:\n"); // Instruccion mas directa
    int cedula_cliente = obtener_entero("   Cedula del Cliente: "); // Prompt mas directo
    int cliente_index = buscar_cliente_por_cedula(cedula_cliente);
    if (cliente_index == -1) {
        printf("¡CLIENTE NO ENCONTRADO! No existe cliente con cedula %d. Factura cancelada.\n", cedula_cliente); // Mensaje mas amigable
        presionar_enter_para_continuar();
        return;
    }
    nueva_factura.cliente = clientes[cliente_index]; // Asigna cliente a la factura
    printf("   Cliente: %s %s\n", clientes[cliente_index].nombre, clientes[cliente_index].apellido); // Feedback al usuario


    float subtotal_factura = 0.0;

    // ========================= 3. Agregar productos =========================
    char agregar_producto = 's';
    while (agregar_producto == 's' || agregar_producto == 'S') {
        if (nueva_factura.cantidad_productos_factura >= MAX_PRODUCTOS_EN_FACTURA) {
            printf("¡MAXIMO DE PRODUCTOS POR FACTURA ALCANZADO (%d)!\n", MAX_PRODUCTOS_EN_FACTURA); // Mensaje mas amigable
            break; // Sale del ciclo de agregar productos
        }

        printf("\n--- 3. AGREGAR PRODUCTO A LA FACTURA ---\n"); // Separador visual mas claro
        printf("   Ingrese el CODIGO del producto:\n"); // Instruccion mas directa
        int codigo_producto = obtener_entero("   Codigo del Producto (o 0 para TERMINAR): "); // Prompt mas directo, opcion de terminar
        if (codigo_producto == 0) {
            break; // Termina de agregar productos
        }
        int producto_index = buscar_producto_por_codigo(codigo_producto);
        if (producto_index == -1) {
            printf("¡PRODUCTO NO ENCONTRADO! No existe producto con codigo %d. Intente de nuevo.\n", codigo_producto); // Mensaje mas amigable
            continue; // Vuelve a pedir codigo de producto
        }

        // Corrección aquí: Separar printf para el mensaje formateado y obtener_entero para la entrada
        char prompt_message[100];
        sprintf(prompt_message, "   Cantidad a facturar de '%s': ", productos[producto_index].nombre);
        int cantidad_producto_factura = obtener_entero(prompt_message);


        if (cantidad_producto_factura <= 0) {
            printf("¡CANTIDAD NO VALIDA! Debe ser mayor a 0. Intente de nuevo.\n"); // Mensaje mas amigable
            continue; // Vuelve a pedir cantidad
        }
        if (productos[producto_index].cantidad_disponible < cantidad_producto_factura) {
            printf("¡STOCK INSUFICIENTE! No hay suficiente stock disponible de '%s'. Stock disponible: %d. Intente de nuevo.\n", productos[producto_index].nombre, productos[producto_index].cantidad_disponible); // Mensaje mas amigable
            continue; // Vuelve a pedir cantidad
        }


        nueva_factura.productos_factura[nueva_factura.cantidad_productos_factura].producto = productos[producto_index]; // Asigna producto
        nueva_factura.productos_factura[nueva_factura.cantidad_productos_factura].cantidad = cantidad_producto_factura; // Asigna cantidad
        nueva_factura.cantidad_productos_factura++; // Incrementa contador de productos en factura

        productos[producto_index].cantidad_disponible -= cantidad_producto_factura; // Reduce stock del producto vendido
        subtotal_factura += productos[producto_index].precio_bs * cantidad_producto_factura; // Acumula subtotal

        printf("   Producto '%s' agregado a la factura.\n", productos[producto_index].nombre); // Feedback positivo

        printf("¿Agregar OTRO producto a la factura? (s/n): "); // Pregunta mas directa
        scanf(" %c", &agregar_producto);
    }

    // Calcula total con IVA
    float iva = subtotal_factura * (IVA_PORCENTAJE / 100.0);
    nueva_factura.monto_total = subtotal_factura + iva;

    facturas[cantidad_facturas++] = nueva_factura; // Agrega factura al listado
    cajas[caja_index].total_bs_facturado += nueva_factura.monto_total; // Acumula el total facturado en la caja

    limpiar_consola();
    printf("=================== FACTURA GENERADA ====================\n"); // Titulo final mas amigable
    mostrar_factura_detalle(&nueva_factura); // Muestra detalle de la factura generada
    printf("=========================================================\n");
    presionar_enter_para_continuar();
    printf("¿Desea regresar al menu principal? (s/n): "); // Mensaje final de regresar al menu principal
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}

void mostrar_factura() {
    if (cantidad_facturas == 0) {
        printf("No hay facturas generadas para mostrar.\n");
        presionar_enter_para_continuar();
        return;
    }

    int numero_factura_mostrar = obtener_entero("Ingrese el numero de factura a mostrar: ");
    int index = buscar_factura_por_numero(numero_factura_mostrar);

    if (index == -1) {
        printf("No se encontro ninguna factura con el numero %d.\n", numero_factura_mostrar);
        presionar_enter_para_continuar();
        return;
    }
    limpiar_consola();
    printf("=================== Detalle de Factura ====================\n");
    mostrar_factura_detalle(&facturas[index]);
    printf("=========================================================\n");
    presionar_enter_para_continuar();
    printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}

void mostrar_factura_detalle(Factura *factura) {
    struct tm *info_tiempo;
    char buffer_tiempo[80];

    time_t tiempo_unix = factura->fecha;
    info_tiempo = localtime(&tiempo_unix);
    strftime(buffer_tiempo, sizeof(buffer_tiempo), "%d/%m/%Y %H:%M:%S", info_tiempo);

    printf("%s\n", NOMBRE_HIPERMERCADO);
    printf("RIF: %s - Telefono: %s\n", RIF_HIPERMERCADO, TELEFONO_HIPERMERCADO);
    printf("=========================================================\n");
    printf("Factura No: %06d\n", factura->numero_factura); // Formato con 6 digitos y ceros a la izquierda
    printf("Fecha: %s\n", buffer_tiempo);
    printf("Caja No: %d - Empleado: %s (Empleado No: %d)\n", factura->numero_caja, cajas[buscar_caja_por_numero(factura->numero_caja)].nombre_empleado, factura->numero_empleado_caja);
    printf("---------------- Datos del Cliente ----------------------\n");
    printf("Nombre: %s %s\n", factura->cliente.nombre, factura->cliente.apellido);
    printf("Cedula: %d\n", factura->cliente.cedula);
    printf("------------------- Productos ---------------------------\n");
    printf("%-10s %-30s %-10s %-15s\n", "Cantidad", "Descripcion", "Precio Unitario", "Subtotal");
    printf("---------------------------------------------------------\n");
    float subtotal_productos = 0.0;
    for (int i = 0; i < factura->cantidad_productos_factura; i++) {
        float subtotal_producto = factura->productos_factura[i].producto.precio_bs * factura->productos_factura[i].cantidad;
        printf("%-10d %-30s %-10.2f %-15.2f\n", factura->productos_factura[i].cantidad,
               factura->productos_factura[i].producto.nombre, factura->productos_factura[i].producto.precio_bs, subtotal_producto);
        subtotal_productos += subtotal_producto;
    }
    printf("---------------------------------------------------------\n");
    printf("Subtotal:                          %.2f Bs\n", subtotal_productos);
    printf("IVA (%0.0f%%):                         %.2f Bs\n", IVA_PORCENTAJE, subtotal_productos * (IVA_PORCENTAJE / 100.0));
    printf("=========================================================\n");
    printf("Total a Pagar:                     %.2f Bs\n", factura->monto_total);
    printf("=========================================================\n");
}

void mostrar_ultimas_facturas() {
    if (cantidad_facturas == 0) {
        printf("No hay facturas generadas para mostrar.\n");
        presionar_enter_para_continuar();
        return;
    }

    limpiar_consola();
    printf("========= Ultimas 10 Facturas Generadas =========\n");
    if (cantidad_facturas <= 10) {
        for (int i = 0; i < cantidad_facturas; i++) {
            printf("----- Factura No: %06d -----\n", facturas[i].numero_factura);
            mostrar_factura_detalle(&facturas[i]);
        }
    } else {
        for (int i = cantidad_facturas - 10; i < cantidad_facturas; i++) {
             printf("----- Factura No: %06d -----\n", facturas[i].numero_factura);
            mostrar_factura_detalle(&facturas[i]);
        }
    }
    presionar_enter_para_continuar();
     printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


// ========================= Funciones de Ventas y Reportes =========================

void consultar_ventas_diarias_caja() {
    if (cantidad_cajas == 0) {
        printf("No hay cajas registradas para consultar ventas por caja.\n");
        presionar_enter_para_continuar();
        return;
    }
    if (cantidad_facturas == 0) {
        printf("No hay facturas generadas para consultar ventas.\n");
        presionar_enter_para_continuar();
        return;
    }

    limpiar_consola();
    mostrar_todas_cajas(); // Muestra listado para seleccionar caja
    int numero_caja_consultar = obtener_entero("Ingrese el numero de caja para consultar ventas diarias: ");
    int caja_index = buscar_caja_por_numero(numero_caja_consultar);
    if (caja_index == -1) {
        printf("No se encontro ninguna caja con el numero %d. Reporte cancelado.\n", numero_caja_consultar);
        presionar_enter_para_continuar();
        return;
    }

    time_t tiempo_actual = time(NULL);
    struct tm *fecha_actual_tm = localtime(&tiempo_actual);
    int dia_actual = fecha_actual_tm->tm_mday;
    int mes_actual = fecha_actual_tm->tm_mon + 1; // Meses en tm struct van de 0 a 11
    int ano_actual = fecha_actual_tm->tm_year + 1900;

    float total_ventas_bs = 0;
    int cantidad_facturas_caja = 0;
    char reporte_diario_caja[2000]; // Buffer para reporte en memoria
    sprintf(reporte_diario_caja, "========= Reporte Ventas Diarias Caja No %d - Fecha: %02d/%02d/%d =========\n", cajas[caja_index].numero_caja, dia_actual, mes_actual, ano_actual);

    for (int i = 0; i < cantidad_facturas; i++) {
        if (facturas[i].numero_caja == numero_caja_consultar) {
            struct tm *fecha_factura_tm = localtime(&facturas[i].fecha);
            if (fecha_factura_tm->tm_mday == dia_actual && fecha_factura_tm->tm_mon + 1 == mes_actual && fecha_factura_tm->tm_year + 1900 == ano_actual) {
                total_ventas_bs += facturas[i].monto_total;
                cantidad_facturas_caja++;
            }
        }
    }
    sprintf(reporte_diario_caja + strlen(reporte_diario_caja), "Caja No: %d - Empleado Responsable: %s\n", cajas[caja_index].numero_caja, cajas[caja_index].nombre_empleado);
    sprintf(reporte_diario_caja + strlen(reporte_diario_caja), "Total Ventas del Dia: %.2f Bs\n", total_ventas_bs);
    sprintf(reporte_diario_caja + strlen(reporte_diario_caja), "Cantidad de Facturas Emitidas: %d\n", cantidad_facturas_caja);
    sprintf(reporte_diario_caja + strlen(reporte_diario_caja), "===================================================================\n");


    limpiar_consola();
    printf("%s", reporte_diario_caja); // Imprime reporte en consola
    presionar_enter_para_continuar();

    guardar_reporte_txt("reporte_ventas_diarias_caja.txt", reporte_diario_caja); // Guarda reporte en archivo TXT
    printf("Reporte de ventas diarias por caja guardado en 'reporte_ventas_diarias_caja.txt'\n");
    presionar_enter_para_continuar();
     printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


void consultar_ventas_mensuales_caja() {
    if (cantidad_cajas == 0) {
        printf("No hay cajas registradas para consultar ventas mensuales por caja.\n");
        presionar_enter_para_continuar();
        return;
    }
     if (cantidad_facturas == 0) {
        printf("No hay facturas generadas para consultar ventas.\n");
        presionar_enter_para_continuar();
        return;
    }

    limpiar_consola();
    mostrar_todas_cajas(); // Muestra listado para seleccionar caja
    int numero_caja_consultar = obtener_entero("Ingrese el numero de caja para consultar ventas mensuales: ");
    int caja_index = buscar_caja_por_numero(numero_caja_consultar);
    if (caja_index == -1) {
        printf("No se encontro ninguna caja con el numero %d. Reporte cancelado.\n", numero_caja_consultar);
        presionar_enter_para_continuar();
        return;
    }

    time_t tiempo_actual = time(NULL);
    struct tm *fecha_actual_tm = localtime(&tiempo_actual);
    int mes_actual = fecha_actual_tm->tm_mon + 1; // Meses en tm struct van de 0 a 11
    int ano_actual = fecha_actual_tm->tm_year + 1900;

    float total_ventas_bs = 0;
    int cantidad_facturas_caja = 0;
    char reporte_mensual_caja[2000]; // Buffer para reporte en memoria
    sprintf(reporte_mensual_caja, "========= Reporte Ventas Mensuales Caja No %d - Mes: %02d/%d =========\n", cajas[caja_index].numero_caja, mes_actual, ano_actual);


    for (int i = 0; i < cantidad_facturas; i++) {
        if (facturas[i].numero_caja == numero_caja_consultar) {
            struct tm *fecha_factura_tm = localtime(&facturas[i].fecha);
            if (fecha_factura_tm->tm_mon + 1 == mes_actual && fecha_factura_tm->tm_year + 1900 == ano_actual) {
                total_ventas_bs += facturas[i].monto_total;
                cantidad_facturas_caja++;
            }
        }
    }

    sprintf(reporte_mensual_caja + strlen(reporte_mensual_caja), "Caja No: %d - Empleado Responsable: %s\n", cajas[caja_index].numero_caja, cajas[caja_index].nombre_empleado);
    sprintf(reporte_mensual_caja + strlen(reporte_mensual_caja), "Total Ventas del Mes: %.2f Bs\n", total_ventas_bs);
    sprintf(reporte_mensual_caja + strlen(reporte_mensual_caja), "Cantidad de Facturas Emitidas: %d\n", cantidad_facturas_caja);
    sprintf(reporte_mensual_caja + strlen(reporte_mensual_caja), "===================================================================\n");


    limpiar_consola();
    printf("%s", reporte_mensual_caja); // Imprime reporte en consola
    presionar_enter_para_continuar();

    guardar_reporte_txt("reporte_ventas_mensuales_caja.txt", reporte_mensual_caja); // Guarda reporte en archivo TXT
    printf("Reporte de ventas mensuales por caja guardado en 'reporte_ventas_mensuales_caja.txt'\n");
    presionar_enter_para_continuar();
     printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}

void mostrar_total_ventas_diarias() {
    if (cantidad_facturas == 0) {
        printf("No hay facturas generadas para mostrar el total de ventas diarias.\n");
        presionar_enter_para_continuar();
        return;
    }

    time_t tiempo_actual = time(NULL);
    struct tm *fecha_actual_tm = localtime(&tiempo_actual);
    int dia_actual = fecha_actual_tm->tm_mday;
    int mes_actual = fecha_actual_tm->tm_mon + 1;
    int ano_actual = fecha_actual_tm->tm_year + 1900;

    float total_ventas_bs = 0;
    char reporte_total_diario[2000]; // Buffer para reporte en memoria
    sprintf(reporte_total_diario, "========= Reporte Total Ventas Diarias - Fecha: %02d/%02d/%d =========\n", dia_actual, mes_actual, ano_actual);


    for (int i = 0; i < cantidad_facturas; i++) {
        struct tm *fecha_factura_tm = localtime(&facturas[i].fecha);
        if (fecha_factura_tm->tm_mday == dia_actual && fecha_factura_tm->tm_mon + 1 == mes_actual && fecha_factura_tm->tm_year + 1900 == ano_actual) {
            total_ventas_bs += facturas[i].monto_total;
        }
    }

    sprintf(reporte_total_diario + strlen(reporte_total_diario), "Total Ventas Diarias del Hipermercado: %.2f Bs\n", total_ventas_bs);
    sprintf(reporte_total_diario + strlen(reporte_total_diario), "===================================================================\n");


    limpiar_consola();
    printf("%s", reporte_total_diario); // Imprime reporte en consola
    presionar_enter_para_continuar();

    guardar_reporte_txt("reporte_total_ventas_diarias.txt", reporte_total_diario); // Guarda reporte en archivo TXT
    printf("Reporte de total de ventas diarias guardado en 'reporte_total_ventas_diarias.txt'\n");
    presionar_enter_para_continuar();
     printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }

}

void mostrar_total_ventas_mensuales() {
    if (cantidad_facturas == 0) {
        printf("No hay facturas generadas para mostrar el total de ventas mensuales.\n");
        presionar_enter_para_continuar();
        return;
    }
    time_t tiempo_actual = time(NULL);
    struct tm *fecha_actual_tm = localtime(&tiempo_actual);
    int mes_actual = fecha_actual_tm->tm_mon + 1;
    int ano_actual = fecha_actual_tm->tm_year + 1900;

    float total_ventas_bs = 0;
    char reporte_total_mensual[2000]; // Buffer para reporte en memoria
    sprintf(reporte_total_mensual, "========= Reporte Total Ventas Mensuales - Mes: %02d/%d =========\n", mes_actual, ano_actual);

    for (int i = 0; i < cantidad_facturas; i++) {
        struct tm *fecha_factura_tm = localtime(&facturas[i].fecha);
        if (fecha_factura_tm->tm_mon + 1 == mes_actual && fecha_factura_tm->tm_year + 1900 == ano_actual) {
            total_ventas_bs += facturas[i].monto_total;
        }
    }

    sprintf(reporte_total_mensual + strlen(reporte_total_mensual), "Total Ventas Mensuales del Hipermercado: %.2f Bs\n", total_ventas_bs);
    sprintf(reporte_total_mensual + strlen(reporte_total_mensual), "===================================================================\n");


    limpiar_consola();
    printf("%s", reporte_total_mensual); // Imprime reporte en consola
    presionar_enter_para_continuar();

    guardar_reporte_txt("reporte_total_ventas_mensuales.txt", reporte_total_mensual); // Guarda reporte en archivo TXT
    printf("Reporte de total de ventas mensuales guardado en 'reporte_total_ventas_mensuales.txt'\n");
    presionar_enter_para_continuar();
     printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


void mostrar_producto_mas_vendido() {
    if (cantidad_facturas == 0) {
        printf("No hay facturas generadas para determinar el producto mas vendido.\n");
        presionar_enter_para_continuar();
        return;
    }

    int ventas_productos[MAX_PRODUCTOS] = {0}; // Array para contar ventas por producto (index corresponde al indice del producto en array productos)
    int producto_mas_vendido_index = -1;
    int cantidad_mas_vendida = 0;

    for (int i = 0; i < cantidad_facturas; i++) {
        for (int j = 0; j < facturas[i].cantidad_productos_factura; j++) {
            int codigo_producto_factura = facturas[i].productos_factura[j].producto.codigo;
            int producto_index = buscar_producto_por_codigo(codigo_producto_factura);
            if (producto_index != -1) {
                ventas_productos[producto_index] += facturas[i].productos_factura[j].cantidad; // Acumula cantidad vendida por producto
            }
        }
    }

    for (int i = 0; i < cantidad_productos; i++) {
        if (ventas_productos[i] > cantidad_mas_vendida) {
            cantidad_mas_vendida = ventas_productos[i];
            producto_mas_vendido_index = i;
        }
    }

    char reporte_producto_mas_vendido[2000]; // Buffer para reporte en memoria
    sprintf(reporte_producto_mas_vendido, "========= Reporte Producto Mas Vendido =========\n");


    limpiar_consola();
    if (producto_mas_vendido_index != -1) {
        sprintf(reporte_producto_mas_vendido + strlen(reporte_producto_mas_vendido), "Producto mas vendido: %s (Codigo: %d)\n", productos[producto_mas_vendido_index].nombre, productos[producto_mas_vendido_index].codigo);
        sprintf(reporte_producto_mas_vendido + strlen(reporte_producto_mas_vendido), "Cantidad total vendida: %d unidades\n", cantidad_mas_vendida);

        printf("%s", reporte_producto_mas_vendido); // Imprime reporte en consola
    } else {
        sprintf(reporte_producto_mas_vendido + strlen(reporte_producto_mas_vendido), "No hay ventas registradas para determinar el producto mas vendido.\n");
        printf("%s", reporte_producto_mas_vendido);
    }
     sprintf(reporte_producto_mas_vendido + strlen(reporte_producto_mas_vendido), "===================================================================\n");


    presionar_enter_para_continuar();

    guardar_reporte_txt("reporte_producto_mas_vendido.txt", reporte_producto_mas_vendido); // Guarda reporte en archivo TXT
    printf("Reporte de producto mas vendido guardado en 'reporte_producto_mas_vendido.txt'\n");
    presionar_enter_para_continuar();
     printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}


void mostrar_cantidad_trabajadores_hipermercado() {
    limpiar_consola();
    printf("========= Cantidad de Trabajadores del %s =========\n", NOMBRE_HIPERMERCADO);
    printf("Cantidad total de trabajadores registrados: %d\n", cantidad_trabajadores);
    printf("===========================================================\n");
    presionar_enter_para_continuar();
     printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}

void mostrar_cantidad_clientes_con_compras() {
    if (cantidad_facturas == 0) {
        printf("No hay facturas generadas para determinar clientes con compras.\n");
        presionar_enter_para_continuar();
        return;
    }

    int clientes_con_compra[MAX_CLIENTES] = {0}; // Array para marcar clientes con compras (1 si tiene compra, 0 no)
    int cantidad_clientes_con_compras = 0;

    for (int i = 0; i < cantidad_facturas; i++) {
        int cedula_cliente_factura = facturas[i].cliente.cedula;
        int cliente_index = buscar_cliente_por_cedula(cedula_cliente_factura);
        if (cliente_index != -1) {
            clientes_con_compra[cliente_index] = 1; // Marca cliente con compra
        }
    }

    for (int i = 0; i < cantidad_clientes; i++) {
        if (clientes_con_compra[i] == 1) {
            cantidad_clientes_con_compras++;
        }
    }

    char reporte_clientes_con_compra[2000]; // Buffer para reporte en memoria
    sprintf(reporte_clientes_con_compra, "========= Reporte Clientes con Compras Realizadas =========\n");
    sprintf(reporte_clientes_con_compra + strlen(reporte_clientes_con_compra), "Cantidad de clientes con compras en %s: %d\n", NOMBRE_HIPERMERCADO, cantidad_clientes_con_compras);
     sprintf(reporte_clientes_con_compra + strlen(reporte_clientes_con_compra), "===================================================================\n");


    limpiar_consola();
    printf("%s", reporte_clientes_con_compra); // Imprime reporte en consola
    presionar_enter_para_continuar();

    guardar_reporte_txt("reporte_clientes_con_compras.txt", reporte_clientes_con_compra); // Guarda reporte en archivo TXT
    printf("Reporte de clientes con compras guardado en 'reporte_clientes_con_compras.txt'\n");
    presionar_enter_para_continuar();
     printf("¿Desea regresar al menu principal? (s/n): ");
    char regresar_menu;
    scanf(" %c", &regresar_menu);
    if (regresar_menu == 's' || regresar_menu == 'S') {
        mostrar_menu_principal(); // Regresa al menu principal directamente
    } else {
        return;
    }
}



// ========================= Funciones Auxiliares =========================


int obtener_entero(const char *mensaje) {
    int valor;
    printf("%s", mensaje);
    while (scanf("%d", &valor) != 1) {
        printf("Entrada no valida. Debe ingresar un numero entero. Intente de nuevo: ");
        while (getchar() != '\n'); // Limpiar buffer de entrada
    }
    while (getchar() != '\n'); // Limpiar buffer de entrada
    return valor;
}

float obtener_flotante(const char *mensaje) {
    float valor;
    printf("%s", mensaje);
    while (scanf("%f", &valor) != 1) {
        printf("Entrada no valida. Debe ingresar un numero. Intente de nuevo: ");
        while (getchar() != '\n'); // Limpiar buffer de entrada
    }
    while (getchar() != '\n'); // Limpiar buffer de entrada
    return valor;
}

void obtener_cadena(const char *mensaje, char *cadena, int longitud_maxima) {
    printf("%s", mensaje);
    if (fgets(cadena, longitud_maxima, stdin) != NULL) {
        cadena[strcspn(cadena, "\n")] = 0; // Elimina el salto de línea final
    } else {
        cadena[0] = 0; // Cadena vacía en caso de error de lectura
    }
}

void presionar_enter_para_continuar() {
    printf("Presione ENTER para continuar...");
    getchar(); // Espera a que se presione ENTER
}

int buscar_producto_por_codigo(int codigo) {
    for (int i = 0; i < cantidad_productos; i++) {
        if (productos[i].codigo == codigo) {
            return i; // Retorna el indice si encuentra el producto
        }
    }
    return -1; // Retorna -1 si no encuentra el producto
}

int buscar_cliente_por_cedula(int cedula) {
    for (int i = 0; i < cantidad_clientes; i++) {
        if (clientes[i].cedula == cedula) {
            return i; // Retorna el indice si encuentra el cliente
        }
    }
    return -1; // Retorna -1 si no encuentra el cliente
}

int buscar_trabajador_por_numero_empleado(int numero_empleado) {
    for (int i = 0; i < cantidad_trabajadores; i++) {
        if (trabajadores[i].numero_empleado == numero_empleado) {
            return i; // Retorna el indice si encuentra el trabajador
        }
    }
    return -1; // Retorna -1 si no encuentra el trabajador
}

int buscar_caja_por_numero(int numero_caja) {
    for (int i = 0; i < cantidad_cajas; i++) {
        if (cajas[i].numero_caja == numero_caja) {
            return i; // Retorna el indice si encuentra la caja
        }
    }
    return -1; // Retorna -1 si no encuentra la caja
}

int buscar_factura_por_numero(int numero_factura) {
    for (int i = 0; i < cantidad_facturas; i++) {
        if (facturas[i].numero_factura == numero_factura) {
            return i; // Retorna el indice si encuentra la factura
        }
    }
    return -1; // Retorna -1 si no encuentra la factura
}


void limpiar_consola() {
    // Funcion para limpiar la consola, funciona en sistemas operativos comunes
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void guardar_reporte_txt(const char *nombre_archivo, const char *contenido) {
    FILE *archivo_reporte = fopen(nombre_archivo, "w"); // Abre archivo en modo escritura
    if (archivo_reporte == NULL) {
        printf("Error al guardar el reporte en el archivo '%s'.\n", nombre_archivo);
        return;
    }
    fprintf(archivo_reporte, "%s", contenido); // Escribe el contenido en el archivo
    fclose(archivo_reporte); // Cierra el archivo
}

char* obtener_fecha_hora_actual() {
    time_t tiempo_actual_unix;
    struct tm* tiempo_info;
    char* fecha_hora_str = (char*)malloc(20 * sizeof(char)); // Reserva memoria para la cadena

    time(&tiempo_actual_unix); // Obtiene tiempo actual en segundos Unix
    tiempo_info = localtime(&tiempo_actual_unix); // Convierte a tiempo local

    // Formatea la fecha y hora en la cadena
    strftime(fecha_hora_str, 20, "%Y-%m-%d %H:%M:%S", tiempo_info);
    return fecha_hora_str; // Retorna puntero a la cadena con fecha y hora
}
