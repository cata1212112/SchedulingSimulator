import win32pipe, win32file, pywintypes
import matplotlib.pyplot as plt
import re
import numpy as np

pipe_name = r'\\.\pipe\plotting_pipe'
pipe_feedback_name = r'\\.\pipe\plotting_pipe_feedback'


def createTheOutput(statistics):

    colors = ["#e60049", "#0bb4ff", "#50e991"]
    burst_distribution = []
    for stat in statistics:
        stat = stat.split('\n')
        for i in range(1, len(stat)):
            burst_distribution.append(stat[i].split())
        break


    fig, ax = plt.subplots(nrows=1, ncols=5, figsize=(24, 12), dpi=80)
    ax[0].title.set_text("Histograma Burst Times")

    for x, c in zip(burst_distribution, colors):
        distributioId = int(x[0])
        mean = float(x[1])
        std = float(x[2])
        ax[0].hist(list(map(int, x[3:])), bins=10, alpha=0.75, color=c, label=f"Distributia {distributioId} de medie {mean} si deviatie standard {std}")
    ax[0].legend(loc='upper right')

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
            #                wait                   response                turnaround
            perDist[i] = [values[4 + 4 * i + 1], values[4 + 4 * i + 2], values[4 + 4 * i + 3]]

            allWaiting.append(values[4 + 4 * i + 1])
            allResponse.append(values[4 + 4 * i + 2])
            allTurnAround.append(values[4 + 4 * i + 3])

        print(alg, avgWaitTime, allWaiting)

        algorithms.append(alg)
        perAlgorithmWaits.append(allWaiting)
        perAlgorithmTT.append(allTurnAround)
        perAlgorithmrT.append(allResponse)
        perAlgoritmContextSwitches.append(values[3])

    print(algorithms)
    print(perAlgorithmWaits)

    for alg, waitingTimes in zip(algorithms, perAlgorithmWaits):
        bottom = 0
        for i, value in enumerate(waitingTimes):
            ax[1].bar(alg, value, bottom=bottom, color=colors[i])
            bottom += value
        ax[1].title.set_text('Average Waiting Time')

    for alg, responseTimes in zip(algorithms, perAlgorithmrT):
        bottom = 0
        for i, value in enumerate(responseTimes):
            ax[2].bar(alg, value, bottom=bottom, color=colors[i])
            bottom += value
        ax[2].title.set_text('Average Response Time')

    for alg, turnaroundTimes in zip(algorithms, perAlgorithmTT):
        bottom = 0
        for i, value in enumerate(turnaroundTimes):
            ax[3].bar(alg, value, bottom=bottom, color=colors[i])
            bottom += value
        ax[3].title.set_text('Average Turnaround Time')

    for alg, contexSwitches in zip(algorithms, perAlgorithmTT):
        ax[4].bar(alg, contexSwitches)
        ax[4].title.set_text('Number of Context Switches')


    for i, label in enumerate(ax[1].get_xticklabels()):
        if i % 2 == 1:
            label.set_y(-0.02)  # Default position

    for i, label in enumerate(ax[2].get_xticklabels()):
        if i % 2 == 1:
            label.set_y(-0.02)  # Default position

    for i, label in enumerate(ax[3].get_xticklabels()):
        if i % 2 == 1:
            label.set_y(-0.02)  # Default position

    for i, label in enumerate(ax[4].get_xticklabels()):
        if i % 2 == 1:
            label.set_y(-0.02)  # Default position



    fig.tight_layout()
    plt.savefig('statistici.png')


def pipe_server():
    statistics = []


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
                # Continue using the pipe...
                break  # Exit the loop since pipe is successfully opened
            except:
                pass
        print("Connected to main pipe.")
        win32file.WriteFile(pipe, b"OK\n")

        # Connect to the feedback named pipe
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
            print("Reading from main pipe...")
            resp = win32file.ReadFile(pipe, 64 * 1024)
            line = resp[1].decode().strip()

            statistics.append(line)
            # Write to the file
            with open("test.txt", "w") as f:
                createTheOutput(statistics)
                f.write('\n'.join(statistics))

            # Send feedback
            print("Sending feedback...")
            win32file.WriteFile(pipe_feedback, b"OK\n")
        except pywintypes.error as e:
            if e.args[0] == 109:  # ERROR_BROKEN_PIPE
                print("Client disconnected.")
                break

    # Close the pipes
    win32file.CloseHandle(pipe)
    win32file.CloseHandle(pipe_feedback)
    print("Pipes closed.")

if __name__ == "__main__":
    pipe_server()
