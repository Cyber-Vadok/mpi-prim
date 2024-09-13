import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Load data from CSV
df = pd.read_csv('output/metrics.csv')

# Set up the plotting style
sns.set(style="whitegrid")

# Function to plot data
def plot_data(problem_df, problem_num):
    # Unique matrix sizes
    matrix_sizes = problem_df['Matrix Size'].unique()
    
    # Plot 1: Average Time vs. Processors
    plt.figure(figsize=(12, 6))
    for size in matrix_sizes:
        subset = problem_df[problem_df['Matrix Size'] == size]
        plt.plot(subset['Processors'], subset['Average Time'], label=f'Matrix Size {size}', marker='o')
    plt.title(f'Problem {problem_num}: Average Time vs. Processors')
    plt.xlabel('Processors')
    plt.ylabel('Average Time')
    plt.legend(title='Matrix Size')
    plt.grid(True)
    plt.savefig(f'Problem_{problem_num}_Average_Time_vs_Processors.png')
    plt.show()

    # Plot 2: Speedup vs. Processors
    plt.figure(figsize=(12, 6))
    for size in matrix_sizes:
        subset = problem_df[problem_df['Matrix Size'] == size]
        plt.plot(subset['Processors'], subset['Speedup'], label=f'Matrix Size {size}', marker='o')
    plt.title(f'Problem {problem_num}: Speedup vs. Processors')
    plt.xlabel('Processors')
    plt.ylabel('Speedup')
    plt.legend(title='Matrix Size')
    plt.grid(True)
    plt.savefig(f'Problem_{problem_num}_Speedup_vs_Processors.png')
    plt.show()

    # Plot 3: Efficiency vs. Processors
    plt.figure(figsize=(12, 6))
    for size in matrix_sizes:
        subset = problem_df[problem_df['Matrix Size'] == size]
        plt.plot(subset['Processors'], subset['Efficiency'], label=f'Matrix Size {size}', marker='o')
    plt.title(f'Problem {problem_num}: Efficiency vs. Processors')
    plt.xlabel('Processors')
    plt.ylabel('Efficiency')
    plt.legend(title='Matrix Size')
    plt.grid(True)
    plt.savefig(f'Problem_{problem_num}_Efficiency_vs_Processors.png')
    plt.show()

# Plot data for each problem
for problem_num in df['Problem'].unique():
    problem_df = df[df['Problem'] == problem_num]
    plot_data(problem_df, problem_num)
