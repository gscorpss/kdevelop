/* This file is part of KDevelop
    Copyright 2002-2005 Roberto Raggi <roberto@kdevelop.org>
    Copyright 2007-2008 David Nolden <david.nolden.kdevelop@art-master.de>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef AST_H
#define AST_H

#include "listnode.h"

#define DECLARE_AST_NODE(k) \
    enum { __node_kind = Kind_##k };

class ParseSession;
namespace KDevelop
{
class DUContext;
}
class AccessSpecifierAST;
class AsmDefinitionAST;
class BaseClauseAST;
class BaseSpecifierAST;
class BinaryExpressionAST;
class BracedInitListAST;
class CastExpressionAST;
class ClassMemberAccessAST;
class ClassSpecifierAST;
class CompoundStatementAST;
class ConditionAST;
class ConditionalExpressionAST;
class CppCastExpressionAST;
class CtorInitializerAST;
class DeclarationAST;
class DeclarationStatementAST;
class DeclaratorAST;
class DeleteExpressionAST;
class DoStatementAST;
class Elaborated;
class EnumSpecifierAST;
class EnumeratorAST;
class ExceptionSpecificationAST;
class ExpressionAST;
class ExpressionOrDeclarationStatementAST;
class ExpressionStatementAST;
class ForStatementAST;
class ForRangeDeclarationAst;
class FunctionCallAST;
class FunctionDefinitionAST;
class IfStatementAST;
class IncrDecrExpressionAST;
class InitDeclaratorAST;
class InitializerAST;
class InitializerListAST;
class InitializerClauseAST;
class LabeledStatementAST;
class LambdaExpressionAST;
class LambdaCaptureAST;
class LambdaDeclaratorAST;
class LinkageBodyAST;
class LinkageSpecificationAST;
class MemInitializerAST;
class NameAST;
class NamespaceAST;
class NamespaceAliasDefinitionAST;
class NewDeclaratorAST;
class NewExpressionAST;
class NewInitializerAST;
class NewTypeIdAST;
class OperatorAST;
class OperatorFunctionIdAST;
class ParameterDeclarationAST;
class ParameterDeclarationClauseAST;
class PostfixExpressionAST;
class PrimaryExpressionAST;
class PtrOperatorAST;
class PtrToMemberAST;
class ReturnStatementAST;
class SimpleDeclarationAST;
class SimpleTypeSpecifierAST;
class SizeofExpressionAST;
class StatementAST;
class StringLiteralAST;
class SubscriptExpressionAST;
class SwitchStatementAST;
class TemplateArgumentAST;
class TemplateDeclarationAST;
class TemplateParameterAST;
class ThrowExpressionAST;
class TrailingReturnTypeAST;
class TranslationUnitAST;
class TryBlockStatementAST;
class CatchStatementAST;
class TypeIdAST;
class TypeIDOperatorAST;
class TypeIdentificationAST;
class TypeParameterAST;
class TypeSpecifierAST;
class TypedefAST;
class UnaryExpressionAST;
class UnqualifiedNameAST;
class UsingAST;
class UsingDirectiveAST;
class WhileStatementAST;
class WinDeclSpecAST;

class AST
{
public:
  /**
   * @WARNING: When adding new nodes here, you also need to modify the following places:
   * - names[] array in dumptree.cpp
   * - add visitXYZ to visitor.h and visitor.cpp, esp. also to the Visitor::_S_table
   * - overload visitXYZ impl to default_visitor.cpp
   */
  enum NODE_KIND
    {
      Kind_UNKNOWN = 0,

      Kind_AccessSpecifier,                     // 1
      Kind_AsmDefinition,                       // 2
      Kind_BaseClause,                          // 3
      Kind_BaseSpecifier,                       // 4
      Kind_BinaryExpression,                    // 5
      Kind_CastExpression,                      // 6
      Kind_ClassMemberAccess,                   // 7
      Kind_ClassSpecifier,                      // 8
      Kind_CompoundStatement,                   // 9
      Kind_Condition,                           // 10
      Kind_ConditionalExpression,               // 11
      Kind_CppCastExpression,                   // 12
      Kind_CtorInitializer,                     // 13
      Kind_DeclarationStatement,                // 14
      Kind_Declarator,                          // 15
      Kind_DeleteExpression,                    // 16
      Kind_DoStatement,                         // 17
      Kind_ElaboratedTypeSpecifier,             // 18
      Kind_EnumSpecifier,                       // 19
      Kind_Enumerator,                          // 20
      Kind_ExceptionSpecification,              // 21
      Kind_ExpressionOrDeclarationStatement,    // 22
      Kind_ExpressionStatement,                 // 23
      Kind_ForStatement,                        // 24
      Kind_FunctionCall,                        // 25
      Kind_FunctionDefinition,                  // 26
      Kind_IfStatement,                         // 27
      Kind_IncrDecrExpression,                  // 28
      Kind_InitDeclarator,                      // 29
      Kind_Initializer,                         // 30
      Kind_InitializerClause,                   // 31
      Kind_LabeledStatement,                    // 32
      Kind_LinkageBody,                         // 33
      Kind_LinkageSpecification,                // 34
      Kind_MemInitializer,                      // 35
      Kind_Name,                                // 36
      Kind_Namespace,                           // 37
      Kind_NamespaceAliasDefinition,            // 38
      Kind_NewDeclarator,                       // 39
      Kind_NewExpression,                       // 40
      Kind_NewInitializer,                      // 41
      Kind_NewTypeId,                           // 42
      Kind_Operator,                            // 43
      Kind_OperatorFunctionId,                  // 44
      Kind_ParameterDeclaration,                // 45
      Kind_ParameterDeclarationClause,          // 46
      Kind_PostfixExpression,                   // 47
      Kind_PrimaryExpression,                   // 48
      Kind_PtrOperator,                         // 49
      Kind_PtrToMember,                         // 50
      Kind_ReturnStatement,                     // 51
      Kind_SimpleDeclaration,                   // 52
      Kind_SimpleTypeSpecifier,                 // 53
      Kind_SizeofExpression,                    // 54
      Kind_StringLiteral,                       // 55
      Kind_SubscriptExpression,                 // 56
      Kind_SwitchStatement,                     // 57
      Kind_TemplateArgument,                    // 58
      Kind_TemplateDeclaration,                 // 59
      Kind_TemplateParameter,                   // 60
      Kind_ThrowExpression,                     // 61
      Kind_TranslationUnit,                     // 62
      Kind_TryBlockStatement,                   // 63
      Kind_CatchStatement,                      // 64
      Kind_TypeId,                              // 65
      Kind_TypeIdentification,                  // 66
      Kind_TypeParameter,                       // 67
      Kind_Typedef,                             // 68
      Kind_UnaryExpression,                     // 69
      Kind_UnqualifiedName,                     // 70
      Kind_Using,                               // 71
      Kind_UsingDirective,                      // 72
      Kind_WhileStatement,                      // 73
      Kind_WinDeclSpec,                         // 74
      Kind_Comment,                             // 75
      Kind_JumpStatement,                       // 76
      Kind_SignalSlotExpression,                // 77
      Kind_QPropertyDeclaration,                // 78
      Kind_ForRangeDeclaration,                 // 79
      Kind_TypeIDOperator,                      // 80
      Kind_StaticAssert,                        // 81
      Kind_TrailingReturnType,                  // 82
      Kind_LambdaExpression,                    // 83
      Kind_LambdaCapture,                       // 84
      Kind_LambdaDeclarator,                    // 85
      Kind_InitializerList,                     // 86
      Kind_BracedInitList,                      // 87
      Kind_AliasDeclaration,                    // 88
      NODE_KIND_COUNT
    };
  AST() :kind(0), start_token(0), end_token(0), ducontext(nullptr) {}
  int kind;

  uint start_token;
  uint end_token;

  /** Context opened by this AST
    *This is usually not filled for all AST's, only for those that open a new context
    */
  KDevelop::DUContext* ducontext;
};

