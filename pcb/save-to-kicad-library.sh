#!/bin/bash -e

PROJECTNAME="$(basename -s .pro $(ls *.pro))"
cp mods/* ../../kicad-library/mods/
kicad-split --yes -i ${PROJECTNAME}.lib -o ../../kicad-library/libs/
