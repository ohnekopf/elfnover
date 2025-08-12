CFLAGS += -Wall
CFLAGS += -MMD
CFLAGS += -fno-strict-aliasing



SRC = $(wildcard  *.c)

OBJS= $(patsubst %.c,%.o,${SRC})

DEPS = $(patsubst %.o,%.d,${OBJS})

elfnover: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@ 


${BUILDDIR}/%.o: %.c 
	${CC} ${CFLAGS} -c $<  -o $@ 


.PHONY: clean showdeps echo
clean:
	${RM} main ${OBJS} ${DEPS}

-include ${DEPS}

showdeps:
	@cat ${DEPS} 2>&- || true