class CommentAST
{
public:

  const ListNode<uint> *comments; //A list of comment-tokens
};

class TypeSpecifierAST : public AST
{
public:

  const ListNode<uint> *cv; // const or volatile tokens
};

class StatementAST : public AST
{
public:

};

class ExpressionAST : public AST
{
public:

};

class DeclarationAST : public AST, public CommentAST
{
public:

};

class AccessSpecifierAST : public DeclarationAST
{
public:

  DECLARE_AST_NODE(AccessSpecifier)

  const ListNode<uint> *specs;
};

class AsmDefinitionAST : public DeclarationAST
{
public:

  DECLARE_AST_NODE(AsmDefinition)

  const ListNode<uint> *cv;
};

class BaseClauseAST : public AST // ### kill me
{
public:

  DECLARE_AST_NODE(BaseClause)

  const ListNode<BaseSpecifierAST*> *base_specifiers;
};

class BaseSpecifierAST : public AST
{
public:

  DECLARE_AST_NODE(BaseSpecifier)

  uint virt;
  uint access_specifier;
  NameAST *name;
  bool isVariadic;
};

class BinaryExpressionAST : public ExpressionAST
{
public:

  DECLARE_AST_NODE(BinaryExpression)

  uint op; //Index of the token that describes the operator
  ExpressionAST *left_expression;
  ExpressionAST *right_expression;
};

