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

target_link_libraries(demo PRIVATE ${PROJECT_NAME} spdlog::spdlog)
