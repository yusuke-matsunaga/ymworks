#! /usr/bin/env python3

""" PyClibTiming 用のファイルを生成するスクリプト

:file: mk_ClibTiming.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

from mk_classobj import mk_classobj
import os.path

class_name = "ClibTiming"
impl_name = "CiTiming"

mk_classobj(class_name, impl_name, "YM_CLIB", "ym/ClibTiming.h", "ymcell")


