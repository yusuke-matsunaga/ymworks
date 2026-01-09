#! /usr/bin/env python3

"""

:file: sat_arg.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import ObjConvArg, TypedObjConvArg, TypedObjRefArg


class CnfSizeArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='CnfSize',
                         cvardefault=None,
                         pyclassname='PyCnfSize')


class SatBool3Arg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='SatBool3',
                         cvardefault=None,
                         pyclassname='PySatBool3')


class SatInitParamArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='SatInitParam',
                         cvardefault=None,
                         pyclassname='PySatInitParam')


class SatLiteralArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='SatLiteral',
                         cvardefault=None,
                         pyclassname='PySatLiteral')


class SatLiteralListArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='std::vector<SatLiteral>',
                         cvardefault=None,
                         pyclassname='PySatLiteralList')


class SatLiteralMapArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='SatLiteralMap',
                         cvardefault=None,
                         pyclassname='PySatLiteralMap')


class SatModelArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='SatModel',
                         cvardefault=None,
                         pyclassname='PySatModel')


class SatSolverRefArg(TypedObjRefArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         pyclassname='PySatSolver')


class SatStatsArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='SatStats',
                         cvardefault=None,
                         pyclassname='PySatStats')
