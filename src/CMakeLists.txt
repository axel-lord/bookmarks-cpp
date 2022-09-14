add_executable(
	demo

	# sources
	"main.cpp"
)

target_precompile_headers(
	demo
	PRIVATE
	"./PCH.hpp"
)

target_link_libraries(demo PRIVATE ${PROJECT_NAME})

add_subdirectory("./commands")
add_subdirectory("./bookmarks")
add_subdirectory("./util")
