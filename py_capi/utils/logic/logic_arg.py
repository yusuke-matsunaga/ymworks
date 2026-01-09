#! /usr/bin/env python3

""" BddArg の定義ファイル

:file: bdd_arg.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import ObjConvArg, TypedObjConvArg


class Val3Arg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='Val3',
                         cvardefault=None,
                         pyclassname='PyVal3')


class PrimTypeArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='PrimType',
                         cvardefault=None,
                         pyclassname='PyPrimType')


class LiteralArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='Literal',
                         cvardefault=None,
                         pyclassname='PyLiteral')


class LiteralListArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='std::vector<Literal>',
                         cvardefault=None,
                         pyclassname='PyList<Literal, PyLiteral>')


class NpnMapArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='NpnMap',
                         cvardefault=None,
                         pyclassname='PyNpnMap')


class ExprArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='Expr',
                         cvardefault=None,
                         pyclassname='PyExpr')


class ExprListArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='std::vector<Expr>',
                         cvardefault=None,
                         pyclassname='PyList<Expr, PyExpr>')


class TvFuncArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='TvFunc',
                         cvardefault=None,
                         pyclassname='PyTvFunc')


class BddMgrArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='BddMgr',
                         cvardefault=None,
                         pyclassname='PyBddMgr')


class BddArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='Bdd',
                         cvardefault=None,
                         pyclassname='PyBdd')


class BddListArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='std::vector<Bdd>',
                         cvardefault=None,
                         pyclassname='PyList<Bdd, PyBdd>')


class BddVarArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='BddVar',
                         cvardefault=None,
                         pyclassname='PyBddVar')


class BddVarListArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='std::vector<BddVar>',
                         cvardefault=None,
                         pyclassname='PyList<BddVar, PyBddVar>')


class BddVarSetArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='BddVarSet',
                         cvardefault=None,
                         pyclassname='PyBddVarSet')


class BddLitArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='BddLit',
                         cvardefault=None,
                         pyclassname='PyBddLit')


class BddLitListArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='std::vector<BddLit>',
                         cvardefault=None,
                         pyclassname='PyList<BddLit, PyBddLit>')


class BddCubeArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='BddCube',
                         cvardefault=None,
                         pyclassname='PyBddCube')


class BddCompMapArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='Bdd::ComposeMap',
                         cvardefault=None,
                         pyclassname='PyBddCompMap')


class BddVarMapArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='Bdd::VarMap',
                         cvardefault=None,
                         pyclassname='PyBddVarMap')


class SopCubeArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='SopCube',
                         cvardefault=None,
                         pyclassname='PySopCube')


class SopCubeListArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='std::vector<SopCube>',
                         cvardefault=None,
                         pyclassname='PyList<SopCube, PySopCube>')


class SopCoverArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='SopCover',
                         cvardefault=None,
                         pyclassname='PySopCover')


class SopCoverListArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='std::vector<SopCover>',
                         cvardefault=None,
                         pyclassname='PyList<SopCover, PySopCover>')


class TruthTableArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='TruthTable',
                         cvardefault=None,
                         pyclassname='PyTruthTable')


class BcOpTypeArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='BcOpType',
                         cvardefault=None,
                         pyclassname='PyBcOpType')


class BcEdgeArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='BcEdge',
                         cvardefault=None,
                         pyclassname='PyBcEdge')


class BcEdgeListArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='std::vector<BcEdge>',
                         cvardefault=None,
                         pyclassname='PyList<BcEdge, PyBcEdge>')

class BcBvListArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='std::vector<BcBvType>',
                         cvardefault=None,
                         pyclassname='PyList<BcBvType, PyUint64>')