///An expression used to do more detailed processing of SIGNAL(..) and SLOT(..) specifications
class SignalSlotExpressionAST : public ExpressionAST {
public:

  DECLARE_AST_NODE(SignalSlotExpression)
  //The unqualified name also contains the argument types in its template-arguments
  UnqualifiedNameAST *name;
};

// An node used to do more detailed processing of Q_PROPERTY specifications
class QPropertyDeclarationAST : public DeclarationAST
{
public:

  DECLARE_AST_NODE(QPropertyDeclaration)

  TypeSpecifierAST *type;
  const ListNode<PtrOperatorAST*> *ptr_ops;
  NameAST *name;

  /// MEMBER, since Qt 5.1
  NameAST *member;
  NameAST *getter;
  NameAST *setter;
  NameAST *resetter;
  NameAST *notifier;

  NameAST *designableMethod;
  NameAST *scriptableMethod;
};

class CastExpressionAST : public ExpressionAST
{
public:

  DECLARE_AST_NODE(CastExpression)

  TypeIdAST *type_id;
  ExpressionAST *expression;
};

class ClassMemberAccessAST : public ExpressionAST
{
public:

  DECLARE_AST_NODE(ClassMemberAccess)

  uint op; //Index of the token that describes the operator
  NameAST *name;
};

class ClassSpecifierAST : public TypeSpecifierAST
{
public:

  DECLARE_AST_NODE(ClassSpecifier)

  WinDeclSpecAST *win_decl_specifiers;
  uint class_key;
  NameAST *name;
  BaseClauseAST *base_clause;
  const ListNode<DeclarationAST*> *member_specs;
  const ListNode<uint> *virt_specifiers;
};

class CompoundStatementAST : public StatementAST
{
public:

  DECLARE_AST_NODE(CompoundStatement)

  const ListNode<StatementAST*> *statements;
};

class ConditionAST : public AST
{
public:

  DECLARE_AST_NODE(Condition)

  TypeSpecifierAST *type_specifier;
  DeclaratorAST *declarator;
  ExpressionAST *expression;
};

class ConditionalExpressionAST : public ExpressionAST
{
public:

  DECLARE_AST_NODE(ConditionalExpression)

  ExpressionAST *condition;
  ExpressionAST *left_expression;
  ExpressionAST *right_expression;
};

/**
 * type_id is the type that should be casted to
 * expression is the expression casted from
 * sub_expressions is a list of post-fix expressions, see PostfixExpressionAST
 */
class CppCastExpressionAST : public ExpressionAST
{
public:

  DECLARE_AST_NODE(CppCastExpression)

  uint op; //Index of the token that describes the operator
  TypeIdAST *type_id;
  ExpressionAST *expression;
  const ListNode<ExpressionAST*> *sub_expressions;
};

class CtorInitializerAST : public AST
{
public:

  DECLARE_AST_NODE(CtorInitializer)

