#! /usr/bin/env python3

""" Arg の定義ファイル

:file: arg.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""


def make_vardef(vartype, varname, vardefault):
    """変数宣言文を作る．

    :param str vartype: 変数の型
    :param str varname: 変数名
    :param str vardefault: 初期値
    :return: 文を返す．

    末尾のセミコロンは含まない．
    """
    vardef = f'{vartype} {varname}'
    if vardefault is not None:
        vardef += f' = {vardefault}'
    return vardef

def make_varref(varname):
    """変数のアドレス参照を作る．

    :param str varname: 変数名
    """
    return f'&{varname}'


class ArgBase:
    """引数の基底クラス

    :param str name: 変数名
    :param str pchar: PyArg_ParseTuple() 用のフォーマット文字
    :param str vardef: 変数宣言文
    :param str varref: 変数のアドレス参照式

    読み込んだ値を変換する必要がある場合には gen_conv(self, writer)
    にその変換コードを生成する処理を記述する．
    このクラスのデフォルトはなにもしない．
    """

    def __init__(self, *,
                 name=None,
                 pchar,
                 vardef,
                 varref):
        self.name = name
        self.pchar = pchar
        self.vardef = vardef
        self.varref = varref

    def gen_conv(self, writer):
        pass


class OptArg(ArgBase):
    """以降がオプション引数であることを示すマーカー
    """

    def __init__(self):
        super().__init__(pchar='|',
                         vardef=None,
                         varref=None)


class KwdArg(ArgBase):
    """以降がキーワード引数であることを示すマーカー
    """

    def __init__(self):
        super().__init__(pchar='$',
                         vardef=None,
                         varref=None)


class RawArg(ArgBase):
    """PyArg_ParseTuple() で直接変換するタイプの引数を表す基底クラス

    :param str name: 変数名
    :param str pchar: PyArg_ParseTuple() 用のフォーマット文字
    :param str cvartype: C++の変数の型
    :param str cvarname: C++の変数名
    :param str cvardefault: C++の変数の初期値(省略時は None)
    """

    def __init__(self, *,
                 name=None,
                 pchar,
                 cvartype,
                 cvarname,
                 cvardefault=None):
        super().__init__(name=name,
                         pchar=pchar,
                         vardef=make_vardef(cvartype, cvarname, cvardefault),
                         varref=make_varref(cvarname))


class CharArg(RawArg):
    """char 型の引数を表すクラス

    :param str name: 変数名
    :param str cvarname: C++の変数名
    :param str cvardefault: C++の変数の初期値(省略時は 0)
    """

    def __init__(self, *,
                 name=None,
                 cvarname,
                 cvardefault=None):
        super().__init__(name=name,
                         pchar='C',
                         cvartype='int',
                         cvarname=cvarname,
                         cvardefault=cvardefault)


class IntArg(RawArg):
    """int 型の引数を表すクラス

    :param str name: 変数名
    :param str cvarname: C++の変数名
    :param str cvardefault: C++の変数の初期値(省略時は None)
    """

    def __init__(self, *,
                 name=None,
                 cvarname,
                 cvardefault=None):
        super().__init__(name=name,
                         pchar='i',
                         cvartype='int',
                         cvarname=cvarname,
                         cvardefault=cvardefault)


class Int32Arg(RawArg):
    """std::int32_t 型の引数を表すクラス

    :param str name: 変数名
    :param str cvarname: C++の変数名
    :param str cvardefault: C++の変数の初期値(省略時は None)
    """

    def __init__(self, *,
                 name=None,
                 cvarname,
                 cvardefault=None):
        super().__init__(name=name,
                         pchar='l',
                         cvartype='std::int32_t',
                         cvarname=cvarname,
                         cvardefault=cvardefault)


class Int64Arg(RawArg):
    """std::int64_t 型の引数を表すクラス

    :param str name: 変数名
    :param str cvarname: C++の変数名
    :param str cvardefault: C++の変数の初期値(省略時は None)
    """

    def __init__(self, *,
                 name=None,
                 cvarname,
                 cvardefault=None):
        super().__init__(name=name,
                         pchar='L',
                         cvartype='std::int64_t',
                         cvarname=cvarname,
                         cvardefault=cvardefault)


class UintArg(RawArg):
    """unsigned int 型の引数を表すクラス

    :param str name: 変数名
    :param str cvarname: C++の変数名
    :param str cvardefault: C++の変数の初期値(省略時は None)
    """

    def __init__(self, *,
                 name=None,
                 cvarname,
                 cvardefault=None):
        super().__init__(name=name,
                         pchar='I',
                         cvartype='unsigned int',
                         cvarname=cvarname,
                         cvardefault=cvardefault)


class Uint32Arg(RawArg):
    """std::uint32_t 型の引数を表すクラス

    :param str name: 変数名
    :param str cvarname: C++の変数名
    :param str cvardefault: C++の変数の初期値(省略時は None)
    """

    def __init__(self, *,
                 name=None,
                 cvarname,
                 cvardefault=None):
        super().__init__(name=name,
                         pchar='k',
                         cvartype='std::uint32_t',
                         cvarname=cvarname,
                         cvardefault=cvardefault)


class Uint64Arg(RawArg):
    """std::uint64_t 型の引数を表すクラス

    :param str name: 変数名
    :param str cvarname: C++の変数名
    :param str cvardefault: C++の変数の初期値(省略時は None)
    """

    def __init__(self, *,
                 name=None,
                 cvarname,
                 cvardefault=None):
        super().__init__(name=name,
                         pchar='K',
                         cvartype='std::uint64_t',
                         cvarname=cvarname,
                         cvardefault=cvardefault)


class LongArg(RawArg):
    """long 型の引数を表すクラス

    :param str name: 変数名
    :param str cvarname: C++の変数名
    :param str cvardefault: C++の変数の初期値(省略時は None)
    """

    def __init__(self, *,
                 name=None,
                 cvarname,
                 cvardefault=None):
        super().__init__(name=name,
                         pchar='l',
                         cvartype='long',
                         cvarname=cvarname,
                         cvardefault=cvardefault)


class UlongArg(RawArg):
    """unsigned long 型の引数を表すクラス

    :param str name: 変数名
    :param str cvarname: C++の変数名
    :param str cvardefault: C++の変数の初期値(省略時は None)
    """

    def __init__(self, *,
                 name=None,
                 cvarname,
                 cvardefault=None):
        super().__init__(name=name,
                         pchar='k',
                         cvartype='unsigned long',
                         cvarname=cvarname,
                         cvardefault=cvardefault)


class DoubleArg(RawArg):
    """double 型の引数を表すクラス

    :param str name: 変数名
    :param str cvarname: C++の変数名
    :param str cvardefault: C++の変数の初期値(省略時は None)
    """

    def __init__(self, *,
                 name=None,
                 cvarname,
                 cvardefault=None):
        super().__init__(name=name,
                         pchar='d',
                         cvartype='double',
                         cvarname=cvarname,
                         cvardefault=cvardefault)


class RawObjArg(RawArg):
    """PyObject* 型の引数を表すクラス

    :param str name: 変数名
    :param str cvarname: C++の変数名
    :param str cvardefault: C++の変数の初期値(省略時は nullptr)
    """

    def __init__(self, *,
                 name=None,
                 cvarname,
                 cvardefault='nullptr'):
        super().__init__(name=name,
                         pchar='O',
                         cvartype='PyObject*',
                         cvarname=cvarname,
                         cvardefault=cvardefault)


class TypedRawObjArg(ArgBase):
    """型チェック付きの PyObject* 型の引数を表すクラス

    :param str name: 変数名
    :param str cvarname: C++の変数名
    :param str cvardefault: C++の変数の初期値(省略時は nullptr)
    :param str pytypename: PyObjectType の型名
    """

    def __init__(self, *,
                 name=None,
                 cvarname,
                 cvardefault='nullptr',
                 pytypename):
        super().__init__(name=name,
                         pchar='O!',
                         vardef=f'PyObject* {cvarname} = nullptr',
                         varref=f'{pytypename}, &{cvarname}')


class TypedObjRefArg(ArgBase):
    """型チェック付きの PyObject* 型の引数を表すクラス(参照を返す)

    :param str name: 変数名
    :param str cvarname: C++の変数名
    :param str pyclassname: Python のクラス名
    """

    def __init__(self, *,
                 name=None,
                 cvarname,
                 pyclassname):
        tmpname = f'{cvarname}_obj'
        super().__init__(name=name,
                         pchar='O!',
                         vardef=f'PyObject* {tmpname} = nullptr',
                         varref=f'{pyclassname}::_typeobject(), &{tmpname}')
        self.cvarname = cvarname
        self.tmpname = tmpname
        self.pyclassname = pyclassname

    def gen_conv(self, writer):
        writer.gen_autoref_assign(self.cvarname,
                                  f'{self.pyclassname}::_get_ref({self.tmpname})')


class ConvFunc:
    """変換コードを生成するファンクタ

    :param str cvartype: C++の変数の型
    :param str cvarname: C++の変数名
    :param str cvardefault: C++の変数の初期値(省略時は None)
    """

    def __init__(self,
                 cvartype,
                 cvarname,
                 cvardefault):
        self.cvartype = cvartype
        self.cvarname = cvarname
        self.cvardefault = cvardefault

    def __call__(self, writer):
        stmt = make_vardef(self.cvartype, self.cvarname, self.cvardefault)
        writer.write_stmt(stmt)
        self.conv_body(writer)


class ConvArg(ArgBase):
    """一旦読み込んだ値を変換するタイプの引数を表すクラス

    :param str name: 変数名
    :param str pchar: PyArg_ParseTuple() 用のフォーマット文字
    :param str cvartype: C++の変数の型
    :param str cvarname: C++の変数名
    :param str cvardefault: C++の変数の初期値(省略時は None)
    :param str tmptype: 読み込んだ値を一時的に保存する変数の型
    :param str tmpname: 読み込んだ値を一時的に保存する変数の名前
    :param str tmpdefault: 読み込んだ値を一時的に保存する変数の初期値

    変換コードを生成する処理を conv_body(self, writer) で実装する必要がある．
    """

    def __init__(self, *,
                 name=None,
                 pchar,
                 cvartype,
                 cvarname,
                 cvardefault=None,
                 tmptype,
                 tmpname,
                 tmpdefault=None):
        super().__init__(name=name,
                         pchar=pchar,
                         vardef=make_vardef(tmptype, tmpname, tmpdefault),
                         varref=make_varref(tmpname))
        self.tmpname = tmpname
        self.cvartype = cvartype
        self.cvarname = cvarname
        self.cvardefault = cvardefault

    def gen_conv(self, writer):
        line = make_vardef(self.cvartype, self.cvarname, self.cvardefault) + ';'
        writer.write_line(line)
        self.conv_body(writer)


class BoolArg(ConvArg):
    """bool 型の引数を表すクラス

    :param str name: 変数名
    :param str cvarname: C++の変数名
    :param str cvardefault: C++の変数の初期値(省略時は None)
    """

    def __init__(self, *,
                 name=None,
                 cvarname,
                 cvardefault=None):
        tmpname = f'{cvarname}_tmp'
        super().__init__(name=name,
                         pchar='p',
                         cvartype='bool',
                         cvarname=cvarname,
                         cvardefault=cvardefault,
                         tmptype='int',
                         tmpname=tmpname,
                         tmpdefault='-1')

    def conv_body(self, writer):
        with writer.gen_if_block(f'{self.tmpname} != -1'):
            writer.gen_assign(self.cvarname, self.tmpname, casttype='bool')


class StringArg(ConvArg):
    """std::string 型の引数を表すクラス

    :param str name: 変数名
    :param str cvarname: C++の変数名
    :param str cvardefault: C++の変数の初期値(省略時は None)
    """

    def __init__(self, *,
                 name=None,
                 cvarname,
                 cvardefault=None):
        tmpname = f'{cvarname}_tmp'
        super().__init__(name=name,
                         pchar='s',
                         cvartype='std::string',
                         cvarname=cvarname,
                         cvardefault=cvardefault,
                         tmptype='const char*',
                         tmpname=tmpname,
                         tmpdefault='nullptr')

    def conv_body(self, writer):
        with writer.gen_if_block(f'{self.tmpname} != nullptr'):
            writer.gen_assign(f'{self.cvarname}',
                              f'std::string({self.tmpname})')


class ObjConvArgBase(ArgBase):
    """変換コード付きの PyObject* 型の引数を表す基底クラス

    :param str name: 変数名
    :param str cvartype: C++の変数の型
    :param str cvarname: C++の変数の名前
    :param str cvardefault: C++の変数の初期値(省略時は None)

    継承クラスで conv_body(self, writer) を実装する必要がある．
    """

    def __init__(self, *,
                 name=None,
                 cvartype,
                 cvarname,
                 cvardefault):
        tmptype = 'PyObject*'
        tmpname = f'{cvarname}_obj'
        super().__init__(name=name,
                         pchar='O',
                         vardef=make_vardef(tmptype, tmpname, 'nullptr'),
                         varref=make_varref(tmpname))
        self.cvartype = cvartype
        self.cvarname = cvarname
        self.cvardefault = cvardefault
        self.tmpname = tmpname

    def gen_conv(self, writer):
        line = make_vardef(self.cvartype, self.cvarname, self.cvardefault) + ';'
        writer.write_line(line)
        with writer.gen_if_block(f'{self.tmpname} != nullptr'):
            self.conv_body(writer)

    def conv_body(self, writer):
        raise ValueError('You must overwrite this code')


class ObjConvArg(ObjConvArgBase):
    """PyObject* 型の引数を表すクラス

    PyXXX::FromPyObject() を用いる．
    """

    def __init__(self, *,
                 name=None,
                 cvartype,
                 cvarname,
                 cvardefault,
                 pyclassname):
        super().__init__(name=name,
                         cvartype=cvartype,
                         cvarname=cvarname,
                         cvardefault=cvardefault)
        self.pyclassname = pyclassname

    def conv_body(self, writer):
        with writer.gen_if_block(f'!{self.pyclassname}::FromPyObject({self.tmpname}, {self.cvarname})'):
            writer.gen_value_error(f'"could not convert to {self.cvartype}"')


class TypedObjConvArg(ArgBase):
    """型指定付きの PyObject* 型の引数を表すクラス

    PyXXX::FromPyObject() を用いる．
    """

    def __init__(self, *,
                 name=None,
                 cvartype,
                 cvarname,
                 cvardefault,
                 pyclassname):
        tmptype = 'PyObject*'
        tmpname = f'{cvarname}_obj'
        super().__init__(name=name,
                         pchar='O!',
                         vardef=make_vardef(tmptype, tmpname, 'nullptr'),
                         varref=f'{pyclassname}::_typeobject(), &{tmpname}')
        self.cvartype = cvartype
        self.cvarname = cvarname
        self.cvardefault = cvardefault
        self.pyclassname = pyclassname
        self.tmpname = tmpname

    def gen_conv(self, writer):
        line = make_vardef(self.cvartype, self.cvarname, self.cvardefault) + ';'
        writer.write_line(line)
        with writer.gen_if_block(f'{self.tmpname} != nullptr'):
            with writer.gen_if_block(f'!{self.pyclassname}::FromPyObject({self.tmpname}, {self.cvarname})'):
                writer.gen_type_error(f'"could not convert to {self.cvartype}"')
