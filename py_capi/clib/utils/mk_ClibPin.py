#! /usr/bin/env python3

""" PyClibPin 用のファイルを生成するスクリプト

:file: mk_ClibPin.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

from mk_classobj import mk_classobj
import os.path

class_name = "ClibPin"
impl_name = "CiPin"

mk_classobj(class_name, impl_name, "YM_CLIB", "ym/ClibPin.h", "ymcell")