  uint colon;
  const ListNode<MemInitializerAST*> *member_initializers;
};

class DeclarationStatementAST : public StatementAST
{
public:

  DECLARE_AST_NODE(DeclarationStatement)

  DeclarationAST *declaration;
};

class DeclaratorAST : public AST
{
public:

  DECLARE_AST_NODE(Declarator)

  const ListNode<PtrOperatorAST*> *ptr_ops;
  DeclaratorAST *sub_declarator;
  NameAST *id;
  ExpressionAST *bit_expression;
  const ListNode<ExpressionAST*> *array_dimensions;

  ParameterDeclarationClauseAST *parameter_declaration_clause;
  const ListNode<uint> *fun_cv;
  ExceptionSpecificationAST *exception_spec;
  TrailingReturnTypeAST* trailing_return_type;
  const ListNode<uint> *virt_specifiers;
  // either & or && token or null
  uint ref_qualifier;
  bool isVariadic;
};

class DeleteExpressionAST : public ExpressionAST
{
public:

  DECLARE_AST_NODE(DeleteExpression)

  uint scope_token;
  uint delete_token;
  uint lbracket_token;
  uint rbracket_token;
  ExpressionAST *expression;
};

class DoStatementAST : public StatementAST
{
public:

  DECLARE_AST_NODE(DoStatement)

  StatementAST *statement;
  ExpressionAST *expression;
};

class ElaboratedTypeSpecifierAST : public TypeSpecifierAST
{
public:

  DECLARE_AST_NODE(ElaboratedTypeSpecifier)

  uint type;
  NameAST *name;
  bool isDeclaration; //Whether this type-specifier is a forward declaration rather than use use
};

class EnumSpecifierAST : public TypeSpecifierAST
{
public:

  DECLARE_AST_NODE(EnumSpecifier)

  NameAST *name;
  TypeSpecifierAST* type;
  const ListNode<EnumeratorAST*> *enumerators;
  bool isClass : 1;
  bool isOpaque : 1;
};

class EnumeratorAST : public AST, public  CommentAST
{
public:

  DECLARE_AST_NODE(Enumerator)

  uint id;
  ExpressionAST *expression;
};

class ExceptionSpecificationAST : public AST
{
public:

  DECLARE_AST_NODE(ExceptionSpecification)

  // when type_ids _and_ ellipsis is not null,
  // the exception spec is variadic (pack expansion)
  uint ellipsis;
  const ListNode<TypeIdAST*> *type_ids;

  // noexcept token
  uint no_except;
  // if set, this is actually a noexcept(...) expression
  // and the ast node below points to the ...
  ExpressionAST *noexcept_expression;
};

class ExpressionOrDeclarationStatementAST : public StatementAST
{
public:

  DECLARE_AST_NODE(ExpressionOrDeclarationStatement)

  StatementAST *expression;
  StatementAST *declaration;

  // This was not originally part of the AST - added by the context visitor
  bool expressionChosen;
};

///An expression terminated by a semicolon or similar
class ExpressionStatementAST : public StatementAST
{
public:

  DECLARE_AST_NODE(ExpressionStatement)

  ExpressionAST *expression;
};

class FunctionCallAST : public ExpressionAST
{
public:

  DECLARE_AST_NODE(FunctionCall)

  ExpressionAST *arguments;
  bool isVariadic;
};

class FunctionDefinitionAST : public DeclarationAST
{
public:

  DECLARE_AST_NODE(FunctionDefinition)

  const ListNode<uint> *storage_specifiers;
  const ListNode<uint> *function_specifiers;
  TypeSpecifierAST *type_specifier;
  DeclaratorAST *declarator;
  StatementAST *function_body;
  WinDeclSpecAST *win_decl_specifiers;
  CtorInitializerAST *constructor_initializers;

  // support for = default or = deleted functions
  enum DefaultDeleted {
    NotDefaultOrDeleted,
    Default,
    Deleted
  };
  DefaultDeleted defaultDeleted;
};

class ForStatementAST : public StatementAST
{
public:

  DECLARE_AST_NODE(ForStatement)

  // either init_statement == 0 or range_declaration == 0

