# run with "make" or "make PYTHON=python3" on ubuntu

PYTHON := python
CC := gcc
CFLAGS += -O3 -fPIC -std=c99 -Wall -Wextra  
# compile with the system Python:
INCDIRS += -I $(shell $(PYTHON) -m site --user-site)/numpy/
INCDIRS += -I $(shell $(PYTHON) -m site --user-site)/
INCDIRS += -I $(shell $(PYTHON) -m site --user-site)/numpy/core/include/
INCDIRS += -I $(shell $(PYTHON) -c 'import sysconfig; print(sysconfig.get_paths()["include"])')
INCDIRS += -I /usr/include/numpy/
# INCDIRS = -I /opt/ciao-4.6/ots/lib/python2.7/site-packages/numpy/core/include/ -I /opt/ciao-4.6/ots/include/python2.7/

all: npyinterp.so
	@echo "done: $@"
allplatforms: npyinterp64.so npyinterp32.so
	@echo "done: $@"

%.so: %.c
	${CC} ${CFLAGS} $< -o $@ -shared ${INCDIRS}
%64.so: %.c 
	${CC} ${CFLAGS} -m64 $< -o $@ -shared ${INCDIRS}
%32.so: %.c 
	${CC} ${CFLAGS} -m32 $< -o $@ -shared ${INCDIRS}

clean: 
	rm *.so

.PHONY: all allplatforms clean
