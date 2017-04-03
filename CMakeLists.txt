# Project name
project( QtGUIv1 )

# Minimum CMake version required
cmake_minimum_required(VERSION 2.8)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set ( CMAKE_BUILD_TYPE Release )

# Initialize CXXFLAGS
set(CMAKE_CXX_FLAGS                "-Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
#set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g -D__D__ -D__D_CHURN__ -D__D_LOST_CHUNKS__")
#set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os")
#set(CMAKE_CXX_FLAGS_RELEASE        "-O4")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package ( Qt5 REQUIRED )

list(APPEND BOOST_REQUIRED_COMPONENTS
  system
  filesystem
  chrono
  serialization
  timer
  thread
  log
  log_setup
  date_time
  program_options
)

find_package(Boost COMPONENTS
  ${BOOST_REQUIRED_COMPONENTS}
REQUIRED)

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

include ( ${QT_USE_FILE} )

set(P2PSP_INCLUDE_DIR ../core/src/)

link_directories(${Boost_LIBRARY_DIR})

include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTWIDGETS_INCLUDE_DIR} ${QT_QTNETWORK_INCLUDE_DIR}
  	${P2PSP_INCLUDE_DIR}
  	${Boost_INCLUDE_DIR}
  	${OPENSSL_INCLUDE_DIR}
	/home/praneeth/P2PSP/p2psp-qt-master/p2psp-qt/GUI/include
	/home/praneeth/P2PSP/core/src
	/home/praneeth/P2PSP/core/bin)
set ( QtGUIv1_HDRS
	addchannels.h
	globalheader.h
	rundialogstorage.h
	)

set ( QtGUIv1_SRCS
	main.cpp
	SimplePlayer.cpp
	add_dialog.cpp
	peer.cpp
	peerthread.cpp
	helper.cpp
	addchannels.cpp
	rundialog.cpp
	rundialogstorage.cpp
	)

set ( QtGUIv1_UIS
	SimplePlayer.ui
	add_dialog.ui
	helper.ui
	rundialog.ui
	)
QT4_WRAP_UI(UIS ${QtGUIv1_UIS})

set ( QtGUIv1_RSCS
	icons.qrc
	)
QT4_ADD_RESOURCES(RSCS ${QtGUIv1_RSCS})

set ( QtGUIv1_MOCS
	SimplePlayer.h
	add_dialog.h
	peer.h
	peerthread.h
	helper.h
	rundialog.h
	)
QT4_WRAP_CPP(MOCS ${QtGUIv1_MOCS})

add_executable ( QtGUIv1 ${QtGUIv1_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( QtGUIv1  ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTWIDGETS_LIBRARY} ${QT_QTNETWORK_LIBRARY} )