  // c-style for
  StatementAST *init_statement;
  ConditionAST *condition;

  // range-based for
  ForRangeDeclarationAst *range_declaration;

  // shared between both for
  ExpressionAST *expression;

  // body
  StatementAST *statement;
};

class ForRangeDeclarationAst : public DeclarationAST
{
public:

  DECLARE_AST_NODE(ForRangeDeclaration)

  const ListNode<uint> *storage_specifiers;
  TypeSpecifierAST *type_specifier;
  DeclaratorAST *declarator;
};

class IfStatementAST : public StatementAST
{
public:

  DECLARE_AST_NODE(IfStatement)

  ConditionAST *condition;
  StatementAST *statement;
  StatementAST *else_statement;
};

class IncrDecrExpressionAST : public ExpressionAST
{
public:

  DECLARE_AST_NODE(IncrDecrExpression)

  uint op; //Index of the token that describes the operator
};

class InitDeclaratorAST : public AST
{
public:

  DECLARE_AST_NODE(InitDeclarator)

  DeclaratorAST *declarator;
  InitializerAST *initializer;
};

class InitializerAST : public AST
{
public:

  DECLARE_AST_NODE(Initializer)

  InitializerClauseAST *initializer_clause;
  // expression list or braced init list
  ExpressionAST *expression;
};

class InitializerListAST : public ExpressionAST
{
public:
  DECLARE_AST_NODE(InitializerList)

  const ListNode<InitializerClauseAST*> *clauses;
  bool isVariadic : 1;
};

class BracedInitListAST : public ExpressionAST
{
public:
  DECLARE_AST_NODE(BracedInitList)

  InitializerListAST *list;
};


class InitializerClauseAST : public AST
{
public:

  DECLARE_AST_NODE(InitializerClause)

  // assignment expression or braced-init-list
  ExpressionAST *expression;
};

class LabeledStatementAST : public StatementAST
{
public:

  DECLARE_AST_NODE(LabeledStatement)

  uint label;
  //The constant label expression
  ExpressionAST *expression;
  StatementAST* statement;
};

/*
 * lambda-expression from the spec
 *
 * for optimization purposes we also include
 * lambda-introducer, lambda-capture, capture-default
 * and capture-list in here
 */
class LambdaExpressionAST : public ExpressionAST
{
public:

  DECLARE_AST_NODE(LambdaExpression)

  // '&' or '=' or neither
  uint default_capture;
  const ListNode<LambdaCaptureAST*> *capture_list;

  LambdaDeclaratorAST *declarator;
  StatementAST *compound;
};

/*
 * in the spec this is called just 'capture'.
 * We wrapped lambda-capture in LambdaExpressionAST
 */
class LambdaCaptureAST : public AST
{
public:

  DECLARE_AST_NODE(LambdaCapture)

  NameAST* identifier;

  bool isThis : 1;
  bool isRef : 1;
  bool isVariadic : 1;
};

class LambdaDeclaratorAST : public AST
{
public:

  DECLARE_AST_NODE(LambdaDeclarator)

  ParameterDeclarationClauseAST *parameter_declaration_clause;
  ///TODO: attribute-specifier
  bool isMutable : 1;
  ExceptionSpecificationAST *exception_spec;
  TrailingReturnTypeAST *trailing_return_type;
};

class LinkageBodyAST : public AST
{
public:
  LinkageBodyAST() :declarations(nullptr){}
  DECLARE_AST_NODE(LinkageBody)

  const ListNode<DeclarationAST*> *declarations;
};

class LinkageSpecificationAST : public DeclarationAST
{
public:

  DECLARE_AST_NODE(LinkageSpecification)

  uint extern_type;
  LinkageBodyAST *linkage_body;
  DeclarationAST *declaration;
};

class MemInitializerAST : public AST
{
public:

  DECLARE_AST_NODE(MemInitializer)

  NameAST *initializer_id;
  ExpressionAST *expression;
  // : foo(args)...
  bool initializerIsVariadic : 1;
  // : foo(args...)
  bool expressionIsVariadic : 1;;
};

class NameAST : public AST
{
public:

