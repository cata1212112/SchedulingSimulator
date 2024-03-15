import matplotlib
import matplotlib.pyplot as plt
import argparse
import ast

# Exemplu input
# 0 "Shortest Job First" [[2,0,2],[1,2,8]] "First In First Out" [[1,0,6],[2,6,8]] 1 "Shortest Job First" [[2,0,2],[1,2,8]] "First In First Out" [[1,0,6],[2,6,8]]


# cores_data = {core_id : {algorithm : [gantt list]}}
def plot_data(cores_data):
    num_cores = len(cores_data)
    num_algs = len(cores_data[0])
    gantt_charts = {}
    for core_id in cores_data.keys():
        for alg in cores_data[core_id].keys():
            gantt_charts[alg] = {}
    for core_id in cores_data.keys():
        for alg in cores_data[core_id].keys():
            gantt_charts[alg][core_id] = cores_data[core_id][alg]

    main_figure = plt.figure(layout="constrained")
    subfigures = main_figure.subfigures(num_algs)
    # fig, axes = plt.subplots(num_algs, num_cores)

    for index1, alg in enumerate(gantt_charts.keys()):
        if num_algs > 1:
            axes = subfigures[index1].subplots(1, num_cores)
            subfigures[index1].suptitle(alg)
            for index2, core_id in enumerate(gantt_charts[alg]):
                proecsses_bars = {}
                for proc in gantt_charts[alg][core_id]:
                    if proc[0] not in proecsses_bars:
                        proecsses_bars[proc[0]] = [(proc[1], proc[2] - proc[1])]
                    else:
                        proecsses_bars[proc[0]].append((proc[1], proc[2] - proc[1]))

                procs = []
                for proc in proecsses_bars.keys():
                    procs.append(f"Process {proc}")

                num_proc = len(set(procs))
                colors = matplotlib.colormaps.get_cmap('hsv')
                # print(len(colors))

                if num_cores > 1:
                    axes[index2].set_title(f"Core {core_id}")
                    axes[index2].set_yticks([6 * i + 3 for i in range(1, num_proc + 1)])
                    axes[index2].set_yticklabels(procs)
                    # axes[index1, index2].set_xticks()
                    for proc in proecsses_bars.keys():
                        axes[index2].broken_barh(proecsses_bars[proc], (6 * proc, 6), color=colors((proc * 65337) % 211))
            else:
                axes.set_title(f"Core {core_id}")
                axes.set_yticks([6 * i + 3 for i in range(1, num_proc + 1)])
                axes.set_yticklabels(procs)
                # axes[index1, index2].set_xticks()
                for proc in proecsses_bars.keys():
                    axes.broken_barh(proecsses_bars[proc], (6 * proc, 6), color=colors((proc * 65337) % 211))
        else:
            axes = subfigures.subplots(1, num_cores)
            subfigures.suptitle(alg)
            for index2, core_id in enumerate(gantt_charts[alg]):
                proecsses_bars = {}
                for proc in gantt_charts[alg][core_id]:
                    if proc[0] not in proecsses_bars:
                        proecsses_bars[proc[0]] = [(proc[1], proc[2] - proc[1])]
                    else:
                        proecsses_bars[proc[0]].append((proc[1], proc[2] - proc[1]))

                procs = []
                for proc in proecsses_bars.keys():
                    procs.append(f"Process {proc}")

                num_proc = len(set(procs))
                colors = matplotlib.colormaps.get_cmap('hsv')
                # print(len(colors))

                if num_cores > 1:
                    axes[index2].set_title(f"Core {core_id}")
                    axes[index2].set_yticks([6 * i + 3 for i in range(1, num_proc + 1)])
                    axes[index2].set_yticklabels(procs)
                    # axes[index1, index2].set_xticks()
                    for proc in proecsses_bars.keys():
                        axes[index2].broken_barh(proecsses_bars[proc], (6 * proc, 6), color=colors((proc * 65337) % 211))
                else:
                    axes.set_title(f"Core {core_id}")
                    axes.set_yticks([6 * i + 3 for i in range(1, num_proc + 1)])
                    axes.set_yticklabels(procs)
                    # axes[index1, index2].set_xticks()
                    for proc in proecsses_bars.keys():
                        axes.broken_barh(proecsses_bars[proc], (6 * proc, 6), color=colors((proc * 65337) % 211))

    plt.savefig("gantt_chart.png")


# --core 0 "Shortest Job First" [[2,0,2],[1,2,8]] "First In First Out" [[1,0,6],[2,6,8]] --core 1 "Shortest Job First" [[2,0,2],[1,2,8]] "First In First Out" [[1,0,6],[2,6,8]]
def main():
    core_values_args = argparse.ArgumentParser()
    core_values_args.add_argument('--core', nargs='+', action='append', metavar=('CORE_NUMBER', 'OBJECTS'))

    data = {}

    args = core_values_args.parse_args()

    cores_data = args.core

    for core_data in cores_data:
        core_id = int(core_data[0])
        core_data = core_data[1:]

        algortihms = {}
        for i in range(0, len(core_data), 2):
            algorithm = core_data[i]
            gantt_list = ast.literal_eval(core_data[i+1])

            algortihms[algorithm] = gantt_list

        data[core_id] = algortihms

    plot_data(data)

if __name__ == '__main__':
    main()