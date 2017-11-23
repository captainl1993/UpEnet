TARGET = ../lib/enet.a
OBJPATH = ../obj

CFLAGS =  -std=c++11 -g -O0  -Wno-deprecated-declarations -Wno-deprecated\

LFLAGS =  -lpthread  -lssl -lcrypto -lz -ldl \
 
LLIB = 

OBJS = $(patsubst %.cpp,$(OBJPATH)/%.o, $(notdir $(wildcard *.cpp)))
OBJCS= $(patsubst %.c,$(OBJPATH)/%.o, $(notdir $(wildcard *.c)))

$(OBJPATH)/%.o : %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

$(OBJPATH)/%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

all : $(TARGET)

$(TARGET) : $(OBJS) $(OBJCS)
	$(AR) -rcs $(OBJPATH)/$(TARGET) $(OBJS) $(OBJCS)

install:
	@cp $(OBJPATH)/$(TARGET) $(LIBPATH) $(LFLAGS)
	
clean:
	@rm -f $(OBJPATH)/*.o
	@rm -f $(TARGET)
	
	
	

