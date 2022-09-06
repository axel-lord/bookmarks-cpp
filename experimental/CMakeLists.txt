
add_executable(
	custom_vec
	
	# Sources
	"./custom_vec.cpp"
	"./prepare.cpp"

	#headers
	"./prepare.hpp"
	"./custom_malloc.hpp"
	"./custom_memcpy.hpp"
	"./custom_normal.hpp"
	"./custom_realloc.hpp"
	"./custom_capacity.hpp"
	"./custom_realloc_no_span.hpp"
	"./custom_realloc_no_span_simple.hpp"
	"./custom_realloc_no_span_starting_capacity.hpp"
)

target_link_libraries(
	custom_vec
	PRIVATE
	benchmark::benchmark_main
	${PROJECT_NAME}
)

target_precompile_headers(
	custom_vec
	PRIVATE
	"./PCH.hpp"
)
