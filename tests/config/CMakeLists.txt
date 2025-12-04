message(
  STATUS "Entrando no CMakeLists.txt dos testes. ENABLE_TESTS=${ENABLE_TESTS}")
if(ENABLE_TESTS)
  # Forçar modo Release para testes
  set(CMAKE_BUILD_TYPE Release)
  if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  endif()

  add_gtest(config config_test.cpp)
  target_include_directories(config PRIVATE ${CMAKE_SOURCE_DIR})
  # target_link_libraries(config_test PRIVATE utils_static)
endif()
