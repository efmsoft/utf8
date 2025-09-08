add_executable(StringTest Convert.cpp Split.cpp StringTest.cpp Template.cpp) 

target_compile_definitions(StringTest PUBLIC _CRT_SECURE_NO_WARNINGS)
target_link_libraries(StringTest LINK_PUBLIC utf8 gtest_main) 

if(WIN32)
  target_link_libraries(StringTest LINK_PUBLIC icuuc.lib icuin.lib) 
elseif(APPLE)
  target_link_libraries(StringTest LINK_PUBLIC
    iconv
    "-framework Cocoa"
  ) 
endif()

set_target_properties(StringTest PROPERTIES FOLDER "Tests")

include(GoogleTest)
gtest_discover_tests(StringTest)
