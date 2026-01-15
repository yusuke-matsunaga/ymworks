#! /usr/bin/env python3

""" BnModelArg の定義ファイル

:file: bn_arg.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import TypedObjRefArg, TypedObjConvArg, ObjConvArg


class BnModelArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='BnModel',
                         cvardefault=None,
                         pyclassname='PyBnModel')


class BnModelRefArg(TypedObjRefArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         pyclassname='PyBnModel')


class BnNodeArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='BnNode',
                         cvardefault=None,
                         pyclassname='PyBnNode')


class BnNodeListArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='std::vector<BnNode>',
                         cvardefault=None,
                         pyclassname='PyList<BnNode, PyBnNode>')



class BnDffArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='BnDff',
                         cvardefault=None,
                         pyclassname='PyBnDff')


class BnDffListArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='std::vector<BnDff>',
                         cvardefault=None,
                         pyclassname='PyList<BnDff, PyBnDff>')


class BnFuncArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='BnFunc',
                         cvardefault=None,
                         pyclassname='PyBnFuncl')
