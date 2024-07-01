import win32pipe, win32file, pywintypes
import matplotlib.pyplot as plt
import re
import numpy as np

pipe_name = r'\\.\pipe\plotting_pipe'
pipe_feedback_name = r'\\.\pipe\plotting_pipe_feedback'

ganttCores = []
statistics = []
realtimeGantt = []

def createTheOutput(statistics):
    print(statistics)
    colors = ["#e60049", "#0bb4ff", "#50e991"]
    burst_distribution = []
    for stat in statistics:
        stat = stat.split('\n')
        for i in range(1, len(stat)):
            burst_distribution.append(stat[i].split())
        break

    fig, ax = plt.subplots()
    ax.set_title("Histograma Burst Times")

    for x, c in zip(burst_distribution, colors):
        distributioId = int(x[0])
        mean = float(x[1])
        std = float(x[2])
        data = list(map(int, x[3:]))
        ax.hist(data, bins=10, alpha=0.75, color=c, label=f"Distributia {distributioId} de medie {mean} si deviatie standard {std}")

    ax.legend(loc='upper right')
    plt.savefig('histograms.png')

    fig, ax = plt.subplots(nrows=2, ncols=2, figsize=(12, 8), dpi=80)

    algorithms = []
    perAlgorithmWaits = []
    perAlgorithmTT = []
    perAlgorithmrT = []
    perAlgoritmContextSwitches = []

    for stat in statistics:
        stat = stat.split('\n')

        numbers = re.findall(r'\b\d+\.?\d*\b', stat[0])
        strings = re.findall(r'\b[a-zA-Z]+\b', stat[0])

        numbers = list(map(float, numbers))

        information = stat[0]
        idx = information.split().index(strings[0])

        values = information.split()[:idx]
        alg = ' '.join(information.split()[idx:])

        values = list(map(float, values))

        distNums = (len(values) - 4) // 4

        avgWaitTime = values[0]
        avgTurnaroundTime = values[1]
        avgResponseTime = values[2]

        perDist = {}

        allWaiting = []
        allTurnAround = []
        allResponse = []

        for i in range(distNums):
            perDist[i] = [values[4 + 4 * i + 1], values[4 + 4 * i + 2], values[4 + 4 * i + 3]]
            allWaiting.append(values[4 + 4 * i + 1])
            allResponse.append(values[4 + 4 * i + 2])
            allTurnAround.append(values[4 + 4 * i + 3])

        algorithms.append(alg)
        perAlgorithmWaits.append(allWaiting)
        perAlgorithmTT.append(allTurnAround)
        perAlgorithmrT.append(allResponse)
        perAlgoritmContextSwitches.append(values[3])

    for alg, waitingTimes in zip(algorithms, perAlgorithmWaits):
        bottom = 0
        if alg == "Efficient Dynamic Round Robin":
            algo = "EDRR"
        elif "Mean Threshold Shortest Job Round Robin" in alg:
            algo = "MTSJRR T = " + alg.split()[-1]
        elif "Round Robin" in alg:
            algo = "RR T = " + alg.split()[-1]
        elif alg == "Mean Median Time Quantum Round Robin":
            algo = "MeanMedianRR"
        elif alg == "Min Max Round Robin":
            algo = "MinMaxRR"
        elif alg == "First In First Out":
            algo = "FIFO"
        elif alg == "Shortest Job First":
            algo = "SJF"
        elif alg == "Shortest Remaining Time First":
            algo = "SRTF"
        for i, value in enumerate(waitingTimes):
            ax[0,0].bar(algo, value, bottom=bottom, color=colors[i])
            bottom += value
        ax[0,0].title.set_text('Average Waiting Time')

    for alg, responseTimes in zip(algorithms, perAlgorithmrT):
        bottom = 0
        if alg == "Efficient Dynamic Round Robin":
            algo = "EDRR"
        elif "Mean Threshold Shortest Job Round Robin" in alg:
            algo = "MTSJRR T = " + alg.split()[-1]
        elif "Round Robin" in alg:
            algo = "RR T = " + alg.split()[-1]
        elif alg == "Mean Median Time Quantum Round Robin":
            algo = "MeanMedianRR"
        elif alg == "Min Max Round Robin":
            algo = "MinMaxRR"
        elif alg == "First In First Out":
            algo = "FIFO"
        elif alg == "Shortest Job First":
            algo = "SJF"
        elif alg == "Shortest Remaining Time First":
            algo = "SRTF"
        for i, value in enumerate(responseTimes):
            ax[0,1].bar(algo, value, bottom=bottom, color=colors[i])
            bottom += value
        ax[0,1].title.set_text('Average Response Time')

    for alg, turnaroundTimes in zip(algorithms, perAlgorithmTT):
        bottom = 0
        if alg == "Efficient Dynamic Round Robin":
            algo = "EDRR"
        elif "Mean Threshold Shortest Job Round Robin" in alg:
            algo = "MTSJRR T = " + alg.split()[-1]
        elif "Round Robin" in alg:
            algo = "RR T = " + alg.split()[-1]
        elif alg == "Mean Median Time Quantum Round Robin":
            algo = "MeanMedianRR"
        elif alg == "Min Max Round Robin":
            algo = "MinMaxRR"
        elif alg == "First In First Out":
            algo = "FIFO"
        elif alg == "Shortest Job First":
            algo = "SJF"
        elif alg == "Shortest Remaining Time First":
            algo = "SRTF"
        for i, value in enumerate(turnaroundTimes):
            ax[1,0].bar(algo, value, bottom=bottom, color=colors[i])
            bottom += value
        ax[1,0].title.set_text('Average Turnaround Time')

    for alg, contexSwitches in zip(algorithms, perAlgoritmContextSwitches):
        if alg == "Efficient Dynamic Round Robin":
            algo = "EDRR"
        elif "Mean Threshold Shortest Job Round Robin" in alg:
            algo = "MTSJRR T = " + alg.split()[-1]
        elif "Round Robin" in alg:
            algo = "RR T = " + alg.split()[-1]
        elif alg == "Mean Median Time Quantum Round Robin":
            algo = "MeanMedianRR"
        elif alg == "Min Max Round Robin":
            algo = "MinMaxRR"
        elif alg == "First In First Out":
            algo = "FIFO"
        elif alg == "Shortest Job First":
            algo = "SJF"
        elif alg == "Shortest Remaining Time First":
            algo = "SRTF"
        ax[1,1].bar(algo, contexSwitches)
        ax[1,1].title.set_text('Number of Context Switches')

    fig.tight_layout()
    plt.savefig('statistici.png')


