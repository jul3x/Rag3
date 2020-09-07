// C++ Abstract Syntax Implementation generated by the BNF Converter.

#include <algorithm>
#include <string>
#include <vector>
#include <R3E/j3x/Absyn.h>

/********************   Prog    ********************/
Prog::Prog(ListDef *p1)
{
  listdef_ = p1;

}

Prog::Prog(const Prog & other)
{
  listdef_ = other.listdef_->clone();

}

Prog &Prog::operator=(const Prog & other)
{
  Prog tmp(other);
  swap(tmp);
  return *this;
}

void Prog::swap(Prog & other)
{
  std::swap(listdef_, other.listdef_);

}

Prog::~Prog()
{
  delete(listdef_);

}

void Prog::accept(Visitor *v)
{
  v->visitProg(this);
}

Prog *Prog::clone() const
{
  return new Prog(*this);
}



/********************   VarDef    ********************/
VarDef::VarDef(Type *p1, Ident p2, Expr *p3)
{
  type_ = p1;
  ident_ = p2;
  expr_ = p3;

}

VarDef::VarDef(const VarDef & other)
{
  type_ = other.type_->clone();
  ident_ = other.ident_;
  expr_ = other.expr_->clone();

}

VarDef &VarDef::operator=(const VarDef & other)
{
  VarDef tmp(other);
  swap(tmp);
  return *this;
}

void VarDef::swap(VarDef & other)
{
  std::swap(type_, other.type_);
  std::swap(ident_, other.ident_);
  std::swap(expr_, other.expr_);

}

VarDef::~VarDef()
{
  delete(type_);
  delete(expr_);

}

void VarDef::accept(Visitor *v)
{
  v->visitVarDef(this);
}

VarDef *VarDef::clone() const
{
  return new VarDef(*this);
}



/********************   Int    ********************/
Int::Int()
{

}

Int::Int(const Int & other)
{

}

Int &Int::operator=(const Int & other)
{
  Int tmp(other);
  swap(tmp);
  return *this;
}

void Int::swap(Int & other)
{

}

Int::~Int()
{

}

void Int::accept(Visitor *v)
{
  v->visitInt(this);
}

Int *Int::clone() const
{
  return new Int(*this);
}



/********************   Str    ********************/
Str::Str()
{

}

Str::Str(const Str & other)
{

}

Str &Str::operator=(const Str & other)
{
  Str tmp(other);
  swap(tmp);
  return *this;
}

void Str::swap(Str & other)
{

}

Str::~Str()
{

}

void Str::accept(Visitor *v)
{
  v->visitStr(this);
}

Str *Str::clone() const
{
  return new Str(*this);
}



/********************   Bool    ********************/
Bool::Bool()
{

}

Bool::Bool(const Bool & other)
{

}

Bool &Bool::operator=(const Bool & other)
{
  Bool tmp(other);
  swap(tmp);
  return *this;
}

void Bool::swap(Bool & other)
{

}

Bool::~Bool()
{

}

void Bool::accept(Visitor *v)
{
  v->visitBool(this);
}

Bool *Bool::clone() const
{
  return new Bool(*this);
}



/********************   Float    ********************/
Float::Float()
{

}

Float::Float(const Float & other)
{

}

Float &Float::operator=(const Float & other)
{
  Float tmp(other);
  swap(tmp);
  return *this;
}

void Float::swap(Float & other)
{

}

Float::~Float()
{

}

void Float::accept(Visitor *v)
{
  v->visitFloat(this);
}

Float *Float::clone() const
{
  return new Float(*this);
}



/********************   Vector    ********************/
Vector::Vector()
{

}

Vector::Vector(const Vector & other)
{

}

Vector &Vector::operator=(const Vector & other)
{
  Vector tmp(other);
  swap(tmp);
  return *this;
}

void Vector::swap(Vector & other)
{

}

Vector::~Vector()
{

}

void Vector::accept(Visitor *v)
{
  v->visitVector(this);
}

Vector *Vector::clone() const
{
  return new Vector(*this);
}



/********************   ArrType    ********************/
ArrType::ArrType(Type *p1)
{
  type_ = p1;

}

ArrType::ArrType(const ArrType & other)
{
  type_ = other.type_->clone();

}

ArrType &ArrType::operator=(const ArrType & other)
{
  ArrType tmp(other);
  swap(tmp);
  return *this;
}

void ArrType::swap(ArrType & other)
{
  std::swap(type_, other.type_);

}

ArrType::~ArrType()
{
  delete(type_);

}

void ArrType::accept(Visitor *v)
{
  v->visitArrType(this);
}

ArrType *ArrType::clone() const
{
  return new ArrType(*this);
}



