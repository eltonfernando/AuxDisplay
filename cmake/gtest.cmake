# Configuração do Google Test
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1)

# Desabilitar testes do próprio googletest
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
set(BUILD_GMOCK
    ON
    CACHE BOOL "" FORCE)
set(INSTALL_GTEST
    OFF
    CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Função para adicionar testes
function(add_gtest TARGET_NAME)
  add_executable(${TARGET_NAME} ${ARGN})
  target_link_libraries(${TARGET_NAME} PRIVATE GTest::gtest_main
                                               GTest::gmock_main)

  # Forçar modo Release para testes
  set_target_properties(
    ${TARGET_NAME} PROPERTIES CMAKE_BUILD_TYPE Release CMAKE_CXX_FLAGS
                                                       "${CMAKE_CXX_FLAGS} -O3")

  # Adicionar o teste ao CTest
  add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
endfunction()
