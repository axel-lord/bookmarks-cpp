cmake_minimum_required(VERSION 3.19.0)
project(bookmark_browser VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(Threads REQUIRED)
find_package(Catch2 3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)

if(MSVC)
	add_compile_options(/W4 /WX)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -Werror)
	add_compile_options(-Wconversion -Wshadow -Wdouble-promotion -Wnonnull -Wnull-dereference -Wduplicated-branches)
endif()

add_library(
	${PROJECT_NAME}
)

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
	LINKER_LANGUAGE CXX
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC	
	"./include"
)

target_compile_features(
	${PROJECT_NAME}
	PUBLIC
	cxx_std_20
)

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC
	fmt::fmt
	spdlog::spdlog
	range-v3::range-v3
)

add_subdirectory("./src")
add_subdirectory("./include")
add_subdirectory("./test")
# add_subdirectory("./experimental")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
