TARGET := riscv64-unknown-linux-gnu
CC := $(TARGET)-gcc
LD := $(TARGET)-gcc
OBJCOPY := $(TARGET)-objcopy
DEBUG_FLAGS := -DCKB_C_STDLIB_PRINTF
CFLAGS := -O3 -fPIC -fno-builtin-printf -nostdinc -nostdlib -nostartfiles -fvisibility=hidden  -DWORDSIZE=64 -D__unix__ -DWITH_STDLIB -I deps/libecc -I deps/libecc/src -I deps/libecc/src/external_deps -I . -I deps/ckb-c-stdlib -I deps/ckb-c-stdlib/libc -I deps/ckb-c-stdlib/molecule -Wall -Werror -Wno-nonnull -Wno-nonnull-compare -Wno-unused-function -g
#CFLAGS := -Os -fPIC -fPIE -pie -fno-builtin-printf -nostdinc -nostdlib -nostartfiles -fvisibility=hidden -DCKB_DECLARATION_ONLY -DWORDSIZE=64 -D__unix__ -DWITH_STDLIB -I . -I deps/ckb-c-stdlib -I deps/ckb-c-stdlib/libc -I deps/ckb-c-stdlib/molecule -Wall -Werror -Wno-nonnull -Wno-nonnull-compare -Wno-unused-function -Wno-unused-variable -g
LDFLAGS := -Wl,-static -fdata-sections -ffunction-sections -Wl,--gc-sections


SECP256R1_DEP := deps/libecc/build/libsign.a
PASSED_R1_CFLAGS := -O3 -fPIC -nostdinc -nostdlib -Wno-unused-result -DCKB_DECLARATION_ONLY -DWORDSIZE=64 -D__unix__ -DWITH_STDLIB  -fdata-sections -ffunction-sections -I ../ckb-c-stdlib/libc
#CFLAGS_R1 := -fPIC -Os -fno-builtin-printf -nostdinc -nostdlib -nostartfiles -fvisibility=hidden -fdata-sections -ffunction-sections -I deps/libecc -I deps/libecc/src -I deps/libecc/src/external_deps -I deps/ckb-c-stdlib -I deps/ckb-c-stdlib/molecule -I deps/ckb-c-stdlib/libc -Wall -Werror -Wno-nonnull -Wno-nonnull-compare -Wno-unused-function -DWORDSIZE=64 -D__unix__ -DWITH_STDLIB -I deps/secp256k1/src -I deps/secp256k1
#LDFLAGS_R1 := -Wl,-static -Wl,--gc-sections


# docker pull nervos/ckb-riscv-gnu-toolchain:gnu-bionic-20191012
BUILDER_DOCKER := nervos/ckb-riscv-gnu-toolchain@sha256:aae8a3f79705f67d505d1f1d5ddc694a4fd537ed1c7e9622420a470d59ba2ec3

all-via-docker:
	docker run --rm -v `pwd`:/code ${BUILDER_DOCKER} bash -c "cd /code && make all"
	cp dispatch.release ./build/release/dispatch

debug-all-via-docker:
	docker run --rm -v `pwd`:/code ${BUILDER_DOCKER} bash -c "cd /code && make debug_all"
	cp dispatch.debug ./build/debug/dispatch


all: dispatch.release


debug_all: dispatch.debug


dispatch.debug: dispatch.c $(SECP256R1_DEP)
	$(CC) $(CFLAGS) $(LDFLAGS) $(DEBUG_FLAGS) $< $(SECP256R1_DEP) deps/libecc/src/external_deps/rand.c deps/libecc/src/external_deps/print.c -o $@
	$(OBJCOPY) --only-keep-debug $@ $@.debug
	$(OBJCOPY) --strip-debug --strip-all $@

dispatch.release: dispatch.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<


$(SECP256R1_DEP):
	cd deps/libecc && \
	CC=$(CC) LD=$(LD) CFLAGS="${PASSED_R1_CFLAGS}" COMPLETE=0 LADDER=0 SMALLSTACK=1 make 64
	#CC=$(CC) LD=$(LD) CFLAGS="${PASSED_R1_CFLAGS}" BLINDING=0 COMPLETE=0 make 64

