import sys
import matplotlib.pyplot as plt
import numpy as np

def save_plot(algorithms, cpu_utilizations, avg_waiting_times, avg_turnaround_times, avg_response_times):
    num_algorithms = len(algorithms)
    width = 0.15
    ind = np.arange(num_algorithms)

    fig, axs = plt.subplots(2, figsize=(12, 10))

    # Plot CPU Utilization separately
    axs[0].bar(ind, cpu_utilizations, width, label='CPU Utilization', color='orange')
    axs[0].set_title('CPU Utilization')
    axs[0].set_ylabel('CPU Utilization')
    axs[0].set_xticks(ind)
    axs[0].set_xticklabels(algorithms)
    axs[0].legend()
    axs[0].grid(True)

    # Plot other performance metrics
    axs[1].bar(ind - 3*width/2, avg_waiting_times, width, label='Average Waiting Time')
    axs[1].bar(ind - width/2, avg_turnaround_times, width, label='Average Turnaround Time')
    axs[1].bar(ind + width/2, avg_response_times, width, label='Average Response Time')

    axs[1].set_xlabel('Algorithm')
    axs[1].set_ylabel('Values')
    axs[1].set_title('Performance Metrics')
    axs[1].set_xticks(ind)
    axs[1].set_xticklabels(algorithms)
    axs[1].legend()
    axs[1].grid(True)

    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.savefig('performance_metrics_plot.png')

if __name__ == "__main__":
    if len(sys.argv) < 6:
        print("Usage: python script.py <algorithm1> <cpu_utilization1> <avg_waiting_time1> <avg_turnaround_time1> <avg_response_time1> ...")
        sys.exit(1)

    algorithms = []
    cpu_utilizations = []
    avg_waiting_times = []
    avg_turnaround_times = []
    avg_response_times = []

    for i in range(1, len(sys.argv), 5):
        algorithms.append(sys.argv[i])
        cpu_utilizations.append(float(sys.argv[i+1]))
        avg_waiting_times.append(float(sys.argv[i+2]))
        avg_turnaround_times.append(float(sys.argv[i+3]))
        avg_response_times.append(float(sys.argv[i+4]))

    save_plot(algorithms, cpu_utilizations, avg_waiting_times, avg_turnaround_times, avg_response_times)
