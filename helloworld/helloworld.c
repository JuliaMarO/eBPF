#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <bpf/libbpf.h>
#include "helloworld.skel.h"

static int libbpf_print_fn(enum libbpf_print_level level, const char *format, va_list args) {
	return vfprintf(stderr, format, args);
}

int main(int argc, char **argv) {
	struct helloworld_bpf *skel;
	int err;

	libbpf_set_strict_mode(LIBBPF_STRICT_ALL);
	libbpf_set_print(libbpf_print_fn);

	skel = helloworld_bpf__open();
	if (!skel) {
		fprintf(stderr, "failed to open skel\n");
		return 1;
	}

	err = helloworld_bpf__load(skel);
	if (err) {
		fprintf(stderr, "failed to load/verify skel\n");
		goto cleanup;
	}

	err = helloworld_bpf__attach(skel);
	if (err) {
		fprintf(stderr, "failed to attach skel\n");
		goto cleanup;
	}

	printf("yay!!!!!\n");

	for(;;) {
		fprintf(stderr, ".");
		sleep(1);
	}

cleanup:
	helloworld_bpf__destroy(skel);
	return -err;
}
