find_package(benchmark CONFIG REQUIRED)

add_executable(
	custom_vec
	
	# Sources
	"./custom_vec.cpp"
)

target_link_libraries(
	custom_vec
	PRIVATE
	fmt::fmt
	benchmark::benchmark_main
	${PROJECT_NAME}
)