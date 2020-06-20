#!/bin/bash -e

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                         #
#  Copyright (C) 2019 Simon Stürz <stuerz.simon@gmail.com>                #
#                                                                         #
#  This file is part of selffinding-chronicles                            #
#                                                                         #
#  This program is free software; you can redistribute it and/or          #
#  modify it under the terms of the GNU General Public License            #
#  as published by the Free Software Foundation; either version 2         #
#  of the License, or (at your option) any later version.                 #
#                                                                         #
#  This program is distributed in the hope that it will be useful,        #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of         #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          #
#  GNU General Public License for more details.                           #
#                                                                         #
#  You should have received a copy of the GNU General Public License      #
#  along with guh. If not, see <http://www.gnu.org/licenses/>.            #
#                                                                         #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Note: sudo apt-get install imagemagick krita

# Directories
SOURCE_DIR=$(cd $(dirname $0); cd ..; pwd)
DATA_DIR=${SOURCE_DIR}/data
DATA_BACKUP_DIR=${SOURCE_DIR}/data-backup
RESOURCES_DIR=${SOURCE_DIR}/resources

IGNORE_EXTENSIONS=("kra~" "bundle" "sh")

# Colors
BASH_GREEN="\e[1;32m"
BASH_RED="\e[1;31m"
BASH_NORMAL="\e[0m"

# Image scaling
GRID_UNIT_PX=500


#---------------------------------------------------------------
function printGreen() {
    echo -e "${BASH_GREEN}[+] $1${BASH_NORMAL}"
}

#---------------------------------------------------------------
function printRed() {
    echo -e "${BASH_RED}[!] $1${BASH_NORMAL}"
}

#---------------------------------------------------------------
function processDirectory() {
    printGreen "Process directory $1 relative to ${RELATIVE_PATH}"
    local currentPath=${RELATIVE_PATH}
    RELATIVE_PATH+="/$1"
    echo "--> Relative path: ${RELATIVE_PATH}"
    cd $1

    if [ -z "$(ls -A)" ]; then
        echo "$RELATIVE_PATH is empty"
    else
        for entry in *; do
            if [ -d $entry ]; then
                processDirectory $entry
            fi

           if [ -f $entry ]; then
               processFile $entry
           fi
        done
    fi

    cd ..
    RELATIVE_PATH=${currentPath}
}

#---------------------------------------------------------------
function processFile() {
    local fileExtension=${1##*.}
    local fileName=${1%.*}

    if [ ${fileExtension} == "bundle" ]; then return; fi
    if [ ${fileExtension} == "kra~" ]; then return; fi
    if [ ${fileName} == "template" ]; then return; fi

    # we can copy svg files without modification
    if [ ${fileExtension} == "svg" ]; then
        echo "TODO: copy svg to data directory"
    fi


    # krita files must be exported as png

    if [ ${fileExtension} == "kra" ]; then
        exportDrawingToPng $1
    fi

    printGreen "--> Process file $1 - $fileName $fileExtension"

    TARGETFOLDER=$(readlink -f ${DATA_DIR}/$RELATIVE_PATH)
    printGreen "Copy to data folder $TARGETFOLDER"
}

#---------------------------------------------------------------
function exportDrawingToPng() {
    OUTPUT_PNG=$(basename "$1" .kra).png
    printGreen "Export $1 to ${OUTPUT_PNG}"
    krita $1 --export --export-filename ${OUTPUT_PNG} &> /dev/null || true
    file ${OUTPUT_PNG}
}

#---------------------------------------------------------------
# Main
#---------------------------------------------------------------
printGreen "Source: ${SOURCE_DIR}"
printGreen "Data: ${DATA_DIR}"
printGreen "Resources: ${RESOURCES_DIR}"

RELATIVE_PATH="."

cd ${RESOURCES_DIR}
for entry in *; do
    if [ -d $entry ]; then
        processDirectory $entry
    fi

    if [ -f $entry ]; then
       processFile $entry
    fi
done
