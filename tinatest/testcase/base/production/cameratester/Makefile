TARGET = cameratester
INCLUDES += -I$(STAGING_DIR)/usr/include/allwinner/include -I$(STAGING_DIR)/usr/include/allwinner
LIBS +=  -lpthread -ldl -lrt

SRCS = cameratester.c add_water.c water_mark.c convert.c
OBJS = $(SRCS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) $(LDFLAGS)
	cp $(TARGET) $(PREFIX)
	cp -rf ./wm_res $(PREFIX_DATA)

all:$(TARGET)

clean:
	rm -rf $(TARGET) *.o *.a *~
	cd common && rm -f *.o *.a *.bak *~ .depend
