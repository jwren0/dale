## User configuration ##
CC ?= cc
CCFLAGS ?= -march=native -O2 -pipe

BIN_DIR ?= /usr/bin

EXEC := dale


## Developer configuration ##
CCFLAGS := $(CCFLAGS) -Wall -Wextra -Werror -Wformat-security \
		-Wpedantic -pedantic-errors -std=c18 \
		$(shell pkg-config --cflags --libs libpq)

SRC_FILES := $(shell find src/ -name "*.c")
OBJ_FILES := ${SRC_FILES:.c=.o}


## User targets ##
build: $(EXEC)
build_debug: $(EXEC)_debug

clean:
	@rm -f $(OBJ_FILES) $(EXEC) $(EXEC)_debug

install:
	@install -m 755 -v $(EXEC) $(BIN_DIR)/$(EXEC)

uninstall:
	@rm -f $(BIN_DIR)/$(EXEC)


## Developer targets ##
$(EXEC): $(OBJ_FILES)
	@printf "CC      $@\n"
	@$(CC) $(CCFLAGS) -Xlinker --strip-all -o $@ $^

$(EXEC)_debug: $(OBJ_FILES)
	@printf "CC      $@\n"
	@$(CC) $(CCFLAGS) -g -o $@ $^

src/%.o: src/%.c src/%.h
	@printf "CC      $@\n"
	@$(CC) $(CCFLAGS) -c -g -o $@ $<

src/%.o: src/%.c
	@printf "CC      $@\n"
	@$(CC) $(CCFLAGS) -c -g -o $@ $<
