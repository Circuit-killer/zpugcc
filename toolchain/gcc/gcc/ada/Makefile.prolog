# Makefile included at the beginning of the makefiles generated by gpr2make
# to support compilation for multiple languages.
# See also Makefile.generic
#
#   Copyright (C) 2001-2004 Free Software Foundation, Inc.

# This file is part of GCC.

# GCC is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# GCC is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with GCC; see the file COPYING.  If not, write to
# the Free Software Foundation, 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.

# all reserved variables are saved in <VAR>.saved

BASE_DIR.saved := $(BASE_DIR)
C_EXT.saved:=$(C_EXT)
CXX_EXT.saved:=$(CXX_EXT)
OBJ_EXT.saved:=$(OBJ_EXT)
SRC_DIRS.saved:=$(SRC_DIRS)
C_SRCS.saved:=$(C_SRCS)
CXX_SRCS.saved:=$(CXX_SRCS)
OBJ_DIR.saved:=$(OBJ_DIR)
LANGUAGES.saved:=$(LANGUAGES)
CC.saved:=$(CC)
CXX.saved:=$(CXX)
AR_CMD.saved:=$(AR_CMD)
AR_EXT.saved:=$(AR_EXT)
GNATMAKE.saved:=$(GNATMAKE)
ADAFLAGS.saved:=$(ADAFLAGS)
CFLAGS.saved:=$(CFLAGS)
CXXFLAGS.saved:=$(CXXFLAGS)
LIBS.saved:=$(LIBS)
LDFLAGS.saved:=$(LDFLAGS)
ADA_SOURCES.saved:=$(ADA_SOURCES)
EXEC.saved:=$(EXEC)
EXEC_DIR.saved:=$(EXEC_DIR)
MAIN.saved:=$(MAIN)
PROJECT_FILE.saved:=$(PROJECT_FILE)
DEPS_PROJECTS.saved:=$(DEPS_PROJECTS)

# Default settings

LANGUAGES:=ada
C_EXT:=.c
CXX_EXT:=.cc
AR_EXT=.a
OBJ_EXT=.o
CC=gcc

# Default target is to build (compile/bind/link)
# Target build is defined in Makefile.generic

default: build