def plotGanttRealTime(ganttData):
    algName = None
    cores = 0
    procs = []
    for line in ganttData.split("\n"):
        if len(line.strip().split()) == 6:
            if "0 Core" in line:
                cores += 1

    coreData = {}
    coreGantt = {}

    for i in range(cores):
        coreData[f"{i}"] = []
        coreGantt[f"{i}"] = []

    for line in ganttData.split("\n"):
        if 2 < len(line.strip().split()) < 6:
            algName = line.split()
        elif len(line.strip().split()) == 6:
            info = line.split()
            coreData[info[2]].append(info[4])
            procs.append(info[4])

    for k in range(cores):
        i = 0
        j = 0
        while i < len(coreData[f"{k}"]):
            j = i
            while j < len(coreData[f"{k}"]) and coreData[f"{k}"][i] == coreData[f"{k}"][j]:
                j += 1
            coreGantt[f"{k}"].append((i, j, coreData[f"{k}"][i]))
            i = j

    nColums = len(coreGantt)

    realtimeGantt.append((algName, coreGantt))
    print(realtimeGantt)
    processes = sorted(set(procs))
    color_map = plt.get_cmap('tab20')
    colors = {process: color_map(i / len(processes)) for i, process in enumerate(processes)}

    fig, ax = plt.subplots(nrows=len(realtimeGantt), ncols=nColums, figsize=(15, 5 * len(realtimeGantt)))

    if len(realtimeGantt) == 1:
        ax = [ax]

    for i in range(len(realtimeGantt)):
        for j in range(nColums):
            for start, end, process in realtimeGantt[i][1][f"{j}"]:
                duration = end - start
                ax[i][j].barh(process, duration, left=start, color=colors[process])
                ax[i][j].text(start + duration / 2, process, str(duration), va='center', ha='center', color='black')
            ax[i][j].set_title(f"Core {j} -- {' '.join(realtimeGantt[i][0])}")

    fig.suptitle("Gantt Chart", fontsize=16)

    plt.tight_layout()
    plt.savefig("realtime.png")




