cmake_minimum_required(VERSION 3.19)
project(game LANGUAGES CXX)

# ① 增加 Sql 组件
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)

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
)

# ② 链接时也加上 Qt6::Sql
target_link_libraries(game
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Sql
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
