#!/bin/bash -e

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                         #
#  Copyright (C) 2019 Simon Stürz <stuerz.simon@gmail.com>                #
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

# Directories
SOURCE_DIR=$(cd $(dirname $0); cd ..; pwd)
DATA_DIR=${SOURCE_DIR}/data
DATA_BACKUP_DIR=${SOURCE_DIR}/data-backup
RESOURCES_DIR=${SOURCE_DIR}/resources

# Colors
BASH_GREEN="\e[1;32m"
BASH_RED="\e[1;31m"
BASH_NORMAL="\e[0m"

#---------------------------------------------------------------
function printGreen() {
    echo -e "${BASH_GREEN}[+] $1${BASH_NORMAL}"
}

#---------------------------------------------------------------
function printRed() {
    echo -e "${BASH_RED}[!] $1${BASH_NORMAL}"
}

#---------------------------------------------------------------
# Main
#---------------------------------------------------------------
printGreen "Source: ${SOURCE_DIR}"
printGreen "Data: ${DATA_DIR}"
printGreen "Resources: ${RESOURCES_DIR}"

printGreen "Backup current data directory ${DATA_DIR}"
if [ -d ${DATA_BACKUP_DIR} ]; then rm -rf ${DATA_BACKUP_DIR}; fi
if [ -d ${DATA_DIR} ]; then mv ${DATA_DIR} ${DATA_BACKUP_DIR}; fi


printGreen "Create data directory ${DATA_DIR}"
mkdir -p ${DATA_DIR}

cd ${RESOURCES_DIR}
ls -l