  DECLARE_AST_NODE(Name)

  bool global;
  const ListNode<UnqualifiedNameAST*> *qualified_names;
  UnqualifiedNameAST *unqualified_name;
};

class NamespaceAST : public DeclarationAST
{
public:

  DECLARE_AST_NODE(Namespace)

  LinkageBodyAST *linkage_body;
  uint namespace_name;
  bool inlined : 1;
};

class NamespaceAliasDefinitionAST : public DeclarationAST
{
public:

  DECLARE_AST_NODE(NamespaceAliasDefinition)

  uint namespace_name = 0;
  NameAST *alias_name = nullptr;
};

class NewDeclaratorAST : public AST
{
public:

  DECLARE_AST_NODE(NewDeclarator)

  PtrOperatorAST *ptr_op  = nullptr;
  NewDeclaratorAST *sub_declarator  = nullptr;
  const ListNode<ExpressionAST*> *expressions = nullptr;
};

class NewExpressionAST : public ExpressionAST
{
public:

  DECLARE_AST_NODE(NewExpression)

  uint scope_token = 0;
  uint new_token = 0;
  ExpressionAST *expression = nullptr;
  TypeIdAST *type_id = nullptr;
  NewTypeIdAST *new_type_id = nullptr;
  NewInitializerAST *new_initializer = nullptr;
};

class NewInitializerAST : public AST
{
public:

  DECLARE_AST_NODE(NewInitializer)

  ExpressionAST *expression = nullptr;
};

class NewTypeIdAST : public AST
{
public:

  DECLARE_AST_NODE(NewTypeId)

  TypeSpecifierAST *type_specifier = nullptr;
  NewInitializerAST *new_initializer = nullptr;
  NewDeclaratorAST *new_declarator = nullptr;
};

class OperatorAST : public AST
{
public:

  DECLARE_AST_NODE(Operator)

  uint op = 0; //Index of the token that describes the operator
  uint open = 0;
  uint close = 0;
};

class OperatorFunctionIdAST : public AST
{
public:

  DECLARE_AST_NODE(OperatorFunctionId)

  OperatorAST *op = nullptr;
  TypeSpecifierAST *type_specifier = nullptr;
  const ListNode<PtrOperatorAST*> *ptr_ops = nullptr;
};

class ParameterDeclarationAST : public AST
{
public:

  DECLARE_AST_NODE(ParameterDeclaration)

  TypeSpecifierAST *type_specifier = nullptr;
  DeclaratorAST *declarator = nullptr;
  ExpressionAST *expression = nullptr;
};

class ParameterDeclarationClauseAST : public AST
{
public:

  DECLARE_AST_NODE(ParameterDeclarationClause)

  const ListNode<ParameterDeclarationAST*> *parameter_declarations = nullptr;
  uint ellipsis = 0;
};

/**
 * A post-fix expression is an expression that consists of one primary expression and multiple sub-expressions that are evaluated from
 * left to right, each  sub-expression based on the previous expression.
 *
 *
 * Examples:
 * "a->b"  : "a" is the primary expression, "->b" is a sub-expression
 * "a->b(5,3)" : "a" is the primary expression, "->b" is a sub-expression, and "(5,3)" is a sub-expression
 **/
class PostfixExpressionAST : public ExpressionAST
{
public:

  DECLARE_AST_NODE(PostfixExpression)

  // Eventually the type-specifier of a constructed type, eg. "MyClass(expression)"
  // If this is nonzero, then expression is most probably zero.
  TypeSpecifierAST *type_specifier = nullptr;
  ExpressionAST *expression = nullptr;
  const ListNode<ExpressionAST*> *sub_expressions = nullptr;
};

class PrimaryExpressionAST : public ExpressionAST
{
public:

  DECLARE_AST_NODE(PrimaryExpression)

  union {
    StringLiteralAST *literal;
    StatementAST *expression_statement;
    ExpressionAST *sub_expression;
    NameAST *name;
    uint token;
  };
  enum Type {
    Literal,
    Token,
    Statement,
    SubExpression,
    Name
  };
  Type type = Type::Literal;
};

class PtrOperatorAST : public AST
{
public:

