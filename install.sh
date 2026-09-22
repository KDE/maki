# SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
# SPDX-License-Identifier: BSD-2-Clause

#!/bin/bash

set -euo pipefail

BUILD_TYPE="${1:-Release}"

echo "==> Configuring Maki in $BUILD_TYPE mode..."
cmake -B build/ -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

echo "==> Building Maki..."
cmake --build build/

echo "==> Installing Maki..."
sudo cmake --install build/

echo "==> Maki was installed successfully!"