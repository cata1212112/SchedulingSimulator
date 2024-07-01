//
// Created by Cata on 2/27/2024.
//

#ifndef LICENTA_MAINWINDOW_H
#define LICENTA_MAINWINDOW_H

#include <windows.h>

#include <QMainWindow>
#include <QVBoxLayout>
#include "../../Utils/Style.h"
#include "../../Utils/Metrics.h"
#include "../../DiscreteEventSimulator/DES.h"
#include "../../Utils/UUniFastDiscard.h"
#include <regex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    std::string selectedAlgorithm;
    int selectedNumberOfCores = 1;
    int quantum = 1;
    QWidget *lastWidget;

    void handleSingleCoreButton();

    void handleMultiCoreButton();

    void handleRealTimeButton();

    void gotoRunning(DES *des, int numCores);

    QWidget* getPlotFromPythonScript(std::string scriptName, std::string imageName, std::string parameters);

    void deleteContent(QWidget *widget);

    void selectNumberOfCores();

    void selectAlgortihmPage(QWidget *parent, const std::vector<std::string>& algorithms, QLayout *layout, bool isMultiCore, bool isRealTime);

    void selectedAlgorithmButton(const std::string& algorithmName, QLayout* layout, bool isMultiCore= false);

    void selectedAlgorithmButtonRealTime(const std::string& algorithmName, QLayout* layout);

    void goBackButton(QLayout* layout, QWidget* parent);

    void selectAlgortihm(const std::string &algortihm, bool isMultiCore = false);

    void clearWidgets(QLayout *layout);

    void setupInputData(QWidget *parent, bool isMultiCore = false);

    void generateDataButton(QLayout *layout, QWidget *parent, bool isMultiCore = false);

    void sendInstructionToPipeAndWait(string op, string package) {
        HANDLE hPipe = CreateNamedPipe(
                L"\\\\.\\pipe\\plotting_pipe",
                PIPE_ACCESS_DUPLEX,
                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                1, 65536, 65536,
                0,
                NULL
        );

        HANDLE hFeedbackPipe = CreateNamedPipe(
                L"\\\\.\\pipe\\plotting_pipe_feedback",
                PIPE_ACCESS_DUPLEX,
                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                1, 65536, 65536,
                0,
                NULL
        );

        if (hPipe == INVALID_HANDLE_VALUE) {
            std::cerr << "Failed to create main pipe. " << std::endl;
            std::cerr << GetLastError() << "\n";
        }

        if (hFeedbackPipe == INVALID_HANDLE_VALUE) {
            std::cerr << "Failed to create feedback pipe." << std::endl;
            std::cerr << GetLastError() << "\n";

            CloseHandle(hPipe);
        }
        ConnectNamedPipe(hPipe, NULL);
        DWORD dwWritten;

        string toSend = op + "\n" + package;
        if (!WriteFile(hPipe, toSend.c_str(), toSend.length(), &dwWritten, NULL)) {
            std::cerr << "Failed to write to main pipe." << std::endl;
            std::cerr << GetLastError() << std::endl;

            CloseHandle(hPipe);
            CloseHandle(hFeedbackPipe);
            return;
        }
        cout << "trimis package\n";

        ConnectNamedPipe(hFeedbackPipe, NULL);

        char buffer[128];
        DWORD dwRead;
        if (ReadFile(hFeedbackPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL)) {
            buffer[dwRead] = '\0';
            std::cout << "Received: " << buffer << std::endl;
        } else {
            std::cerr << "Failed to read from feedback pipe." << std::endl;
            std::cerr << GetLastError() << "\n";
        }

        CloseHandle(hPipe);
        CloseHandle(hFeedbackPipe);
    }

};


#endif //LICENTA_MAINWINDOW_H
