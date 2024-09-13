import pandas as pd
import numpy as np  # Required for setting NaN values

# Load the CSV file
file_name = "output/execution_times.csv"  # Update with the actual filename if necessary
df = pd.read_csv(file_name)

# Create empty columns for Speedup, Efficiency, and Karp-Flatt metrics
df['Speedup'] = 1.0
df['Efficiency'] = 1.0
df['Karp-Flatt'] = np.nan  # Initialize Karp-Flatt with NaN

# Iterate over the rows and calculate speedup, efficiency, and Karp-Flatt metrics
for index, row in df.iterrows():
    if row['Processors'] != 1:
        # Find the corresponding row with 1 processor for the same problem and matrix size
        single_proc_time = df[(df['Problem'] == row['Problem']) & 
                              (df['Processors'] == 1) & 
                              (df['Matrix Size'] == row['Matrix Size'])]['Average Time'].values[0]
        
        # Calculate the speedup
        speedup = single_proc_time / row['Average Time']
        df.at[index, 'Speedup'] = round(speedup, 3)  # Round to 3 decimal places
        
        # Calculate the efficiency: Efficiency = Speedup / Processors
        efficiency = speedup / row['Processors']
        df.at[index, 'Efficiency'] = round(efficiency, 3)  # Round to 3 decimal places
        
        # Calculate the Karp-Flatt metric: Karp-Flatt = ((1 / Speedup) - (1 / p)) / (1 - (1 / p))
        p = row['Processors']
        karp_flatt = ((1 / speedup) - (1 / p)) / (1 - (1 / p))
        df.at[index, 'Karp-Flatt'] = round(karp_flatt, 3)  # Round to 3 decimal places
    else:
        # For 1 processor, leave the Karp-Flatt value as NaN (already set during initialization)
        df.at[index, 'Karp-Flatt'] = np.nan

# Save the updated dataframe to a new CSV file
df.to_csv("output/metrics.csv", index=False)

print("Metrics calculation (Speedup, Efficiency, Karp-Flatt) completed and saved to metrics.csv.csv")