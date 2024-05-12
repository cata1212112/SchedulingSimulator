import matplotlib.pyplot as plt
import argparse
import numpy as np

# Exemplu input
# 0 "First In First Out" 100.000000 3.000000 7.000000 3.000000 "Shortest Job First" 100.000000 1.000000 5.000000 1.000000 1 "First In First Out" 100.000000 3.000000 7.000000 3.000000 "Shortest Job First" 100.000000 1.000000 5.000000 1.000000

# cores_data = {core_id : {algorithm : {cpu_utilization, avg_waiting_time, avg_response_time, avg_turnaround_time}}}

def plot_data(cores_data):
    num_cores = len(cores_data)
    number_of_algorithms = len(cores_data[0])
    if num_cores > 1:
        fig, axes = plt.subplots(num_cores, 2, figsize=(10 * number_of_algorithms, 4 * num_cores))

        for index in cores_data.keys():
            core_data = cores_data[index]
            axes[index, 0].set_title("Core {} : CPU Utilization".format(index))
            axes[index, 0].bar(core_data.keys(), [core_data[alg]['cpu_utilization'] for alg in core_data.keys()])

            axes[index, 1].set_title("Core {} : Metrics".format(index))

            algortihms = core_data.keys()
            metrics = {}
            metrics['avg_turnaround_time'] = []
            metrics['avg_waiting_time'] = []
            metrics['avg_response_time'] = []
            metrics['context_switches'] = []

            for alg in algortihms:
                metrics['avg_turnaround_time'].append(core_data[alg]['avg_turnaround_time'])
                metrics['avg_waiting_time'].append(core_data[alg]['avg_waiting_time'])
                metrics['avg_response_time'].append(core_data[alg]['avg_response_time'])
                metrics['context_switches'].append(core_data[alg]['context_switches'])

            x = np.arange(len(algortihms))
            width = 0.25
            multiplier = 0

            for attribute, measurement in metrics.items():
                offset = width * multiplier
                rects = axes[index, 1].bar(x + offset, measurement, width, label=attribute)
                axes[index, 1].bar_label(rects, padding=3)
                multiplier += 1

            axes[index, 1].set_ylabel('Time unit')
            axes[index, 1].set_xticks(x + width, algortihms)
            axes[index, 1].legend(loc='center left', ncols=1, bbox_to_anchor=(1, 0.5))
    else:
        fig, axes = plt.subplots(num_cores, 2, figsize = (5 * number_of_algorithms, 4))

        core_data = cores_data[0]
        axes[0].set_title("Core {} : CPU Utilization".format(0))
        axes[0].bar(core_data.keys(), [core_data[alg]['cpu_utilization'] for alg in core_data.keys()])

        axes[1].set_title("Core {} : Metrics".format(0))

        algortihms = core_data.keys()
        metrics = {}
        metrics['avg_turnaround_time'] = []
        metrics['avg_waiting_time'] = []
        metrics['avg_response_time'] = []
        metrics['context_switches'] = []


        for alg in algortihms:
            metrics['avg_turnaround_time'].append(core_data[alg]['avg_turnaround_time'])
            metrics['avg_waiting_time'].append(core_data[alg]['avg_waiting_time'])
            metrics['avg_response_time'].append(core_data[alg]['avg_response_time'])
            metrics['context_switches'].append(core_data[alg]['context_switches'])


        x = np.arange(len(algortihms))
        width = 0.25
        multiplier = 0

        for attribute, measurement in metrics.items():
            offset = width * multiplier
            rects = axes[1].bar(x + offset, measurement, width, label=attribute)
            axes[1].bar_label(rects, padding=3)
            multiplier += 1

        axes[1].set_ylabel('Time unit')
        axes[1].set_xticks(x + width, algortihms)
        axes[1].legend(loc='center left', ncols=1, bbox_to_anchor=(1, 0.5))

    fig.tight_layout()
    plt.savefig('performance_metrics_plot.png')

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