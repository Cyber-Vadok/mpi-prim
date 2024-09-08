# Compiler and flags
CC = gcc
MPICC = mpicc
CFLAGS = -Wall -O2
LDFLAGS = 

# Executable names
PRIM_EXEC = prim
SEQ_EXEC = seq
RANDOM_GRAPH_EXEC = random_graph

# Source files
PRIM_SRC = prim.c
SEQ_SRC = seq.c
RANDOM_GRAPH_SRC = random_graph.c

# Object files
PRIM_OBJ = $(PRIM_SRC:.c=.o)
SEQ_OBJ = $(SEQ_SRC:.c=.o)
RANDOM_GRAPH_OBJ = $(RANDOM_GRAPH_SRC:.c=.o)

# Default target
all: $(PRIM_EXEC) $(SEQ_EXEC) $(RANDOM_GRAPH_EXEC)

# Rule for prim (using mpicc)
$(PRIM_EXEC): $(PRIM_OBJ)
	$(MPICC) -o $@ $^ $(LDFLAGS)

# Rule for seq
$(SEQ_EXEC): $(SEQ_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Rule for random_graph
$(RANDOM_GRAPH_EXEC): $(RANDOM_GRAPH_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Clean target to remove executables and object files
clean:
	rm -f $(PRIM_EXEC) $(SEQ_EXEC) $(RANDOM_GRAPH_EXEC) *.o
