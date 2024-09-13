# Progetto Parallel Prim - MPI

Questo progetto implementa una versione parallela dell'algoritmo di Prim utilizzando MPI per calcolare l'albero ricoprente minimo (MST) di un grafo. Inoltre, fornisce strumenti per misurare le prestazioni dell'algoritmo.

## Struttura del Progetto

### 1. `Avg_time.sh`
Script di supporto per il calcolo delle metriche. Avvia il programma `prim.c` per diverse dimensioni di matrice e numero di processori, generando un file CSV temporaneo con i tempi medi di esecuzione paralleli.

### 2. `Bench.sh`
Script per avviare il benchmark. Richiede come argomento il numero massimo di processori utilizzabili. Utilizza gli script `gen.sh`, `avg_time.sh` e lo script Python `metrics.py` per generare le matrici, calcolare le metriche e produrre i risultati.

### 3. `Gen.sh`
Script per generare le matrici di adiacenza che saranno utilizzate dall'algoritmo di Prim. Utilizza il programma `random_graph.c` per creare grafi casuali.

### 4. `Metrics.py`
Script Python che calcola speedup, efficienza e metrica Karp-Flatt partendo dai tempi di esecuzione paralleli e sequenziali generati da `avg_time.sh`.

### 5. `Plot.py`
Script Python per generare i grafici relativi alle prestazioni:
- Speedup vs Processori
- Efficienza vs Processori
- Tempo parallelo vs Processori

### 6. `Prim.c`
Implementazione parallela dell'algoritmo di Prim utilizzando MPI. Prende come input il nome del file contenente la matrice di adiacenza.

### 7. `Random_graph.c`
Utilizzato dallo script `gen.sh` per generare matrici di adiacenza casuali. Prende come argomenti il numero di vertici, la probabilità di inserimento di un vertice nella matrice (densità) e il nome del file di output.

### 8. `Seq.c`
Versione sequenziale dell'algoritmo di Prim per il calcolo dell'MST. Utilizzato in fase di testing per verificare la correttezza.

## Istruzioni per l'Uso

1. Compilare i file sorgenti con il comando:
   ```bash
   make
   ```

2. Dare i permessi di esecuzione agli script:
   ```bash
   chmod +x avg_time.sh bench.sh gen.sh
   ```

3. Avviare il benchmark con il seguente comando, specificando il numero massimo di processori da utilizzare:
   ```bash
   ./bench.sh <num_max_processori>
   ```

4. I risultati saranno salvati in un file CSV nella cartella output
