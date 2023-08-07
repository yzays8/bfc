SRCDIR = src
FILE_PATH = $(abspath $(if $(filter /%,$(FILE)),$(FILE),$(dir $(lastword $(MAKEFILE_LIST)))$(FILE)))

.PHONY: all
all:
	make all -C $(SRCDIR) --no-print-directory

.PHONY: clean
clean:
	make clean -C $(SRCDIR) --no-print-directory

.PHONY: run
run:
	@make run FILE=$(FILE_PATH) -C $(SRCDIR) --no-print-directory

.PHONY: runc
runc:
	@make runc FILE=$(FILE_PATH) -C $(SRCDIR) --no-print-directory
