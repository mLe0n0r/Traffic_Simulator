# ================== Config ==================
CC       := gcc
CFLAGS   := -Wall -Wextra -O2 -std=c11 -I../include
LDFLAGS  := -mconsole
BUILD    := .build

# Estrutura de pastas
SRC_DIR  := ../src
INC_DIR  := ../include

# Fontes
LIST_SRC := $(SRC_DIR)/linked-list.c
LAB1A    := $(SRC_DIR)/Lab1a.c
LAB1B    := $(SRC_DIR)/Lab1b.c

# Artefactos gerados
LIBSRC   := $(BUILD)/linked-list_lib.c
LIBOBJ   := $(BUILD)/linked-list.o
LAB1AOBJ := $(BUILD)/Lab1a.o
LAB1BOBJ := $(BUILD)/Lab1b.o

# Alvos padrão
.PHONY: all clean distclean
all: demo_list lab1a lab1b

# ============ Executáveis ============

# 1) Demo com o main de teste que existe dentro de linked-list.c
demo_list: $(LIST_SRC) $(INC_DIR)/linked-list.h | $(BUILD)
	$(CC) $(CFLAGS) $(LIST_SRC) -o $@ $(LDFLAGS)

# 2) Lab1a: usa o main de Lab1a.c e a biblioteca de listas sem main
lab1a: $(LAB1AOBJ) $(LIBOBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# 3) Lab1b: usa o main de Lab1b.c e a biblioteca de listas sem main
lab1b: $(LAB1BOBJ) $(LIBOBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# ============ Objetos ============

$(LAB1AOBJ): $(LAB1A) $(INC_DIR)/linked-list.h | $(BUILD)
	$(CC) $(CFLAGS) -c $(LAB1A) -o $@

$(LAB1BOBJ): $(LAB1B) $(INC_DIR)/linked-list.h | $(BUILD)
	$(CC) $(CFLAGS) -c $(LAB1B) -o $@

# Objeto da biblioteca de listas gerado a partir de uma cópia de linked-list.c sem o main de teste
$(LIBOBJ): $(LIBSRC) $(INC_DIR)/linked-list.h | $(BUILD)
	$(CC) $(CFLAGS) -c $(LIBSRC) -o $@

# Gerar cópia de linked-list.c sem a função main (remove o bloco do main heurísticamente)
# Caso, no futuro, o ficheiro use #ifdef para o main, esta etapa pode ser removida.
$(LIBSRC): $(LIST_SRC) | $(BUILD)
	@echo "[info] a gerar $(LIBSRC) (linked-list.c sem main)"
	@awk '\
		/^[[:space:]]*int[[:space:]]+main[[:space:]]*\\(/ {skip=1; brace=0} \
		skip==1 { \
			brace+=gsub(/{/, "&"); \
			brace-=gsub(/}/, "&"); \
			if (brace<=0) {skip=0; next} \
			next \
		} \
		{print} \
	' $(LIST_SRC) > $(LIBSRC)

# Pasta temporária
$(BUILD):
	mkdir -p $(BUILD)

# ============ Limpeza ============
clean:
	$(RM) -r $(BUILD) *.o

distclean: clean
	$(RM) lab1a lab1b demo_list
