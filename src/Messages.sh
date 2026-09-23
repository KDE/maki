#!/usr/bin/env bash

# SPDX-FileCopyrightText: 2026 Alex Huber <dev.flu7x@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

$XGETTEXT `find . -name  \*.cpp -o -name \*.h -o -name \*.qml` -o $podir/maki.pot