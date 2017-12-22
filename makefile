# options de compilation
APIDIR = /home/sasl/encad/brajard/projet/distrib/include
CC = gcc
CCFLAGS = -Wall -I$(APIDIR)
LIBS = -L/home/sasl/encad/brajard/projet/distrib/lib
LDFLAGS = -lm -lcgs

# fichiers du projet
SRC = template.c
OBJ = $(SRC:.c=.o)
EXEC = main

# règle initiale
all: $(EXEC)

# dépendance des .h
main.o:

# règles de compilation
%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

# règles d'édition de liens
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(LDFLAGS)

# règles supplémentaires
clean:
	rm -f *.o *~ 
rmproper:
	rm -f $(EXEC) *.o
