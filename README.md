# PlayStation 5 Open Source Software - Massive Collection

This archive contains a consolidated collection of open-source software source code used in the PlayStation 5.

## Contents

### 1. Sony-Hosted Source Code (`/sony_hosted`)
These are the files directly provided by Sony Interactive Entertainment on their official PS5 OSS portal.
- **WebKit**: Multiple versions for different PS5 system firmware.
- **WebKit - JavaScriptCore**: The JavaScript engine source code.
- **FFmpeg**: The specific version used by Sony for PS5.
- **Eigen**: C++ template library for linear algebra.
- **Cairo**: 2D graphics library.
- **Public Suffix List**: Domain name suffix data.

### 2. External Source Code (`/external`)
These are the source codes for major components that Sony uses in the PS5 but does not host directly. These have been fetched from official archives based on the versions identified for the PS5.
- **FreeBSD 11.0 Kernel & Userland**: The foundation of the PS5 operating system.
- **Lua 5.3.6**: Scripting engine.
- **OpenSSL 1.1.1w**: Cryptography library.
- **zlib 1.2.11**: Compression library.
- **SQLite 3.33.0**: Database engine.

### 3. Documentation (`/docs`)
- **OSS Notices**: License information and acknowledgments.

## Disclaimer
This collection is for educational and research purposes. All software is subject to its respective original licenses (GPL, LGPL, BSD, MIT, MPL, etc.). Sony's proprietary modifications to the FreeBSD kernel and other components are NOT included unless explicitly released by Sony in the `/sony_hosted` directory.
