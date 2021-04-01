set(SOURCES
  logging.cpp
  iconloader.cpp
  mainwindow.cpp
  aboutdialog.cpp
  busyindicator.cpp
  qsearchfield.cpp
  gificonmodel.cpp
  gificonview.cpp
  gificonitem.cpp
  gificonviewdelegate.cpp
)

set(HEADERS
 mainwindow.h
 aboutdialog.h
 busyindicator.h
 qsearchfield.h
 gificonmodel.h
 gificonview.h
 gificonviewdelegate.h
)

set(UI mainwindow.ui aboutdialog.ui)
set(RESOURCES ../data/data.qrc ../data/icons.qrc)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)

if(Qt6Core_FOUND)
  qt6_wrap_cpp(MOC ${HEADERS})
  qt6_wrap_ui(UIC ${UI})
  qt6_add_resources(QRC ${RESOURCES})
else()
  qt5_wrap_cpp(MOC ${HEADERS})
  qt5_wrap_ui(UIC ${UI})
  qt5_add_resources(QRC ${RESOURCES})
endif()

add_library(qtgiphy_lib STATIC
  ${SOURCES}
  ${MOC}
  ${UIC}
  ${QRC}
  ${POT}
  ${PO}
  ${OTHER_UIC_SOURCES}
)

target_include_directories(qtgiphy_lib PUBLIC
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(qtgiphy_lib PUBLIC ${QT_LIBRARIES})

if(WIN32)
  target_link_libraries(qtgiphy_lib PUBLIC regex)
endif()
if(MSVC)
  target_link_libraries(qtgiphy_lib PUBLIC legacy_stdio_definitions)
endif()

set(EXECUTABLE_OUTPUT_PATH ..)

# Show the console window in debug mode on Windows
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT ENABLE_WIN32_CONSOLE)
  set(QTGIPHY-WIN32-FLAG WIN32)
endif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT ENABLE_WIN32_CONSOLE)

# Resource file for windows
if(WIN32)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../dist/windows/windres.rc.in ${CMAKE_CURRENT_BINARY_DIR}/windres.rc)
  set(QTGIPHY-WIN32-RESOURCES windres.rc)
endif(WIN32)

add_executable(qtgiphy ${QTGIPHY-WIN32-FLAG} ${QTGIPHY-WIN32-RESOURCES} main.cpp)

target_link_libraries(qtgiphy PRIVATE qtgiphy_lib)

install(TARGETS qtgiphy RUNTIME DESTINATION bin)
