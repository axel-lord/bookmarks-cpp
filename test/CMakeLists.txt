set(TESTS_NAME tests)

add_executable(
	${TESTS_NAME}

	# Sources
	"./string_view_functions.cpp"
	"./line_manipulation.cpp"
	"./case.cpp"
	"./command_arguments.cpp"
)

target_link_libraries(
	${TESTS_NAME}
	PRIVATE
	Catch2::Catch2WithMain
)

target_link_libraries(
	${TESTS_NAME}
	PRIVATE
	${PROJECT_NAME}
)

target_precompile_headers(
	${TESTS_NAME}
	PRIVATE
	"./PCH.hpp"
)

target_compile_features(
	${TESTS_NAME}
	PRIVATE
	cxx_std_20
)

include(CTest)
include(Catch)
enable_testing()
catch_discover_tests(${TESTS_NAME})
