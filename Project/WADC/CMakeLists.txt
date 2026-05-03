file(GLOB_RECURSE SOURCES "Source/*.cpp" "../Tudo/Core/Source/FileSystem.cpp"
"../Tudo/Res/Source/WADMaker.cpp"
"../Tudo/Res/Source/WADFile.cpp"
"../Tudo/Core/Source/IniFile.cpp")

if (WIN32)
    add_executable (WADC ${SOURCES})
	set_target_properties(WADC PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/../GameDir)
else()
    add_executable(WADC ${SOURCES})
endif()

target_include_directories(WADC PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../Tudo/Core/Include> $<INSTALL_INTERFACE:Core>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../Tudo/Math/Include> $<INSTALL_INTERFACE:Math>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../Tudo/Res/Include> $<INSTALL_INTERFACE:Res>
)

if (WIN32)
	set_target_properties(WADC PROPERTIES
   	 	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/x64/Debug
    		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/x64/Release
    		RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/x64/RelWithDebInfo
    		RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_SOURCE_DIR}/x64/MinSizeRel
	)
	if(MSVC)
		target_compile_options(WADC PRIVATE /Zc:__cplusplus /Zc:preprocessor)
	endif()
else()
	set_target_properties(WADC PROPERTIES
   	 	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/arm64/Debug
    		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/arm64/Release
    		RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/arm64/RelWithDebInfo
    		RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_SOURCE_DIR}/arm64/MinSizeRel
	)

	find_library(COREFRAMEWORK CoreFoundation)
	find_library(IOKIT_FRAMEWORK IOKit)
	target_link_libraries(WADC PRIVATE ${COREFRAMEWORK} ${IOKIT_FRAMEWORK})
endif()

add_definitions(-DTUDO_NO_LOGGER)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET WADC PROPERTY CXX_STANDARD 20)
endif()
