configure_file(${CMAKE_CURRENT_SOURCE_DIR}/scripts/maketarball.sh.in ${CMAKE_CURRENT_SOURCE_DIR}/scripts/maketarball.sh @ONLY)

if(RPM_DISTRO AND RPM_DATE)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/unix/qtgiphy.spec.in ${CMAKE_CURRENT_SOURCE_DIR}/unix/qtgiphy.spec @ONLY)
endif(RPM_DISTRO AND RPM_DATE)

if(DEB_CODENAME AND DEB_DATE)
  configure_file(${CMAKE_SOURCE_DIR}/debian/changelog.in ${CMAKE_SOURCE_DIR}/debian/changelog)
endif(DEB_CODENAME AND DEB_DATE)

if (UNIX AND NOT APPLE)
  install(FILES ../data/icons/48x48/qtgiphy.png DESTINATION share/icons/hicolor/48x48/apps/)
  install(FILES ../data/icons/64x64/qtgiphy.png DESTINATION share/icons/hicolor/64x64/apps/)
  install(FILES ../data/icons/128x128/qtgiphy.png DESTINATION share/icons/hicolor/128x128/apps/)
  install(FILES unix/net.jkvinge.qtgiphy.desktop DESTINATION share/applications)
  install(FILES unix/net.jkvinge.qtgiphy.appdata.xml DESTINATION share/metainfo)
  #install(FILES unix/qtgiphy.1 DESTINATION share/man/man1)
endif (UNIX AND NOT APPLE)
