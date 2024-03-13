import sys
import matplotlib.pyplot as plt
import numpy as np

def save_plot(cores, algorithms, cpu_utilizations, avg_waiting_times, avg_turnaround_times, avg_response_times):
    num_cores = len(cores)
    num_algorithms = len(algorithms)
    width = 0.15
    ind = np.arange(num_algorithms)

    fig, axs = plt.subplots(num_cores, 2, figsize=(12, 5*num_cores))

    for i, core in enumerate(cores):
        # Plot CPU Utilization separately
        axs[i, 0].bar(ind, cpu_utilizations[i], width, label='CPU Utilization', color='orange')
        axs[i, 0].set_title(f'CPU Utilization (Core {core})')
        axs[i, 0].set_ylabel('CPU Utilization')
        axs[i, 0].set_xticks(ind)
        axs[i, 0].set_xticklabels(algorithms)
        axs[i, 0].legend()
        axs[i, 0].grid(True)

        # Plot other performance metrics
        axs[i, 1].bar(ind - 3*width/2, avg_waiting_times[i], width, label='Average Waiting Time')
        axs[i, 1].bar(ind - width/2, avg_turnaround_times[i], width, label='Average Turnaround Time')
        axs[i, 1].bar(ind + width/2, avg_response_times[i], width, label='Average Response Time')

        axs[i, 1].set_xlabel('Algorithm')
        axs[i, 1].set_ylabel('Values')
        axs[i, 1].set_title(f'Performance Metrics (Core {core})')
        axs[i, 1].set_xticks(ind)
        axs[i, 1].set_xticklabels(algorithms)
        axs[i, 1].legend()
        axs[i, 1].grid(True)

    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.savefig('performance_metrics_plot.png')

if __name__ == "__main__":
    if len(sys.argv) < 7:
        print("Usage: python script.py <core> <algorithm1> <cpu_utilization1> <avg_waiting_time1> <avg_turnaround_time1> <avg_response_time1> ...")
        sys.exit(1)

    cores = []
    algorithms = []
    cpu_utilizations = []
    avg_waiting_times = []
    avg_turnaround_times = []
    avg_response_times = []

    for i in range(1, len(sys.argv), 6):
        cores.append(sys.argv[i])
        algorithms.append(sys.argv[i+1])
        cpu_utilizations.append(float(sys.argv[i+2]))
        avg_waiting_times.append(float(sys.argv[i+3]))
        avg_turnaround_times.append(float(sys.argv[i+4]))
        avg_response_times.append(float(sys.argv[i+5]))

    save_plot(cores, algorithms, cpu_utilizations, avg_waiting_times, avg_turnaround_times, avg_response_times)
