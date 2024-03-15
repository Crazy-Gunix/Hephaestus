#!/bin/bash

meson setup builddir --reconfigure
meson compile -C builddir

