import argparse
import matplotlib.pyplot as plt

def plot_data(cores_data):
    num_cores = len(cores_data)
    num_algorithms = len(cores_data[0]['algorithms'])

    fig, axs = plt.subplots(num_cores, num_algorithms + 1, figsize=(10, 4 * num_cores))  # Adjust the figsize parameter here

    # if num_cores == 1:
    #     axs = [axs]  # Convert to a single-element list to handle single-core case

    for i, core_data in enumerate(cores_data):
        for j, algorithm_data in enumerate(core_data['algorithms']):
            # Plot CPU utilization
            if num_cores > 1:
                axs[i, 0].bar(f'Core {core_data["core_number"]} - {algorithm_data["algorithm_name"]}',
                              algorithm_data['cpu_utilization'],
                              color='blue')  # You can change color here
                axs[i, 0].set_ylabel('CPU Utilization')
                axs[i, 0].set_title('CPU Utilization')
            else:
                axs[0].bar(f'Core {core_data["core_number"]} - {algorithm_data["algorithm_name"]}',
                        algorithm_data['cpu_utilization'],
                        color='blue')  # You can change color here
                axs[0].set_ylabel('CPU Utilization')
                axs[0].set_title('CPU Utilization')

            # Plot other metrics
            if num_cores > 1:
                axs[i, j + 1].bar(['Avg Waiting Time', 'Avg Turnaround Time', 'Avg Response Time'],
                                  [algorithm_data['avg_waiting_time'], algorithm_data['avg_turnaround_time'], algorithm_data['avg_response_time']],
                                  color=['red', 'green', 'orange'])  # You can specify different colors here
                axs[i, j + 1].set_title(f'Core {core_data["core_number"]} - {algorithm_data["algorithm_name"]}')
            else:
                axs[j + 1].bar(['Avg Waiting Time', 'Avg Turnaround Time', 'Avg Response Time'],
                               [algorithm_data['avg_waiting_time'], algorithm_data['avg_turnaround_time'], algorithm_data['avg_response_time']],
                               color=['red', 'green', 'orange'])  # You can specify different colors here
                axs[j + 1].set_title(f'Core {core_data["core_number"]} - {algorithm_data["algorithm_name"]}')

    plt.tight_layout()
    plt.savefig('performance_metrics_plot.png')

def main():
    parser = argparse.ArgumentParser(description='Plot CPU utilization, average waiting time, average turnaround time, and average response time.')
    parser.add_argument('data', nargs='+', help='Data for each core and algorithm')
    args = parser.parse_args()

    cores_data = []
    current_core_data = None

    for idx, item in enumerate(args.data):
        if idx % 6 == 0:
            if current_core_data:
                cores_data.append(current_core_data)
            current_core_data = {'core_number': int(item), 'algorithms': []}
        else:
            if idx % 5 == 0:
                current_core_data['algorithms'].append({'algorithm_name': item})
            elif idx % 5 == 1:
                try:
                    current_core_data['algorithms'][-1]['cpu_utilization'] = float(item)
                except ValueError:
                    print(f"Invalid CPU utilization value for algorithm {current_core_data['algorithms'][-1]['algorithm_name']}: {item}")
            elif idx % 5 == 2:
                current_core_data['algorithms'][-1]['avg_waiting_time'] = float(item)
            elif idx % 5 == 3:
                current_core_data['algorithms'][-1]['avg_turnaround_time'] = float(item)
            elif idx % 5 == 4:
                current_core_data['algorithms'][-1]['avg_response_time'] = float(item)

    if current_core_data:
        cores_data.append(current_core_data)

    plot_data(cores_data)

if __name__ == "__main__":
    main()