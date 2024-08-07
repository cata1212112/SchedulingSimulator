cmake_minimum_required(VERSION 3.27)
project(Licenta)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.7.0/mingw_64/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Licenta main.cpp
        Pages/MainWindow/mainwindow.cpp
        Pages/MainWindow/mainwindow.h
        Pages/MainWindow/mainwindow.ui
        DiscreteEventSimulator/Event/Event.cpp
        DiscreteEventSimulator/DES.cpp
        Process/Process.cpp
        Utils/Random.cpp
        Utils/ImplementedAlgorithms.h
        Utils/Style.h
        Scheduler/FIFO/FIFO.cpp
        Scheduler/FIFO/FIFO.h
        Utils/WrapperPriorityQueue.h
        Utils/Metrics.h
        Scheduler/RR/RR.cpp
        Scheduler/RR/RR.h
        Scheduler/RR/RR.h
        Scheduler/SJF/SJF.cpp
        Scheduler/SJF/SJF.h
        Scheduler/SRTF/SRTF.cpp
        Scheduler/SRTF/SRTF.h
        Scheduler/CompletlyFairScheduler/CFS.cpp
        Scheduler/CompletlyFairScheduler/CFS.h
        Scheduler/CompletlyFairScheduler/SingleCoreCFS.cpp
        Scheduler/CompletlyFairScheduler/SingleCoreCFS.h
        DiscreteEventSimulator/Core/Core.cpp
        DiscreteEventSimulator/Core/Core.h
        Scheduler/TestMultiCore1/TestMultiCore1.cpp
        Scheduler/TestMultiCore1/TestMultiCore1.h
        Scheduler/EDRR/EDRR.cpp
        Scheduler/EDRR/EDRR.h
        Scheduler/ETRR/MMRR.cpp
        Scheduler/ETRR/MMRR.h
        Scheduler/MTSJ/MTSJ.cpp
        Scheduler/MTSJ/MTSJ.h
        Scheduler/RRKMEANS/MinMaxRR.cpp
        Scheduler/RRKMEANS/MinMaxRR.h
        Utils/KMEANS/KMEANS.cpp
        Utils/KMEANS/KMEANS.h
        Scheduler/TestMultiCore2/TestMultiCore2.cpp
        Scheduler/TestMultiCore2/TestMultiCore2.h
        Scheduler/RealTime/LSTR/LSTR.cpp
        Scheduler/RealTime/LSTR/LSTR.h
        Scheduler/RealTime/GEDF/GEDF.cpp
        Scheduler/RealTime/GEDF/GEDF.h
        Scheduler/RealTime/GEDF/GEDFCore/GEDFCore.cpp
        Scheduler/RealTime/GEDF/GEDFCore/GEDFCore.h
        Utils/UUniFastDiscard.h
        Scheduler/RealTime/LSTR/LSTRCore/LSTRCore.cpp
        Scheduler/RealTime/LSTR/LSTRCore/LSTRCore.h
        Utils/KMEANS/DataPoint.cpp
        Utils/KMEANS/DataPoint.h
        Scheduler/FairShareScheduling/FSS.cpp
        Scheduler/FairShareScheduling/FSS.h
        Scheduler/FairShareScheduling/FSSCore.cpp
        Scheduler/FairShareScheduling/FSSCore.h
        Scheduler/RealTime/LTS/LTS.cpp
        Scheduler/RealTime/LTS/LTS.h
        Scheduler/RealTime/LTS/LTSCore/LTSCore.cpp
        Scheduler/RealTime/LTS/LTSCore/LTSCore.h
        Scheduler/RealTime/DARTS/DARTSCore/DARTSCore.cpp
        Scheduler/RealTime/DARTS/DARTSCore/DARTSCore.h
        Scheduler/RealTime/DARTS/DARTS.cpp
        Scheduler/RealTime/DARTS/DARTS.h
)
target_link_libraries(Licenta
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
