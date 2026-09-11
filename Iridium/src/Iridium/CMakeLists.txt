target_sources(${PROJECT_NAME} PRIVATE
	vector.cpp
	render_target.cpp
	application_window.cpp
	sub_window.cpp
	random.cpp
	time.cpp
	state_machine.cpp
	application.cpp
	log.cpp
	colors.cpp
	# resources.cpp
)

add_subdirectory(rendering)
add_subdirectory(input)
add_subdirectory(assets)
add_subdirectory(vgui)