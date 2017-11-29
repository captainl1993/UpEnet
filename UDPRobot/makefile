TARGET= ./../x64/Robot

CFLAGS :=-Wall -Wno-unused -Wno-sign-compare  -ggdb3 -g -Wno-deprecated -std=c++11 \
	 -I ../enet\
	 -I ../include\
	 -I ../include/libgo/linux\
	 -I ../include/libgo\


cppfiles := $(shell ls *.cpp  )
cfiles := $(-shell ls *.c )

OBJS := $(patsubst %.cpp,../obj/%.o, $(cppfiles) $(cfiles))
COBJS=${patsubst %.c,../obj/%.o,$(cfiles)}
CXXOBJS=${patsubst %.cpp,../obj/%.o,$(cppfiles)}

DEPS=$(patsubst %.o,%.d,$(OBJS))
LIB64 = -L /usr/lib64  -lpthread  -lssl -lcrypto -lz -ldl  -lboost_thread \
	../lib/enet.a\
	../lib/liblibgo.a\

SO_LIB=../lib/liblibgo.so


.PHONY: all clean

all: ${TARGET}

${TARGET}: ${OBJS} ${LIB} 
	${CXX} -o $@ $^ ${LDFLAGS} ${SO_LIB} ${LIB} ${LIB64}

${CXXOBJS}:../obj/%.o:./%.cpp
	${CXX} -MMD -c -o $@ $< ${CFLAGS} 

${COBJS}:../obj/%.o:./%.c
	${CC} -MMD -c -o $@ $< ${CFLAGS} 

-include $(DEPS)

clean:
	rm -f ${OBJS} ${TARGET} ${DEPS}

show:
	@echo GPROF=$(GPROF)
	@echo CFLAGS=$(CFLAGS)
	@echo LDFLAGS=$(LDFLAGS)
	@echo objs=$(OBJS)
	@echo cppfiels=$(cppfiles)
	@echo cfiels=$(cfiles)
	@echo DEPS=$(DEPS)
	@echo CXXOBJS=$(CXXOBJS)
	@echo COBJS=$(COBJS)

