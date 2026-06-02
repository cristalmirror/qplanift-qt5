FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

# Dependencias de compilación
RUN apt-get update && apt-get install -y \
        build-essential \
        qtbase5-dev \
        qt5-qmake \
        qtchooser \
        flex \
        bison \
        libfl-dev \
        wget \
        file \
        libglib2.0-0 \
        imagemagick \
    && rm -rf /var/lib/apt/lists/*

# ── Compilar ──────────────────────────────────────────────────────────────────

WORKDIR /src
COPY src/ .

RUN bison input.y -o input_tab.c \
 && flex -o linput.c linput.l \
 && cp input_tab.h input.tab.h \
 && qmake qplanif.pro \
 && make -j$(nproc)

# ── Estructura AppDir ─────────────────────────────────────────────────────────

RUN mkdir -p /AppDir/usr/bin \
             /AppDir/usr/share/applications \
             /AppDir/usr/share/icons/hicolor/256x256/apps \
             /AppDir/usr/share/qplanif

RUN cp qplanif /AppDir/usr/bin/

# Archivos de ayuda incluidos en el AppImage
RUN cp *.help /AppDir/usr/share/qplanif/ 2>/dev/null || true

# Icono de aplicación
RUN convert -size 256x256 xc:#4682b4 -fill white \
        -gravity Center -pointsize 64 -annotate 0 "Q" \
        /AppDir/usr/share/icons/hicolor/256x256/apps/qplanif.png

# .desktop requerido por linuxdeploy
RUN printf '[Desktop Entry]\nName=Qplanif\nComment=Simulador de planificacion de CPU\nExec=qplanif\nIcon=qplanif\nType=Application\nCategories=Education;Science;\n' \
        > /AppDir/usr/share/applications/qplanif.desktop

# ── Descargar linuxdeploy ─────────────────────────────────────────────────────
# APPIMAGE_EXTRACT_AND_RUN=1 permite ejecutar AppImages sin FUSE en Docker

WORKDIR /tools

RUN wget -q -O linuxdeploy.AppImage \
        https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage \
 && wget -q -O linuxdeploy-plugin-qt.AppImage \
        https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage \
 && chmod +x linuxdeploy.AppImage linuxdeploy-plugin-qt.AppImage

ENV APPIMAGE_EXTRACT_AND_RUN=1
ENV PATH="/tools:$PATH"
ENV QMAKE=/usr/bin/qmake

# ── Construir AppImage ────────────────────────────────────────────────────────

RUN mkdir -p /output \
 && OUTPUT=/output/qplanif \
    linuxdeploy.AppImage --appdir /AppDir --plugin qt --output appimage

CMD ["cat", "/output/qplanif"]
