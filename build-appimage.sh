#!/bin/bash
set -e

echo "Construyendo imagen Docker..."
docker build --no-cache --progress=plain -t qplanif-appimage . 2>&1 | tee build.log
echo "Extrayendo AppImage..."
docker run --rm qplanif-appimage > qplanif
chmod +x qplanif

echo ""
echo "Listo. Ejecuta con:  ./qplanif"
echo "Log completo guardado en: build.log"
