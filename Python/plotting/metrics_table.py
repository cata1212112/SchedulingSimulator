import matplotlib.pyplot as plt
import argparse

# Exemplu input
# 0 "First In First Out" 100.000000 3.000000 7.000000 3.000000 "Shortest Job First" 100.000000 1.000000 5.000000 1.000000 1 "First In First Out" 100.000000 3.000000 7.000000 3.000000 "Shortest Job First" 100.000000 1.000000 5.000000 1.000000

# cores_data = {core_id : {algorithm : {cpu_utilization, avg_waiting_time, avg_response_time, avg_turnaround_time}}}

def plot_data(cores_data):
    num_cores = len(cores_data)
    number_of_algorithms = len(cores_data[0])

    table_data = {}

    for index in cores_data.keys():
        core_data = cores_data[index]
        for alg in core_data.keys():
            table_data[alg] = []

    for index in cores_data.keys():
        core_data = cores_data[index]
        for alg in core_data.keys():
            table_data[alg].append([index, core_data[alg]['cpu_utilization'], core_data[alg]['avg_turnaround_time'],
                                    core_data[alg]['avg_waiting_time'], core_data[alg]['avg_response_time'], core_data[alg]['context_switches']])

    labels = ["Core number", "CPU utilization", "Average turnaround time", "Average waiting time",
              "Average response time", "Context Switches"]

    fig, ax = plt.subplots(number_of_algorithms, 1, figsize=(10, 10))

    if number_of_algorithms > 1:
        for index, alg in enumerate(table_data.keys()):
            ax[index].table(cellText=table_data[alg], colLabels=labels, loc='center')
            ax[index].axis('off')
            ax[index].axis('tight')
            ax[index].set_title(alg)
    else:
        for index, alg in enumerate(table_data.keys()):
            ax.table(cellText=table_data[alg], colLabels=labels, loc='center')
            ax.axis('off')
            ax.axis('tight')
            ax.set_title(alg)

    plt.tight_layout()
    plt.savefig('performance_metrics_table.png', bbox_inches='tight')

# --algortihm "First In First Out" 100.000000 3.000000 7.000000 3.000000 --algortihm "Shortest Job First" 100.000000 1.000000 5.000000 1.000000

# --core 0 "First In First Out" 100.000000 3.000000 7.000000 3.000000 "Shortest Job First" 100.000000 1.000000 5.000000 1.000000 --core 1 "First In First Out" 100.000000 3.000000 7.000000 3.000000 "Shortest Job First" 100.000000 1.000000 5.000000 1.000000
def main():
    core_values_args = argparse.ArgumentParser()
    core_values_args.add_argument('--core', nargs='+', action='append', metavar=('CORE_NUMBER', 'OBJECTS'))

    args = core_values_args.parse_args()

    cores_data = args.core

    data = {}

    for core_data in cores_data:
        core_id = int(core_data[0])
        core_data = core_data[1:]
        algortihms = {}
        for i in range(0, len(core_data), 6):
            algortihms_data = {}
            algortihms_data['cpu_utilization'] = float(core_data[i+1])
            algortihms_data['avg_waiting_time'] = float(core_data[i+2])
            algortihms_data['avg_turnaround_time'] = float(core_data[i+3])
            algortihms_data['avg_response_time'] = float(core_data[i+4])
            algortihms_data['context_switches'] = float(core_data[i+5])

            algortihms[core_data[i]] = algortihms_data

        data[core_id] = algortihms

    plot_data(data)

if __name__ == '__main__':
    main()