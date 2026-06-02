#!/bin/bash
set -e

echo "Construyendo imagen Docker..."
docker build -t qplanif-appimage .

echo "Extrayendo AppImage..."
docker run --rm qplanif-appimage > qplanif
chmod +x qplanif

echo ""
echo "Listo. Ejecuta con:  ./qplanif"
