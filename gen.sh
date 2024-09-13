#!/bin/bash

# Elenco delle probabilità e dei numeri di vertici
# probabilities=(0.1 1)

probabilities=(1)
sizes=(6000 12000 18000 24000 30000)

# Creare una cartella per ogni probabilità e generare i file
for prob in "${probabilities[@]}"; do
    mkdir -p "input/$prob"  # Crea la cartella per la probabilità
    for size in "${sizes[@]}"; do
        # Genera il file della matrice usando random_graph
        ./random_graph "$size" "$prob" "input/$prob/$size.bin"
    done
done

echo "Dataset generation complete."
