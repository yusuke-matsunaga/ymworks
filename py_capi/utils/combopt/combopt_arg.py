#! /usr/bin/env python3

""" combopt サブモジュールのオブジェクトの引数を定義するスクリプト

:file: sat_arg.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import ObjConvArg, TypedObjConvArg


class UdGraphArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='UdGraph',
                         cvardefault=None,
                         pyclassname='PyUdGraph')


class UdEdgeArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='UdEdge',
                         cvardefault=None,
                         pyclassname='PyUdEdge')


class UdEdgeListArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='std::vector<UdEdge>',
                         cvardefault=None,
                         pyclassname='PyUdEdgeList')


class MinCovArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='MinCov',
                         cvardefault=None,
                         pyclassname='PyMinCov')
