cmake_minimum_required(VERSION 3.19)
project(game LANGUAGES CXX)

<<<<<<< HEAD
# ① 增加 Sql 组件
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)

=======
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)
>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d
qt_standard_project_setup()

qt_add_executable(game
    WIN32 MACOSX_BUNDLE
    main.cpp
<<<<<<< HEAD
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    menu.h menu.cpp menu.ui
    mode_1.h mode_1.cpp mode_1.ui
    gameboard.h gameboard.cpp
    mode_2.h mode_2.cpp mode_2.ui
    musicmanager.h
    musicmanager.cpp
    mode_3.cpp
    mode_3.h
    mode_3.ui
    musicsetting.cpp
    musicsetting.h
    musicsetting.ui
    skillmanager.cpp
    skillmanager.h
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
    mode_ai.h mode_ai.cpp mode_ai.ui
)

# ② 链接时也加上 Qt6::Sql
target_link_libraries(game
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Sql
        winmm  # 仅新增这一行：链接Windows多媒体库，解决PlaySoundW报错
=======
    mainwindow.cpp mainwindow.h mainwindow.ui
    menu.cpp menu.h menu.ui
    mode_1.cpp mode_1.h mode_1.ui
    mode_2.cpp mode_2.h mode_2.ui
    gameboard.h gameboard.cpp
    skilltree.cpp skilltree.h          # 新增
    skilltreepage.cpp skilltreepage.h  # 新增
    mode_3.cpp mode_3.h
    mode_3.ui                # 新增
)

target_link_libraries(game PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d
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
