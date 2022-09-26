set(DEMO_NAME demo)

add_executable(
	${DEMO_NAME}
	"main.cpp"
)

target_precompile_headers(
	${DEMO_NAME}
	PRIVATE
	"./PCH.hpp"
)

target_compile_features(
	${DEMO_NAME}
	PRIVATE
	cxx_std_20
)


target_link_libraries(
	${DEMO_NAME}
	PRIVATE
	${PROJECT_NAME}
)

add_subdirectory("./commands")
add_subdirectory("./bookmarks")
add_subdirectory("./util")
