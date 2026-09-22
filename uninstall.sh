# SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
# SPDX-License-Identifier: BSD-2-Clause

#!/bin/bash

set -euo pipefail

#Check if the build directory exists
if [ ! -d "build" ]; then
    echo "Error: Build directory not found." >&2 
    echo "If you installed Maki, run this script from the project root (right next to the build folder)!" >&2
    exit 1
fi
    echo "==> Uninstalling Maki..."
    sudo cmake --build build/ --target uninstall
    
    echo "==> Maki was uninstalled successfully!"