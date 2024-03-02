import sys
import matplotlib.pyplot as plt
import numpy as np

def save_plot(algorithms, cpu_utilizations, avg_waiting_times, avg_turnaround_times, avg_response_times):
    num_algorithms = len(algorithms)
    width = 0.15
    ind = np.arange(num_algorithms)

    fig, ax = plt.subplots(figsize=(12, 6))

    ax.bar(ind - 3*width/2, cpu_utilizations, width, label='CPU Utilization')
    ax.bar(ind - width/2, avg_waiting_times, width, label='Average Waiting Time')
    ax.bar(ind + width/2, avg_turnaround_times, width, label='Average Turnaround Time')
    ax.bar(ind + 3*width/2, avg_response_times, width, label='Average Response Time')

    ax.set_xlabel('Algorithm')
    ax.set_ylabel('Values')
    ax.set_title('Performance Metrics')
    ax.set_xticks(ind)
    ax.set_xticklabels(algorithms)
    ax.legend()

    plt.xticks(rotation=45)
    plt.grid(True)

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