  DECLARE_AST_NODE(PtrOperator)

  const ListNode<uint> *cv = nullptr;
  uint op = 0; //Index of the token that describes the operator. Is zero when mem_ptr is non-zero.
  PtrToMemberAST *mem_ptr = nullptr;
};

class PtrToMemberAST : public AST
{
public:

  DECLARE_AST_NODE(PtrToMember)

  TypeSpecifierAST *class_type = nullptr;
};

class JumpStatementAST : public StatementAST
{
public:

  DECLARE_AST_NODE(JumpStatement)

  // index of operator token which describes the jump, one of
  // 'break', 'continue' or 'goto.  Return statements are handled by
  // ReturnStatementAST
  uint op = 0;
  // identifier for 'goto' statements
  uint identifier = 0;
};

class ReturnStatementAST : public StatementAST
{
public:

  DECLARE_AST_NODE(ReturnStatement)

  ExpressionAST *expression = nullptr;
};

class SimpleDeclarationAST : public DeclarationAST
{
public:

  DECLARE_AST_NODE(SimpleDeclaration)

  const ListNode<uint> *storage_specifiers = nullptr;
  const ListNode<uint> *function_specifiers = nullptr;
  TypeSpecifierAST *type_specifier = nullptr;
  const ListNode<InitDeclaratorAST*> *init_declarators = nullptr;
  WinDeclSpecAST *win_decl_specifiers = nullptr;
};

class SimpleTypeSpecifierAST : public TypeSpecifierAST
{
public:

  DECLARE_AST_NODE(SimpleTypeSpecifier)

  const ListNode<uint> *integrals = nullptr;
  TypeIdAST *type_id = nullptr;
  NameAST *name = nullptr;
  // expression for typeof or decltype
  ExpressionAST *expression = nullptr;
  bool isTypeof = false;
  bool isDecltype = false;
};

class SizeofExpressionAST : public ExpressionAST
{
public:

  DECLARE_AST_NODE(SizeofExpression)

  uint sizeof_token = 0;
  TypeIdAST *type_id = nullptr;
  ExpressionAST *expression = nullptr;
  bool isVariadic = false;
};

class StaticAssertAST : public DeclarationAST
{
public:

  DECLARE_AST_NODE(StaticAssert)

  ExpressionAST *expression = nullptr;
  StringLiteralAST *string = nullptr;
};

class StringLiteralAST : public AST
{
public:

  DECLARE_AST_NODE(StringLiteral)

  const ListNode<uint> *literals = nullptr;
};

/// operator []
class SubscriptExpressionAST : public ExpressionAST
{
public:

  DECLARE_AST_NODE(SubscriptExpression)

  ExpressionAST *subscript = nullptr;
};

class SwitchStatementAST : public StatementAST
{
public:

  DECLARE_AST_NODE(SwitchStatement)

  ConditionAST *condition = nullptr;
  StatementAST *statement = nullptr;
};

class TemplateArgumentAST : public AST
{
public:

  DECLARE_AST_NODE(TemplateArgument)

  TypeIdAST *type_id = nullptr;
  ExpressionAST *expression = nullptr;
  bool isVariadic = false;
};

class TemplateDeclarationAST : public DeclarationAST
{
public:

  DECLARE_AST_NODE(TemplateDeclaration)

  uint exported = 0;
  const ListNode<TemplateParameterAST*> *template_parameters = nullptr;
  DeclarationAST* declaration = nullptr;
};

class TemplateParameterAST : public AST
{
public:

  DECLARE_AST_NODE(TemplateParameter)

  TypeParameterAST *type_parameter = nullptr; //This is used if this is a template-parameter like "class A"
  ParameterDeclarationAST *parameter_declaration = nullptr; //This is used if this is a template-parameter like "int a"
};

class ThrowExpressionAST : public ExpressionAST
{
public:

  DECLARE_AST_NODE(ThrowExpression)

  uint throw_token = 0;
  ExpressionAST *expression = nullptr;
};

class TrailingReturnTypeAST : public AST
{
public:

  DECLARE_AST_NODE(TrailingReturnType)

