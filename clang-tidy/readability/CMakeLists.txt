set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidyReadability
  NamespaceCommentCheck.cpp

  LINK_LIBS
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  clangTidy
  clangTooling
  )
