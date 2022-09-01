add_executable(
	demo

	# sources
	"main.cpp"
)

target_precompile_headers(
	demo
	PRIVATE
	"PCH.hpp"
)

target_link_libraries(demo ${PROJECT_NAME})