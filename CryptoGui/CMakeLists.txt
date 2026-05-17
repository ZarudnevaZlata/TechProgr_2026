cmake_minimum_required(VERSION 3.19)
project(CryptoGui LANGUAGES CXX)

add_compile_definitions(_WIN32_WINNT=0x0A00)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

qt_add_executable(CryptoGui
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    ApiClient.cpp
    ApiClient.h
)

target_include_directories(CryptoGui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(CryptoGui
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
)

install(TARGETS CryptoGui
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET CryptoGui
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})