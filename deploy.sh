#!/bin/bash
echo "🚀 Compilando el plugin de Veyon..."

# Crear carpeta de build si no existe
mkdir -p build
cd build

# Ejecutar CMake y Make
cmake ..
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "✅ Compilación exitosa. Desplegando en el sistema..."
    
    # Ruta donde Ubuntu instala los plugins de Veyon por defecto
    PLUGIN_DIR="/usr/lib/x86_64-linux-gnu/veyon"
    
    # Copiamos el plugin
    sudo cp libinternetcontrol.so $PLUGIN_DIR/
    
    # Reiniciamos el servicio de Veyon para que los Workers lo detecten
    sudo systemctl restart veyon.service
    
    echo "🎉 ¡Despliegue completado! Ya puedes abrir veyon-master."
else
    echo "❌ Error en la compilación. Revisa el código."
fi
