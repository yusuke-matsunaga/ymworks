#! /usr/bin/env python3

"""ymcell パッケージのテストプログラム

:file: py_ymcell_test.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2022 Yusuke Matsunaga, All rights reserved.
"""

import pytest
import io
import os
import sys
import tempfile
from pathlib import Path
from ymworks.clib import ClibCellLibrary


def test_read_mislib():
    DATAPATH = os.environ.get("TESTDATA_DIR")
    lib2_filename = os.path.join(DATAPATH, 'clib', 'lib2.genlib')
    exp_filename = os.path.join(DATAPATH, 'clib', 'lib2.genlib.display')
    lib = ClibCellLibrary.read_mislib(lib2_filename)

    f = io.StringIO()
    lib.display(f)
    lines = f.getvalue()

    with open(exp_filename, "rt") as exp_f:
        exp_lines = exp_f.read()
        assert exp_lines == lines

def test_read_liberty():
    DATAPATH = os.environ.get("TESTDATA_DIR")
    lib_filename = os.path.join(DATAPATH, 'clib', 'HIT018.typ.snp')
    exp_filename = os.path.join(DATAPATH, 'clib', 'HIT018.typ.snp.display')
    lib = ClibCellLibrary.read_liberty(lib_filename)

    assert lib.is_valid()

    f = io.StringIO()
    lib.display(f)
    lines = f.getvalue()

    with open(exp_filename, "rt") as exp_f:
        exp_lines = exp_f.read()
        assert exp_lines == lines

def test_dump_restore():
    DATAPATH = os.environ.get("TESTDATA_DIR")
    lib_filename = os.path.join(DATAPATH, 'clib', 'HIT018.typ.snp')
    exp_filename = os.path.join(DATAPATH, 'clib', 'HIT018.typ.snp.display')
    lib = ClibCellLibrary.read_liberty(lib_filename)

    assert lib.is_valid()

    filename = None
    with tempfile.NamedTemporaryFile(mode='wb', delete=False) as temp:
        filename = temp.name
        lib.dump(filename)

    assert filename is not None
    lib2 = ClibCellLibrary.restore(filename)

    f1 = io.StringIO()
    lib.display(f1)
    lines1 = f1.getvalue()

    f2 = io.StringIO()
    lib2.display(f2)
    lines2 = f2.getvalue()

    assert lines1 == lines2
