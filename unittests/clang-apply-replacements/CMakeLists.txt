get_filename_component(ClangApplyReplacementsLocation
  "${CMAKE_CURRENT_SOURCE_DIR}/../../clang-apply-replacements/include" REALPATH)
get_filename_component(CommonIncLocation
  "${CMAKE_CURRENT_SOURCE_DIR}/../include" REALPATH)
include_directories(
  ${ClangApplyReplacementsLocation}
  ${CommonIncLocation}
  )

add_extra_unittest(ClangApplyReplacementsTests
  ReformattingTest.cpp
  )

target_link_libraries(ClangApplyReplacementsTests
  clangApplyReplacements
  clangTooling
  )
