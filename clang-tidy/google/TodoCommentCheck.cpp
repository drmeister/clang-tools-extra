//===--- TodoCommentCheck.cpp - clang-tidy --------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "TodoCommentCheck.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"

namespace clang {
namespace tidy {
namespace readability {

class TodoCommentCheck::TodoCommentHandler : public CommentHandler {
public:
  TodoCommentHandler(TodoCommentCheck &Check, llvm::Optional<std::string> User)
      : Check(Check), User(User ? *User : "unknown"),
        TodoMatch("^// *TODO(\\(.*\\))?:?( )?(.*)$") {}

  bool HandleComment(Preprocessor &PP, SourceRange Range) override {
    StringRef Text =
        Lexer::getSourceText(CharSourceRange::getCharRange(Range),
                             PP.getSourceManager(), PP.getLangOpts());

    SmallVector<StringRef, 4> Matches;
    if (!TodoMatch.match(Text, &Matches))
      return false;

    StringRef Username = Matches[1];
    StringRef Comment = Matches[3];

    if (!Username.empty())
      return false;

    std::string NewText = ("// TODO(" + Twine(User) + "): " + Comment).str();

    Check.diag(Range.getBegin(), "missing username/bug in TODO")
        << FixItHint::CreateReplacement(CharSourceRange::getCharRange(Range),
                                        NewText);
    return false;
  }

private:
  TodoCommentCheck &Check;
  std::string User;
  llvm::Regex TodoMatch;
};

TodoCommentCheck::TodoCommentCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context),
      Handler(llvm::make_unique<TodoCommentHandler>(
          *this, Context->getOptions().User)) {}

void TodoCommentCheck::registerPPCallbacks(CompilerInstance &Compiler) {
  Compiler.getPreprocessor().addCommentHandler(Handler.get());
}

} // namespace readability
} // namespace tidy
} // namespace clang
