# Guía Técnica: Plugin InternetControl para Veyon 4 (Qt 6)

**Autor:** Iván Jiménez  
**GitHub:** [@ivanjimenez](https://github.com/ivanjimenez)

Este documento detalla el desarrollo, compilación y despliegue del plugin **InternetControl**, diseñado para proporcionar un control granular sobre el acceso a Internet en entornos educativos gestionados por Veyon.

## 1. Descripción General

El plugin **InternetControl** integra dos funciones principales en la interfaz de Veyon Master:

- **Bloqueo Total:** Intercepta y descarta todo el tráfico saliente por los puertos 80 (HTTP) y 443 (HTTPS).
- **Desbloqueo:** Restaura la conectividad eliminando las reglas del firewall.

Utiliza `iptables` en el lado del cliente (alumno) y se comunica a través del protocolo de mensajes de Veyon, ejecutándose con privilegios de superusuario a través del proceso `VeyonServer`.

---

## 2. Licencia y Derechos de Autor

**© 2024 Iván Jiménez. Todos los derechos reservados (All Rights Reserved).**

El código fuente de este desarrollo es propiedad exclusiva de su autor. Queda estrictamente prohibida la copia, reproducción, modificación, distribución o uso comercial del código fuente sin la autorización expresa y por escrito de Iván Jiménez.

> **Aviso Legal de Dependencias (GPL/LGPL):**
> Este software actúa como un plugin ('add-on') para la plataforma Veyon (licenciada bajo GPL) y hace uso del framework Qt 6 (licenciado bajo LGPLv3/GPLv3 en su edición Open Source). Cualquier distribución pública del archivo binario compilado (`.so` / `.dll`) debe realizarse cumpliendo con las obligaciones de dichas licencias subyacentes, lo cual generalmente exige permitir la ingeniería inversa para depuración y proporcionar el código fuente bajo demanda si el binario se distribuye a terceros. El uso interno en una institución no constituye distribución.

---

## 3. Arquitectura de Desarrollo

### Estructura del Proyecto

```text
internet-veyon-plugin/
├── CMakeLists.txt              # Configuración de compilación para Qt 6
├── InternetControl.json        # Metadatos del plugin para el sistema de carga
└── src/
    ├── InternetControlPlugin.h # Definición de la clase y macros Qt
    └── InternetControlPlugin.cpp # Implementación de la lógica y comandos
```

### Lógica de Control de Firewall (`clearRules`)

Para garantizar la estabilidad del sistema del alumno, se implementó una función lambda de limpieza que evita la acumulación de reglas redundantes en la cadena `OUTPUT`:

```cpp
auto clearRules = []() {
    while (QProcess::execute("/usr/sbin/iptables", {"-D", "OUTPUT", "-p", "tcp", "--dport", "80", "-j", "DROP"}) == 0);
    while (QProcess::execute("/usr/sbin/iptables", {"-D", "OUTPUT", "-p", "tcp", "--dport", "443", "-j", "DROP"}) == 0);
};
```

_Esta lógica asegura que, sin importar cuántas veces se envíe la orden de bloqueo, el estado final del firewall sea siempre predecible y limpio._

---

## 4. Proceso de Compilación

### Requisitos Previos

- **Veyon SDK**: Cabeceras (`.h`) ubicadas en la ruta de desarrollo.
- **Qt 6.x**: Librerías de desarrollo (`qt6-base-dev`).
- **CMake**: Versión 3.16 o superior.

### Comandos de Compilación

Desde la raíz del proyecto:

```bash
# Crear y entrar al directorio de construcción
mkdir -p build && cd build

# Configurar el proyecto definiendo las rutas a Qt6 y Veyon
cmake ..

# Compilar el módulo (genera internetcontrol.so)
make -j$(nproc)
```

---

## 5. Despliegue e Instalación

### A. Configuración en el Master (Profesor)

El Master requiere el binario para registrar los nuevos botones en la barra de herramientas.

1.  **Instalación:**
    ```bash
    sudo cp internetcontrol.so /usr/lib/x86_64-linux-gnu/veyon/
    ```
2.  **Registro:** Reiniciar el servicio para forzar la lectura de metadatos:
    ```bash
    sudo systemctl restart veyon
    ```

### B. Configuración en el Cliente (Alumno)

El cliente requiere el binario para procesar los `FeatureMessage` recibidos por la red.

1.  **Transferencia:**
    ```bash
    scp internetcontrol.so admin@IP_ALUMNO:/home/admin/
    ```
2.  **Instalación y Limpieza:**
    ```bash
    ssh admin@IP_ALUMNO "sudo mv /home/admin/internetcontrol.so /usr/lib/x86_64-linux-gnu/veyon/ && \
                         sudo systemctl restart veyon && \
                         sudo iptables -F OUTPUT"
    ```

---

## 6. Protocolo de Pruebas y Verificación

### Prueba 1: Carga del Plugin (Master)

Ejecutar el Master en modo debug para confirmar que no hay errores de IID:

```bash
veyon-master --debug 2>&1 | grep -i "InternetControl"
```

_Éxito: El log debe mostrar el registro de la interfaz `FeatureProviderInterface`._

### Prueba 2: Verificación de Tráfico (Cliente)

En la terminal del alumno, monitorizar el firewall:

```bash
sudo watch -n 1 "iptables -L OUTPUT -n -v --line-numbers"
```

| Acción en Master    | Resultado Esperado en Cliente                                                       |
| :------------------ | :---------------------------------------------------------------------------------- |
| Pulsar **Bloquear** | Aparecen 2 reglas DROP (80, 443). El contador de bytes aumenta al intentar navegar. |
| Pulsar **Permitir** | La cadena OUTPUT queda vacía. La navegación se restaura al instante.                |

---

## 7. Resolución de Problemas (Troubleshooting)

- **El botón no aparece:** Verificar que la macro `Q_PLUGIN_METADATA` reside en el archivo `.h` y que el IID coincide con `io.veyon.Veyon.PluginInterface`. Asegurarse también de que en el archivo `.json` del plugin se define correctamente bajo la interfaz pertinente.
- **Error de permisos en iptables:** Asegurarse de que el plugin esté implementado en la función `handleFeatureMessage` que recibe el parámetro `VeyonServerInterface`, ya que es el único proceso de Veyon que cuenta con privilegios de root nativos en el sistema huésped.
- **Reglas duplicadas:** Si aparecen múltiples reglas, verificar que la compilación actual incluye la lambda de limpieza con el comando `-D` de `iptables`.
