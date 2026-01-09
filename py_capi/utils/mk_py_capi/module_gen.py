#! /usr/bin/env python3

""" ModuleGen のクラス定義ファイル

:file: module_gen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

import re
import os
import sys
from .genbase import GenBase
from .genbase import IncludesGen, BeginNamespaceGen, EndNamespaceGen, GenBase, PatGen
from .method_gen import MethodGen
from .cxxwriter import CxxWriter
from .cmwriter import CmWriter


class YmAddObjectLibraryTarget:
    """ym_add_object_library() を出力するクラス
    """
    def __init__(self, *,
                 objname,
                 src_list):
        self.__objname = objname
        self.__src_list = src_list

    def __call__(self, writer):
        writer.write_line(f'ym_add_object_library( {self.__objname}')
        writer.indent_inc()
        for src in self.__src_list:
            writer.write_line(src)
        writer.write_line(')')
        writer.indent_dec()
        writer.write_line('')


class AddExecutableTarget:
    """add_executable() を出力するクラス
    """
    def __init__(self, *,
                 exename,
                 src_list,
                 option_list=[],
                 definition_list=[],
                 link_list=[]):
        self.__exename = exename
        self.__src_list = src_list
        self.__option_list = option_list
        self.__definition_list = definition_list
        self.__link_list = link_list

    def __call__(self, writer):
        writer.write_line(f'add_executable( {self.__exename}')
        writer.indent_inc()
        for src in self.__src_list:
            writer.write_line(src)
        writer.write_line(')')
        writer.indent_dec()
        if len(self.__option_list) > 0:
            writer.write_line('')
            writer.write_line(f'target_compile_options( {self.__exename}')
            writer.indent_inc()
            writer.write_line('PRIVATE')
            for option in self.__option_list:
                writer.write_line(option)
            writer.write_line(')')
            writer.indent_dec()
        if len(self.__definition_list) > 0:
            writer.write_line('')
            writer.write_line(f'target_compile_definitions( {self.__exename}')
            writer.indent_inc()
            writer.write_line('PRIVATE')
            for definition in self.__definition_list:
                writer.write_line(definition)
            writer.write_line(')')
            writer.indent_dec()
        if len(self.__link_list) > 0:
            writer.write_line('')
            writer.write_line(f'target_link_libraries( {self.__exename}')
            writer.indent_inc()
            for link in self.__link_list:
                writer.write_line(link)
            writer.write_line(')')
            writer.indent_dec()


class AddLibraryTarget:
    """add_library() を出力するクラス
    """
    def __init__(self, *,
                 libname,
                 shared=True,
                 src_list,
                 link_list=[],
                 property_list=[]):
        self.__libname = libname
        self.__shared = shared
        self.__src_list = src_list
        self.__link_list = link_list
        self.__property_list = property_list

    def __call__(self, writer):
        writer.write_line(f'add_library( {self.__libname}')
        writer.indent_inc()
        if self.__shared:
            writer.write_line('SHARED')
        for src in self.__src_list:
            writer.write_line(src)
        writer.write_line(')')
        writer.indent_dec()
        if len(self.__link_list) > 0:
            writer.write_line('')
            writer.write_line(f'target_link_libraries( {self.__libname}')
            writer.indent_inc()
            for lib in self.__link_list:
                writer.write_line(lib)
            writer.write_line(')')
            writer.indent_dec()
        if len(self.__property_list) > 0:
            writer.write_line('')
            writer.write_line(f'set_target_properties( {self.__libname}')
            writer.indent_inc()
            writer.write_line('PROPERTIES')
            for prop, val in self.__property_list:
                writer.write_line(f'{prop} {val}')
            writer.write_line(')')
            writer.indent_dec()


class ModuleGen(GenBase):
    """Python モジュールの初期化コードを生成するクラス
    """

    def __init__(self, *,
                 modulename,
                 header_name=None,
                 submodule=False,
                 namespace=None,
                 doc_str='',
                 pyclass_gen_list=[],
                 header_include_files=['ym_config.h'],
                 source_include_files=[],
                 extra_include_files=[],
                 submodule_list=[],
                 include_dir_list=[],
                 target_list=[],
                 ex_init=None):
        super().__init__()
        self.modulename = modulename
        if header_name is None:
            header_name = f'{modulename}.h'
        self.header_name = header_name
        self.submodule = submodule
        self.namespace = namespace
        self.doc_str = doc_str
        self.gen_list = pyclass_gen_list
        self.header_include_files = header_include_files
        self.source_include_files = source_include_files
        self.extra_include_files = extra_include_files
        self.__include_dir_list = include_dir_list

        # サブモジュールのリスト
        self.submodule_list = submodule_list

        # ターゲットのリスト
        self.target_list = target_list

        # 追加の初期化コード
        self.ex_init_gen = ex_init

        # メソッド構造体の定義
        # モジュール定義の場合は関数がなくても空のテーブルをつくる．
        tbl_name = self.check_name('methods')
        self.__method_gen = MethodGen(self, tbl_name, module_func=True)

        # Python 拡張クラスのリスト
        self.__pyclass_list = [gen.pyclassname for gen in pyclass_gen_list]

    def add_method(self, name, *,
                   func_name=None,
                   arg_list=[],
                   func_body=None,
                   doc_str=''):
        """メソッド定義を追加する．
        """
        # デフォルトの関数名は Python のメソッド名をそのまま用いる．
        func_name = self.complete_name(func_name, name)
        self.__method_gen.add(func_name,
                              name=name,
                              arg_list=arg_list,
                              is_static=False,
                              func_body=func_body,
                              doc_str=doc_str)

    def add_submodule(self, name, init_func):
        """サブモジュールを追加する．
        """
        self.submodule_list.append((name, init_func))

    def make_all(self, *,
                 include_dir,
                 source_dir):

        # ソースディレクトリの設定
        if self.submodule:
            source_dir = os.path.join(source_dir, self.modulename)

        # モジュール用のファイル生成
        filename = os.path.join(include_dir, f'{self.header_name}')
        with open(filename, 'wt') as fout:
            self.make_header(fout=fout)
        filename = os.path.join(source_dir, f'{self.modulename}_module.cc')
        with open(filename, 'wt') as fout:
            self.make_source(fout=fout)

        # 部品クラス用のファイル生成
        for gen in self.gen_list:
            filename = os.path.join(include_dir, f'{gen.pyclassname}.h')
            with open(filename, 'wt') as fout:
                gen.make_header(fout=fout)
            filename = os.path.join(source_dir, f'{gen.pyclassname}.cc')
            with open(filename, 'wt') as fout:
                gen.make_source(fout=fout)

        # CMakeLists.txt 生成
        filename = os.path.join(source_dir, 'CMakeLists.txt')
        with open(filename, 'wt') as fout:
            self.make_cmakefile(fout=fout)

    def make_header(self, fout=sys.stdout):
        """ヘッダファイルを出力する．
        """

        include_files =  self.header_include_files[:]
        include_files += self.extra_include_files

        # Generator リスト
        gen_list = []
        gen_list.append(IncludesGen(self.header_include_files))
        gen_list.append(BeginNamespaceGen(self.namespace))
        gen_list.append(EndNamespaceGen(self.namespace))

        # 置換リスト
        replace_list = []
        # 年の置換
        replace_list.append(('%%Year%%', self.year()))
        # モジュール名の置換
        replace_list.append(('%%ModuleName%%', self.modulename))
        # インタロック用のモジュール名の置換
        replace_list.append(('%%CapModuleName%%',
                             self.modulename.upper()))

        template_file = self.template_file('custom.h')
        writer = CxxWriter(fout=fout)
        self.make_file(template_file=template_file,
                       writer=writer,
                       gen_list=gen_list,
                       replace_list=replace_list)

    def make_source(self, fout=sys.stdout):
        """モジュールの定義ファイルを出力する．
        """

        include_files =  self.source_include_files[:]
        include_files += [f'pym/{name}.h' for name in self.__pyclass_list]
        include_files += [f'pym/{name}.h' for name, _ in self.submodule_list]
        include_files += self.extra_include_files

        # Generator リスト
        gen_list = []
        gen_list.append(IncludesGen(include_files))
        gen_list.append(BeginNamespaceGen(self.namespace))
        gen_list.append(EndNamespaceGen(self.namespace))
        gen_list.append(PatGen('%%EXTRA_CODE%%',
                               lambda writer: self.make_extra_code(writer)))
        gen_list.append(PatGen('%%INIT_CODE%%',
                               lambda writer: self.make_init_code(writer)))

        # 置換リスト
        replace_list = []
        # 年の置換
        replace_list.append(('%%Year%%', self.year()))
        # モジュール名の置換
        replace_list.append(('%%ModuleName%%', self.modulename))
        # DOC_STR の置換
        replace_list.append(('%%DOC_STR%%', self.doc_str))
        # 名前空間の置換
        if self.namespace is not None:
            replace_list.append(('%%NAMESPACE%%', self.namespace))

        template_file = self.template_file('custom_module.cc')
        writer = CxxWriter(fout=fout)
        self.make_file(template_file=template_file,
                       writer=writer,
                       gen_list=gen_list,
                       replace_list=replace_list)

    def make_extra_code(self, writer):
        if self.__method_gen is not None:
            self.__method_gen(writer)

    def make_init_code(self, writer):
        # サブモジュールの登録
        if len(self.submodule_list) > 0:
            writer.gen_CRLF()
        for name, init_func in self.submodule_list:
            with writer.gen_if_block(f'!PyModule::reg_submodule(m, "{name}", {init_func}())'):
                writer.write_line('goto error;')

        # 拡張クラスの登録
        if len(self.__pyclass_list) > 0:
            writer.gen_CRLF()
        for pyclass in self.__pyclass_list:
            with writer.gen_if_block(f'!{pyclass}::init(m)'):
                writer.write_line('goto error;')

        # 追加の初期化コード
        if self.ex_init_gen is not None:
            self.ex_init_gen(writer)

    def make_cmakefile(self, fout):
        """ CMakeLists.txt の生成
        """
        gen_list = []
        if self.submodule:
            gen_list.append(PatGen('%%INCLUDE_DIRECTORIES%%', None))
            gen_list.append(PatGen('%%ADD_SUBDIRECTORY%%', None))
            gen_list.append(PatGen('%%SET_SOURCES%%',
                                   lambda writer: self.make_set_sources(writer)))
        else:
            gen_list.append(PatGen('%%INCLUDE_DIRECTORIES%%',
                                   lambda writer: self.make_include_directories(writer)))
            gen_list.append(PatGen('%%ADD_SUBDIRECTORY%%',
                                   lambda writer: self.make_add_subdirectory(writer)))
            gen_list.append(PatGen('%%SET_SOURCES%%', None))
        gen_list.append(PatGen('%%TARGETS%%',
                               lambda writer: self.make_targets(writer)))
        template_file = self.template_file('CMakeLists.txt')
        writer = CmWriter(fout=fout)
        self.make_file(template_file=template_file,
                       writer=writer,
                       gen_list=gen_list)

    def make_include_directories(self, writer):
        dir_list = []
        dir_list.append('${Python3_INCLUDE_DIRS}')
        dir_list.append('${CMAKE_CURRENT_SOURCE_DIR}/include')
        for inc_dir in self.__include_dir_list:
            dir_list.append(inc_dir)
        writer.gen_include_directories(dir_list)

    def make_add_subdirectory(self, writer):
        for name, _ in self.submodule_list:
            writer.gen_add_subdirectory(name)
        writer.gen_add_subdirectory('pytest')

    def make_set_sources(self, writer):
        writer.write_line(f'set ( {self.modulename}_SOURCES')
        writer.indent_inc()
        writer.write_line(f'${{CMAKE_CURRENT_SOURCE_DIR}}/{self.modulename}_module.cc')
        for pyclass in self.__pyclass_list:
            writer.write_line(f'${{CMAKE_CURRENT_SOURCE_DIR}}/{pyclass}.cc')
        writer.write_line('PARENT_SCOPE')
        writer.write_line(')')
        writer.indent_dec()

    def make_targets(self, writer):
        for target in self.target_list:
            target(writer)
