TARGET      = QtGUIv1
TEMPLATE    = app
CONFIG 	   += c++11

QT         += widgets
QT         += network

SOURCES    += main.cpp \
    add_dialog.cpp \
    peer.cpp \
    peerthread.cpp \
    helper.cpp \
    addchannels.cpp \
    rundialog.cpp \
    rundialogstorage.cpp \
    Player.cpp
#   ../../../core/src/core/peer_dbs.cc

HEADERS    += \
    add_dialog.h \
    peer.h \
    peerthread.h \
    helper.h \
    addchannels.h \
    rundialog.h \
    globalheader.h \
    rundialogstorage.h \
    Player.h
#    ../../../core/src/core/peer_dbs.h

FORMS      += \
    add_dialog.ui \
    helper.ui \
    rundialog.ui \
    Player.ui

#LIBS       += -lVLCQtCore -lVLCQtWidgets


# Edit below for custom library location
LIBS        += -L/usr/include/boost -DBOOST_LOG_DYN_LINK -lboost_thread -lpthread -lboost_system -lboost_log_setup -lboost_log -lboost_date_time  -lboost_chrono -lboost_timer -lboost_filesystem -lboost_log_setup -lboost_program_options
LIBS        += -L/usr/lib -lvlc -lvlccore
LIBS       += -L/home/praneeth/P2PSP/core/src
LIBS       += -L/home/praneeth/P2PSP/core/bin -lp2psp
LIBS       += -L/home/praneeth/P2PSP/p2psp-qt-master/p2psp-qt/GUI/lib -lVLCQtCore -lVLCQtWidgets

INCLUDEPATH += /usr/include/boost
INCLUDEPATH += /usr/include
INCLUDEPATH += /home/praneeth/P2PSP/p2psp-qt-master/p2psp-qt/GUI/include
INCLUDEPATH +=/home/praneeth/P2PSP/core/src
INCLUDEPATH +=/home/praneeth/P2PSP/core/bin
#INCLUDEPATH +=/home/praneeth/P2PSP/p2psp-console/src


RESOURCES += \
    icons.qrc
