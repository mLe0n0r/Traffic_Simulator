# Makefile para Lab1 + linked-list
# Compila dois executáveis:
#   - demo_list : corre o main de teste que já existe em linked-list.c
#   - lab1      : corre o main de Lab1.a.c, removendo automaticamente o main de teste de linked-list.c durante a compilação

# Configuração
CC      := gcc
CFLAGS  := -Wall -Wextra -O2 -std=c11 -I.
LDFLAGS :=

# Estruturas de build temporário
BUILD   := .build
LIBSRC  := $(BUILD)/linked-list_lib.c
LIBOBJ  := $(BUILD)/linked-list.o

# Alvos por omissão (seguro): só a demo. Usa "make all" para ambos.
.PHONY: default all clean distclean
default: demo_list
all: demo_list lab1

# ------------------------------------------------------------
# demo_list: compila o exemplo de teste presente em linked-list.c
demo_list: linked-list.c linked-list.h
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

# ------------------------------------------------------------
# lab1: compila o teu Lab1.a.c + as funções de linked-list,
#       mas sem o main de teste (é removido numa cópia temporária).
lab1: Lab1.a.o $(LIBOBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

Lab1.a.o: Lab1.a.c linked-list.h
	$(CC) $(CFLAGS) -c $< -o $@

# Gera uma cópia de linked-list.c sem a função main de teste
$(LIBSRC): linked-list.c | $(BUILD)
	# Copia tudo até à linha que inicia a função main; ignora o resto
	awk 'BEGIN{p=1} /^[[:space:]]*int[[:space:]]+main[[:space:]]*\\(/{p=0} p{print}' $< > $@

# Compila o objeto da biblioteca de listas a partir da cópia sem main
$(LIBOBJ): $(LIBSRC) linked-list.h
	$(CC) $(CFLAGS) -c $(LIBSRC) -o $@

# Pasta temporária
$(BUILD):
	mkdir -p $(BUILD)

# Limpezas
clean:
	$(RM) -r $(BUILD) *.o

distclean: clean
	$(RM) lab1 demo_list
