# SPDX-License-Identifier: (LGPL-2.1 OR BSD-2-Clause)

CLANG ?= clang-14
LLVM_STRIP ?= llvm-strip

LIBBPF_TOP = ~/eBPF/libbpf
BPFTOOL ?= /usr/local/sbin/bpftool

ARCH := $(shell uname -m | sed 's/x86_64/x86/' | sed 's/aarch64/arm64/' | sed 's/ppc64le/powerpc/' | sed 's/mips.*/mips/')

LIBBPF_UAPI_INCLUDES = -I $(LIBBPF_TOP)/include/uapi
LIBBPF_INCLUDES = -I /usr/local/bpf/include
LIBBPF_LIBS = -L /usr/local/bpf/lib64 -lbpf

INCLUDES = $(LIBBPF_UAPI_INCLUDES) $(LIBBPF_INCLUDES)

CLANG_BPF_SYS_INCLUDES = $(shell $(CLANG) -v -E - </dev/null │ l 2>&1 | sed -n '/<...> search starts here:/,/End of search | list./{ s| \(/.*\)|-idirafter \1|p }')

all: build

build: helloworld

helloworld.bpf.o: helloworld.bpf.c
	$(CLANG) -g -O2 -target bpf -D__TARGET_ARCH_$(ARCH) $(INCLUDES) $(CLANG_BPF_SYS_INCLUDES) -c helloworld.bpf.c

helloworld.skel.h: helloworld.bpf.o
	$(BPFTOOL) gen skeleton helloworld.bpf.o > helloworld.skel.h

helloworld: helloworld.skel.h helloworld.c
	$(CLANG) -g -O2 -D__TARGET_ARCH_$(ARCH) $(INCLUDES) $(CLANG_BPF_SYS_INCLUDES)-o helloworld helloworld.c $(LIBBPF_LIBS) -lbpf -lelf -lz
