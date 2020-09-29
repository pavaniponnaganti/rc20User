# subdirs is synonymous to MSVS solution, this file define the different projects
TEMPLATE = subdirs

SUBDIRS = libwebserver console

# specify the project file for each subdirectory
console.subdir = console
libwebserver.subdir = libwebserver

# define the each project dependencies
console.depends = libwebserver

include ($$PWD/webserver.pri)
