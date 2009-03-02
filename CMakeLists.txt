project(archlinux-torrent-utils)

find_package(KDE4 REQUIRED)
find_package(BTCore REQUIRED)

include_directories(${KDE4_INCLUDES} ${BTCORE_INCLUDE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}")

add_executable(mktorrent mktorrent.cpp)
target_link_libraries(mktorrent ${BTCORE_LIBRARIES})
install(TARGETS mktorrent DESTINATION bin)

add_executable(lstorrent lstorrent.cpp)
target_link_libraries(lstorrent ${BTCORE_LIBRARIES})
install(TARGETS lstorrent DESTINATION bin)

#install(PROGRAMS mktorrent-archiso DESTINATION bin)
