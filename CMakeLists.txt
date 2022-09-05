cmake_minimum_required(VERSION 3.19.0)
project(bookmark_browser VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(Threads REQUIRED)
find_package(Catch2 3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

if(MSVC)
	add_compile_options(/W4 /WX)
else()
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(
	${PROJECT_NAME}
	INTERFACE
)

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
	LINKER_LANGUAGE CXX
)

target_include_directories(
	${PROJECT_NAME}
	INTERFACE
	"./include"
)

target_compile_features(
	${PROJECT_NAME}
	INTERFACE
	cxx_std_23
)

target_link_libraries(
	${PROJECT_NAME}
	INTERFACE
	fmt::fmt
)

add_subdirectory("./src")
add_subdirectory("./include")
add_subdirectory("./test")
add_subdirectory("./experimental")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
