.PHONY: all

TARGET := tinatest
SRCS := \
	$(TARGET:%=%.c) \
	task.c \
	shmem.c

all: $(TARGET)

$(TARGET):
	@echo -e "\033[32mBuilding Core: $@\033[0m"
	@$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o $(PREFIX)/bin/tinatest