/********************   EAdd    ********************/
EAdd::EAdd(Expr *p1, Expr *p2)
{
  expr_1 = p1;
  expr_2 = p2;

}

EAdd::EAdd(const EAdd & other)
{
  expr_1 = other.expr_1->clone();
  expr_2 = other.expr_2->clone();

}

EAdd &EAdd::operator=(const EAdd & other)
{
  EAdd tmp(other);
  swap(tmp);
  return *this;
}

void EAdd::swap(EAdd & other)
{
  std::swap(expr_1, other.expr_1);
  std::swap(expr_2, other.expr_2);

}

EAdd::~EAdd()
{
  delete(expr_1);
  delete(expr_2);

}

void EAdd::accept(Visitor *v)
{
  v->visitEAdd(this);
}

EAdd *EAdd::clone() const
{
  return new EAdd(*this);
}



/********************   ESub    ********************/
ESub::ESub(Expr *p1, Expr *p2)
{
  expr_1 = p1;
  expr_2 = p2;

}

ESub::ESub(const ESub & other)
{
  expr_1 = other.expr_1->clone();
  expr_2 = other.expr_2->clone();

}

ESub &ESub::operator=(const ESub & other)
{
  ESub tmp(other);
  swap(tmp);
  return *this;
}

void ESub::swap(ESub & other)
{
  std::swap(expr_1, other.expr_1);
  std::swap(expr_2, other.expr_2);

}

ESub::~ESub()
{
  delete(expr_1);
  delete(expr_2);

}

void ESub::accept(Visitor *v)
{
  v->visitESub(this);
}

ESub *ESub::clone() const
{
  return new ESub(*this);
}



/********************   EMul    ********************/
EMul::EMul(Expr *p1, Expr *p2)
{
  expr_1 = p1;
  expr_2 = p2;

}

EMul::EMul(const EMul & other)
{
  expr_1 = other.expr_1->clone();
  expr_2 = other.expr_2->clone();

}

EMul &EMul::operator=(const EMul & other)
{
  EMul tmp(other);
  swap(tmp);
  return *this;
}

void EMul::swap(EMul & other)
{
  std::swap(expr_1, other.expr_1);
  std::swap(expr_2, other.expr_2);

}

EMul::~EMul()
{
  delete(expr_1);
  delete(expr_2);

}

void EMul::accept(Visitor *v)
{
  v->visitEMul(this);
}

EMul *EMul::clone() const
{
  return new EMul(*this);
}



/********************   EDiv    ********************/
EDiv::EDiv(Expr *p1, Expr *p2)
{
  expr_1 = p1;
  expr_2 = p2;

}

EDiv::EDiv(const EDiv & other)
{
  expr_1 = other.expr_1->clone();
  expr_2 = other.expr_2->clone();

}

EDiv &EDiv::operator=(const EDiv & other)
{
  EDiv tmp(other);
  swap(tmp);
  return *this;
}

void EDiv::swap(EDiv & other)
{
  std::swap(expr_1, other.expr_1);
  std::swap(expr_2, other.expr_2);

}

EDiv::~EDiv()
{
  delete(expr_1);
  delete(expr_2);

}

void EDiv::accept(Visitor *v)
{
  v->visitEDiv(this);
}

EDiv *EDiv::clone() const
{
  return new EDiv(*this);
}



/********************   ENeg    ********************/
ENeg::ENeg(Expr *p1)
{
  expr_ = p1;

}

ENeg::ENeg(const ENeg & other)
{
  expr_ = other.expr_->clone();

}

ENeg &ENeg::operator=(const ENeg & other)
{
  ENeg tmp(other);
  swap(tmp);
  return *this;
}

void ENeg::swap(ENeg & other)
{
  std::swap(expr_, other.expr_);

}

ENeg::~ENeg()
{
  delete(expr_);

}

void ENeg::accept(Visitor *v)
{
  v->visitENeg(this);
}

ENeg *ENeg::clone() const
{
  return new ENeg(*this);
}



/********************   EList    ********************/
EList::EList(ListExpr *p1)
{
  listexpr_ = p1;

}

EList::EList(const EList & other)
{
  listexpr_ = other.listexpr_->clone();

}

EList &EList::operator=(const EList & other)
{
  EList tmp(other);
  swap(tmp);
  return *this;
}

void EList::swap(EList & other)
{
  std::swap(listexpr_, other.listexpr_);

}

EList::~EList()
{
  delete(listexpr_);

}

void EList::accept(Visitor *v)
{
  v->visitEList(this);
}

EList *EList::clone() const
{
  return new EList(*this);
}



/********************   ELitInt    ********************/
ELitInt::ELitInt(Integer p1)
{
  integer_ = p1;

}

