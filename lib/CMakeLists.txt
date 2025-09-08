option(UTF8_STATIC_WINDOWS_RUNTIME "Use static (MT/MTd) Windows runtime" OFF)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")

set(LIBRARY_NAME utf8)

if(MSVC AND UTF8_STATIC_WINDOWS_RUNTIME)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  set(LIBRARY_NAME utf8s)
else()
  find_package(ICU REQUIRED COMPONENTS uc in dt)
endif()

add_library(${LIBRARY_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(${LIBRARY_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
if (UTF8_STATIC_WINDOWS_RUNTIME)
  target_compile_definitions(${LIBRARY_NAME} PRIVATE UTF8_STATIC_WINDOWS_RUNTIME)
endif()

if(MSVC AND UTF8_STATIC_WINDOWS_RUNTIME)
  target_link_libraries(
    ${LIBRARY_NAME} PUBLIC
    icuuc.lib
    icuin.lib
  )
else()
  target_link_libraries(
    ${LIBRARY_NAME} PUBLIC
    ICU::uc
    ICU::in
    ICU::dt
  )
endif()
