import sys
import matplotlib.pyplot as plt

def plot_table(algorithms, cpu_utilizations, avg_waiting_times, avg_turnaround_times, avg_response_times):
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.axis('tight')
    ax.axis('off')

    table_data = []
    for i in range(len(algorithms)):
        table_data.append([algorithms[i], cpu_utilizations[i], avg_waiting_times[i], avg_turnaround_times[i], avg_response_times[i]])

    column_labels = ["Algorithm", "CPU Utilization", "Avg Waiting Time", "Avg Turnaround Time", "Avg Response Time"]
    ax.table(cellText=table_data, colLabels=column_labels, loc='center')

    plt.title('Performance Metrics Table')
    plt.savefig('performance_metrics_table.png')

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

    plot_table(algorithms, cpu_utilizations, avg_waiting_times, avg_turnaround_times, avg_response_times)
