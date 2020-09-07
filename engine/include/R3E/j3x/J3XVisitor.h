//
// Created by jul3x on 05.09.20.
//

#ifndef RAG3_ENGINE_INCLUDE_R3E_J3X_J3XVISITOR_H
#define RAG3_ENGINE_INCLUDE_R3E_J3X_J3XVISITOR_H

#include <iostream>
#include <unordered_map>
#include <any>
#include <utility>

#include <R3E/j3x/Absyn.h>


namespace r3e::j3x {

    class J3XVisitor : public Visitor {

    public:
        explicit J3XVisitor(std::string ns = "") : ns_(std::move(ns)) {}

        void visitScript(Script *p) override;
        void visitDef(Def *p) override;
        void visitType(Type *p) override;
        void visitExpr(Expr *p) override;
        void visitProg(Prog *p) override;
        void visitVarDef(VarDef *p) override;
        void visitInt(Int *p) override;
        void visitStr(Str *p) override;
        void visitBool(Bool *p) override;
        void visitFloat(Float *p) override;
        void visitVector(Vector *p) override;
        void visitArrType(ArrType *p) override;
        void visitEAdd(EAdd *p) override;
        void visitESub(ESub *p) override;
        void visitEMul(EMul *p) override;
        void visitEDiv(EDiv *p) override;
        void visitENeg(ENeg *p) override;
        void visitEList(EList *p) override;
        void visitELitInt(ELitInt *p) override;
        void visitEString(EString *p) override;
        void visitEDouble(EDouble *p) override;
        void visitELitTrue(ELitTrue *p) override;
        void visitELitFalse(ELitFalse *p) override;
        void visitEVar(EVar *p) override;
        void visitEVector(EVector *p) override;
        void visitListDef(ListDef *p) override;
        void visitListExpr(ListExpr *p) override;
        void visitInteger(Integer x) override;
        void visitChar(Char x) override;
        void visitDouble(Double x) override;
        void visitString(String x) override;
        void visitIdent(Ident x) override;

        const std::unordered_map<std::string, std::any>& getParams() {
            return variables_;
        }

    private:
        void addNewVariable(const Ident& x, const std::any &value, const std::string &type) {
            std::string new_name = ns_.empty() ? x : ns_ + "/" + x;
            if (checkExistance(new_name))
                throw std::invalid_argument("Redeclaration of \"" + new_name + "\" variable!");

            variables_[new_name] = value;
            types_[new_name] = type;
        }

        const std::string &getVariableType(const Ident& x) {
            std::string new_name = ns_.empty() ? x : ns_ + "/" + x;
            if (!checkExistance(new_name))
                throw std::invalid_argument("Variable \"" + new_name + "\" does not exist!");

            return types_.find(new_name)->second;
        }

        const std::any &getVariableValue(const Ident& x) {
            std::string new_name = ns_.empty() ? x : ns_ + "/" + x;
            if (!checkExistance(new_name))
                throw std::invalid_argument("Variable \"" + new_name + "\" does not exist!");

            return variables_.find(new_name)->second;
        }

        bool checkExistance(const Ident& x) {
            return (variables_.find(x) != variables_.end() and types_.find(x) != types_.end());
        }

        std::unordered_map<std::string, std::any> variables_;
        std::unordered_map<std::string, std::string> types_;
        std::string ns_;

    };

}  // namespace r3e::j3x

#endif // RAG3_ENGINE_INCLUDE_R3E_J3X_J3XVISITOR_H
