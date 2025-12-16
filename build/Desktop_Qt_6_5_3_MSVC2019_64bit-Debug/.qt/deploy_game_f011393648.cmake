include(D:/Qt/lostground/game_xxl/build/Desktop_Qt_6_5_3_MSVC2019_64bit-Debug/.qt/QtDeploySupport.cmake)
include("${CMAKE_CURRENT_LIST_DIR}/game-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_ALL_MODULES_FOUND_VIA_FIND_PACKAGE "ZlibPrivate;EntryPointPrivate;Core;Gui;Widgets;Sql")

qt6_deploy_runtime_dependencies(
    EXECUTABLE D:/Qt/lostground/game_xxl/build/Desktop_Qt_6_5_3_MSVC2019_64bit-Debug/game.exe
    GENERATE_QT_CONF
)