  const ListNode<TypeSpecifierAST*> *type_specifier = nullptr;
  // TODO: attribute-specifier-seq
  DeclaratorAST* abstractDeclarator = nullptr;
};

class TranslationUnitAST : public AST, public CommentAST
{
public:

public:

  DECLARE_AST_NODE(TranslationUnit)

  const ListNode<DeclarationAST*> *declarations = nullptr;

  ///true if either a '}' was missing at the end, or there was a '}' too much.
  ///This indicates a temporary state where the user is typing, and the document is completely invalid.
  bool hadMissingCompoundTokens = false;

  // Note: non AST related, saves parsing session...
  ParseSession* session = nullptr;
};

class TryBlockStatementAST : public StatementAST
{
public:

  DECLARE_AST_NODE(TryBlockStatement)

  StatementAST* try_block = nullptr;
  const ListNode<CatchStatementAST*> *catch_blocks = nullptr;
};

class CatchStatementAST : public StatementAST
{
public:

  DECLARE_AST_NODE(CatchStatement)

  ConditionAST* condition = nullptr;
  StatementAST* statement = nullptr;
};

class TypeIdAST : public AST
{
public:

  DECLARE_AST_NODE(TypeId)

  TypeSpecifierAST *type_specifier = nullptr;
  DeclaratorAST *declarator = nullptr;
};

/// typeid(...)
class TypeIDOperatorAST : public PostfixExpressionAST
{
public:

  DECLARE_AST_NODE(TypeIDOperator)

  TypeIdAST* typeId = nullptr;
};

///"typename"
class TypeIdentificationAST : public ExpressionAST
{
public:

  DECLARE_AST_NODE(TypeIdentification)

  uint typename_token = 0;
  NameAST *name = nullptr;
  ExpressionAST *expression = nullptr;
};

class TypeParameterAST : public AST
{
public:

  DECLARE_AST_NODE(TypeParameter)

  NameAST *name = nullptr;
  TypeIdAST *type_id = nullptr;
  const ListNode<TemplateParameterAST*> *template_parameters = nullptr;
  NameAST *template_name = nullptr;
  uint type = 0;
  bool isVariadic = false;
};

class TypedefAST : public DeclarationAST
{
public:

  DECLARE_AST_NODE(Typedef)

  TypeSpecifierAST *type_specifier = nullptr;
  const ListNode<InitDeclaratorAST*> *init_declarators = nullptr;
};

class UnaryExpressionAST : public ExpressionAST
{
public:

  DECLARE_AST_NODE(UnaryExpression)

  uint op = 0; //Index of the token that describes the operator
  ExpressionAST *expression = nullptr;
};

class UnqualifiedNameAST : public AST
{
public:

  DECLARE_AST_NODE(UnqualifiedName)

  bool tilde = false;
  bool ellipsis = false;
  uint id = 0;
  OperatorFunctionIdAST *operator_id = nullptr;
  const ListNode<TemplateArgumentAST*> *template_arguments = nullptr;
};

class UsingAST : public DeclarationAST
{
public:

  DECLARE_AST_NODE(Using)

  uint type_name = 0;
  NameAST *name = nullptr;
};

class UsingDirectiveAST : public DeclarationAST
{
public:

  DECLARE_AST_NODE(UsingDirective)

  NameAST *name = nullptr;
};

class AliasDeclarationAST : public DeclarationAST
{
public:

  DECLARE_AST_NODE(AliasDeclaration)

  NameAST *name = nullptr;
  TypeIdAST *type_id = nullptr;
};

class WhileStatementAST : public StatementAST
{
public:

  DECLARE_AST_NODE(WhileStatement)

  ConditionAST *condition = nullptr;
  StatementAST *statement = nullptr;
};

class WinDeclSpecAST : public AST
{
public:

  DECLARE_AST_NODE(WinDeclSpec)

  uint specifier = 0;
  uint modifier = 0;
};


template <class _Tp>
_Tp ast_cast(AST *item)
{
  if (item && static_cast<_Tp>(0)->__node_kind == item->kind)
    return static_cast<_Tp>(item);

  return 0;
}

#endif // AST_H