ELitInt::ELitInt(const ELitInt & other)
{
  integer_ = other.integer_;

}

ELitInt &ELitInt::operator=(const ELitInt & other)
{
  ELitInt tmp(other);
  swap(tmp);
  return *this;
}

void ELitInt::swap(ELitInt & other)
{
  std::swap(integer_, other.integer_);

}

ELitInt::~ELitInt()
{

}

void ELitInt::accept(Visitor *v)
{
  v->visitELitInt(this);
}

ELitInt *ELitInt::clone() const
{
  return new ELitInt(*this);
}



/********************   EString    ********************/
EString::EString(String p1)
{
  string_ = p1;

}

EString::EString(const EString & other)
{
  string_ = other.string_;

}

EString &EString::operator=(const EString & other)
{
  EString tmp(other);
  swap(tmp);
  return *this;
}

void EString::swap(EString & other)
{
  std::swap(string_, other.string_);

}

EString::~EString()
{

}

void EString::accept(Visitor *v)
{
  v->visitEString(this);
}

EString *EString::clone() const
{
  return new EString(*this);
}



/********************   EDouble    ********************/
EDouble::EDouble(Double p1)
{
  double_ = p1;

}

EDouble::EDouble(const EDouble & other)
{
  double_ = other.double_;

}

EDouble &EDouble::operator=(const EDouble & other)
{
  EDouble tmp(other);
  swap(tmp);
  return *this;
}

void EDouble::swap(EDouble & other)
{
  std::swap(double_, other.double_);

}

EDouble::~EDouble()
{

}

void EDouble::accept(Visitor *v)
{
  v->visitEDouble(this);
}

EDouble *EDouble::clone() const
{
  return new EDouble(*this);
}



/********************   ELitTrue    ********************/
ELitTrue::ELitTrue()
{

}

ELitTrue::ELitTrue(const ELitTrue & other)
{

}

ELitTrue &ELitTrue::operator=(const ELitTrue & other)
{
  ELitTrue tmp(other);
  swap(tmp);
  return *this;
}

void ELitTrue::swap(ELitTrue & other)
{

}

ELitTrue::~ELitTrue()
{

}

void ELitTrue::accept(Visitor *v)
{
  v->visitELitTrue(this);
}

ELitTrue *ELitTrue::clone() const
{
  return new ELitTrue(*this);
}



/********************   ELitFalse    ********************/
ELitFalse::ELitFalse()
{

}

ELitFalse::ELitFalse(const ELitFalse & other)
{

}

ELitFalse &ELitFalse::operator=(const ELitFalse & other)
{
  ELitFalse tmp(other);
  swap(tmp);
  return *this;
}

void ELitFalse::swap(ELitFalse & other)
{

}

ELitFalse::~ELitFalse()
{

}

void ELitFalse::accept(Visitor *v)
{
  v->visitELitFalse(this);
}

ELitFalse *ELitFalse::clone() const
{
  return new ELitFalse(*this);
}



/********************   EVar    ********************/
EVar::EVar(Ident p1)
{
  ident_ = p1;

}

EVar::EVar(const EVar & other)
{
  ident_ = other.ident_;

}

EVar &EVar::operator=(const EVar & other)
{
  EVar tmp(other);
  swap(tmp);
  return *this;
}

void EVar::swap(EVar & other)
{
  std::swap(ident_, other.ident_);

}

EVar::~EVar()
{

}

void EVar::accept(Visitor *v)
{
  v->visitEVar(this);
}

EVar *EVar::clone() const
{
  return new EVar(*this);
}



/********************   EVector    ********************/
EVector::EVector(Expr *p1, Expr *p2)
{
  expr_1 = p1;
  expr_2 = p2;

}

EVector::EVector(const EVector & other)
{
  expr_1 = other.expr_1->clone();
  expr_2 = other.expr_2->clone();

}

EVector &EVector::operator=(const EVector & other)
{
  EVector tmp(other);
  swap(tmp);
  return *this;
}

void EVector::swap(EVector & other)
{
  std::swap(expr_1, other.expr_1);
  std::swap(expr_2, other.expr_2);

}

EVector::~EVector()
{
  delete(expr_1);
  delete(expr_2);

}

void EVector::accept(Visitor *v)
{
  v->visitEVector(this);
}

EVector *EVector::clone() const
{
  return new EVector(*this);
}




/********************   ListDef    ********************/

void ListDef::accept(Visitor *v)
{
  v->visitListDef(this);
}


ListDef *ListDef::clone() const
{
  return new ListDef(*this);
}


/********************   ListExpr    ********************/

void ListExpr::accept(Visitor *v)
{
  v->visitListExpr(this);
}


ListExpr *ListExpr::clone() const
{
  return new ListExpr(*this);
}




