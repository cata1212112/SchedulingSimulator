import argparse

import matplotlib.pyplot as plt
import numpy as np


def generate_gantt_chart(process_data):
    process_data.sort(key=lambda x: x[1])

    y_positions = {}
    for i, process in enumerate(process_data):
        process_id = process[0]
        if process_id not in y_positions:
            y_positions[process_id] = i
        else:
            y_positions[process_id] = min(y_positions[process_id], i)

    num_processes = len(set(p[0] for p in process_data))
    colors = plt.cm.tab10(np.linspace(0, 1, num_processes))

    for process in process_data:
        process_id = process[0]
        color_index = list(set(p[0] for p in process_data)).index(process_id)
        plt.barh(y_positions[process_id], process[2] - process[1], left=process[1], color=colors[color_index])
        plt.text((process[1] + process[2]) / 2, y_positions[process_id], f'P{process_id}', ha='center', va='center')

    plt.yticks(range(len(y_positions)), [f'P{pid}' for pid in sorted(y_positions.keys())])

    plt.xlabel('Time')
    plt.ylabel('Processes')
    plt.title('Gantt Chart')

    plt.savefig("gantt_chart.png")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generate Gantt chart')
    parser.add_argument('process_data_str', type=str, help='Process data as a string')
    args = parser.parse_args()

    process_data = eval(args.process_data_str)

    generate_gantt_chart(process_data)
