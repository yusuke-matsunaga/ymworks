#! /usr/bin/env python3

"""
group statement 用のパース関数を生成するスクリプト

:file: handlerdata.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2021 Yusuke Matsunaga, All rights reserved.
"""

import itertools
import sys


def expand(pat):
    """パタンを解析して展開する"""

    state = 'normal'
    fmt_str = ''
    pat_list_list = list()
    for c in pat:
        if state == 'normal':
            if c == '(':
                state = 'choice'
                fmt_str += '{}'
                pat_list = list()
                pat = ''
            else:
                fmt_str += c
        elif state == 'choice':
            if c == ')':
                pat_list.append(pat)
                pat_list_list.append(pat_list)
                state = 'normal'
            elif c == '|':
                pat_list.append(pat)
                pat = ''
            else:
                pat += c

    n = len(pat_list_list)
    ans_list = list()
    if n == 0:
        ans_list.append(fmt_str)
    elif n == 1:
        for pat in pat_list_list[0]:
            ans = fmt_str.format(pat)
            ans_list.append(ans)
    elif n == 2:
        for pat1, pat2 in itertools.product(pat_list_list[0], pat_list_list[1]):
            ans = fmt_str.format(pat1, pat2)
            ans_list.append(ans)
    elif n == 3:
        for pat1, pat2, pat3 in itertools.product(pat_list_list[0], pat_list_list[1], pat_list_list[2]):
            ans = fmt_str.format(pat1, pat2, pat3)
            ans_list.append(ans)
    else:
        print('pattern is too deep')
        return None
    return ans_list


class HandlerData:
    """group statement 用のパース関数記述を生成するクラス

    基本的には HandlerData の内容に従ってパース関数の記述を生成すればよいが，
    このクラスでは group statement 用のハンドラのリストを更新する．
    simple attribute と complex attribute のハンドラは固定だが，
    group statement 用のハンドラはここで定義される．
    そのため，最終的にはたのハンドラから使われているハンドラは
    すべてここで定義されなければならない．
    """

    def __init__(self):
        # simple attribute 用のハンドラ関数名のセット
        self._simple_handlers = {
            'int',
            'float',
            'string',
            'bool',
            'delay_model',
            'direction',
            'expr',
            'function',
            'timing_sense',
            'timing_type',
            'vartype'
        }

        # complex attribute 用のハンドラ関数名のセット
        self._complex_handlers = {
            'float_float',
            'float_float_string',
            'float_string',
            'float_vector',
            'int_float',
            'int_float_vector',
            'int_string',
            'int_vector',
            'string',
            'string_float',
            'string_int',
            'string_string',
            'string_string_string',
            'fanout_length',
            'technology',
            'values',
            'int_list',
            'string_list',
            'vartype_list',
        }

        # group statement のヘッダ用のハンドラ関数名の辞書
        self._group_handlers = {
            'empty': 'Empty',
            'string': 'Str',
            'string_string': 'StrStr',
            'string_string_int': 'StrStrInt',
            'opt_string': 'OptStr',
        }

        # group 名をキーにしてヘッダハンドラを格納する辞書
        self._group_dict = dict()

        # group statement 用のハンドラ情報の辞書
        # 初期値は空
        self._handler_dict = dict()

        # 他のハンドラから使用されるハンドラのセット
        self._required = set()

    def add_group_data(self, name, header, *,
                       simple_attrs=(),
                       complex_attrs=(),
                       groups=()):
        """group statement 用のハンドラ記述を追加する．
        :param name: グループ名
        :param header: ヘッダ用のパース関数
        :param simple_attrs: simple attribute 用のデータ
        :param complex_attrs: complex attribute 用のデータ
        :param groups: group statement 用のデータ

        ここで与えられるデータの属性名はメタ記述になっているので
        展開する必要がある．
        simple attribute と complex attribute のハンドラ関数が正しいか確認する．
        定義済みの group handler のリストと参照されている group handler
        のリストを更新する．
        """

        if name in self._group_dict:
            # すでに同名のグループが登録されていた．
            print("Error: {} is already defined.".format(name))
            return False

        if header not in self._group_handlers:
            print('Error: {} is not a valid header handler({})'.format(header, name))
            return False

        self._group_dict[name] = self._group_handlers[header]

        # 登録済みのキー
        keys = set()

        # simple attribute をセットする．
        for pat, handler in simple_attrs:
            if handler not in self._simple_handlers:
                print('Error: {} is not a valid simple handler({})'.format(
                    handler, pat))
                return False
            shandler = 'simple_' + handler
            key_list = expand(pat)
            for key in key_list:
                if key in keys:
                    print('Error: {} appears more than once.'.format(key))
                comp_key = name + ':' + key
                self._handler_dict[comp_key] = shandler
                keys.add(key)

        # complex attribute をセットする．
        for pat, handler in complex_attrs:
            if handler not in self._complex_handlers:
                print('Error: {} is not a complex handler({})'.format(handler, pat))
                return False
            chandler = 'complex_' + handler
            key_list = expand(pat)
            for key in key_list:
                if key in keys:
                    print('Error: {} appears more than once.'.format(key))
                comp_key = name + ':' + key
                self._handler_dict[comp_key] = chandler
                keys.add(key)

        # group statement をセットする．
        for pat, handler in groups:
            ghandler = 'group_' + handler
            self._required.add(handler)
            key_list = expand(pat)
            for key in key_list:
                if key in keys:
                    print('Error: {} appears more than once.'.format(key))
                comp_key = name + ':' + key
                self._handler_dict[comp_key] = ghandler
                keys.add(key)

        return True

    def merge(self, source):
        for key, header in source._group_dict.items():
            if key in self._handler_dict:
                print('{}: duplicated group'.format(key))
                continue
            self._group_dict[key] = header
        for key, handler in source._handler_dict.items():
            if key in self._handler_dict:
                print('{}: duplicated key'.format(key))
                continue
            self._handler_dict[key] = handler

    def check(self):
        undef_list = list()
        for name in self._required:
            if name not in self._group_dict:
                undef_list.append(name)
        if len(undef_list) > 0:
            for name in undef_list:
                print('Error: {} is undefined.'.format(name9))
            return False
        else:
            return True

    def print_functions(self, *, fout=sys.stdout):
        header = """#ifndef PARSE_GROUP_H
#define PARSE_GROUP_H

/// @file parse_group.h
/// @brief group statement 用のパーサー関数定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/Parser.h"
#include "dotlib/AstAttr.h"

BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @{
/// @name group statement を読み込む関数
//////////////////////////////////////////////////////////////////////
"""
        fout.write(header)

        for name, header in self._group_dict.items():
            fmt = """
/// @brief {0} group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_{0}(
  Parser& parser,           ///< [in] パーサー
  const string& kwd,        ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{{
  return parser.parse_group_statement(kwd, kwd_loc, "{0}", Parser::s{1}Header);
}}
"""
            fout.write(fmt.format(name, header))

        footer = """
//////////////////////////////////////////////////////////////////////
/// @}
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_DOTLIB

#endif // PARSE_GROUP_H
"""
        fout.write(footer)

    def print_dict(self, *, fout=sys.stdout):
        for key, handler in self._handler_dict.items():
            fout.write('  {{ "{0}", {1} }},\n'.format(key, handler))

    def dump(self):
        for key, handler in self._handler_dict.items():
            print(key, handler)