def plotGantt(ganttData, statistics):
    data = ganttData.split("\n")

    coreId = None
    processes = []
    coreData = []
    for line in data:
        if len(line) == 1:
            coreId = int(line.strip())
        elif len(line) > 5:
            info = line.split()
            coreData.append((int(info[1]), int(info[3]), int(info[5])))
            processes.append(int(info[5]))

    processes = sorted(set(processes))
    ganttCores.append((processes, coreData))

    fig, axes = plt.subplots(nrows=len(ganttCores), figsize=(10, 5 * len(ganttCores)), squeeze=False)

    for ax, (stat, (processes, coreData)) in zip(axes.flat, zip(statistics, ganttCores)):
        strings = re.findall(r'\b[a-zA-Z]+\b', stat)
        idx = stat.split().index(strings[0])
        information = stat.split('\n')[0]
        alg = ' '.join(information.split()[idx:])

        color_map = plt.get_cmap('tab20')
        colors = {process: color_map(i / len(processes)) for i, process in enumerate(processes)}

        for start, end, process in coreData:
            duration = end - start
            ax.barh(process, duration, left=start, color=colors[process])
            ax.text(start + duration / 2, process, str(duration), va='center', ha='center', color='black')

        ax.set_xlabel('Time')
        ax.set_ylabel('Process')
        ax.set_title(f'{alg}')

    plt.tight_layout()
    plt.savefig('burstTimes.png')

def pipe_server():

    while True:
        print("Connecting to named pipes...")

        pipe = None
        while True:
            try:
                pipe = win32file.CreateFile(
                    pipe_name,
                    win32file.GENERIC_READ | win32file.GENERIC_WRITE,
                    0,
                    None,
                    win32file.OPEN_EXISTING,
                    0,
                    None
                )
                print("Pipe exists and opened successfully!")
                break
            except:
                pass
        print("Connected to main pipe.")
        win32file.WriteFile(pipe, b"OK\n")

        pipe_feedback = win32file.CreateFile(
            pipe_feedback_name,
            win32file.GENERIC_READ | win32file.GENERIC_WRITE,
            0,
            None,
            win32file.OPEN_EXISTING,
            0,
            None
        )
        print("Connected to feedback pipe.")

        try:
            print("Reading opcode from main pipe...")
            resp = win32file.ReadFile(pipe, 64 * 1024)

            package = resp[1].decode().strip()
            print(package)
            if "realtime" in package:
                plotGanttRealTime(package.replace("realtime\n", ""))
                win32file.WriteFile(pipe_feedback, b"OK\n")
            elif "singlecore" in package:

                statistics.append(package.replace("singlecore\n", ""))
                createTheOutput(statistics)
                win32file.WriteFile(pipe_feedback, b"OK\n")
            # Add more opcodes and their corresponding actions here as needed
            elif "gantt" in package:
                print(package.replace("gantt\n", ""))

                plotGantt(package.replace("gantt\n", ""), statistics)

                win32file.WriteFile(pipe_feedback, b"OK\n")

            else:
                print(f"Unknown opcode: {opcode}")
                win32file.WriteFile(pipe_feedback, b"Unknown opcode\n")

        except pywintypes.error as e:
            if e.args[0] == 109:  # ERROR_BROKEN_PIPE
                print("Client disconnected.")
                break

    win32file.CloseHandle(pipe)
    win32file.CloseHandle(pipe_feedback)
    print("Pipes closed.")

if __name__ == "__main__":
    pipe_server()
