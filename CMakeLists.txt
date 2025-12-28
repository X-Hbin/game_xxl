cmake_minimum_required(VERSION 3.19)
project(game LANGUAGES CXX)

# ① 增加 Sql 组件
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)
#network
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

qt_standard_project_setup()

qt_add_executable(game
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    menu.h menu.cpp menu.ui
    mode_1.h mode_1.cpp mode_1.ui
    gameboard.h gameboard.cpp
    mode_2.h mode_2.cpp mode_2.ui
    mode_ai.h mode_ai.cpp mode_ai.ui
    musicmanager.h
    musicmanager.cpp
    mode_3.cpp
    mode_3.h
    mode_3.ui
    musicsetting.cpp
    musicsetting.h
    musicsetting.ui


    skilltree.cpp
    skilltree.h
    skilltreepage.cpp
    skilltreepage.h
    skilltreepage.ui
    rankmanager.cpp
    rankmanager.h
    rankpage.cpp
    rankpage.h
    rankpage.ui

    online_menu.ui
    online_menu.h online_menu.cpp
    online_game.ui
    online_game.h online_game.cpp
    networkmanager.h networkmanager.cpp
)

# ② 链接时也加上 Qt6::Sql
target_link_libraries(game
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Sql
        winmm  # 仅新增这一行：链接Windows多媒体库，解决PlaySoundW报错
)

target_link_libraries(game PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network  # 添加网络库链接
)

include(GNUInstallDirs)
install(TARGETS game
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET game
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
