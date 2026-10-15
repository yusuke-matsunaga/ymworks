SATサブモジュール
==================

C++ バージョンとほぼ同様のインターフェイスを用意している．
`SatSolver` クラスはその名の通りSATソルバを表すクラスであり，
同時にSAT問題を保持するクラスでもある．
`SatLiteral` クラスがSAT問題の変数(リテラル)を表す．
SAT問題で扱われる値は `SatBool3` クラスを用いる．
SAT問題の解 `SatModel` クラスが表す．

インポートの仕方
------------------

.. code-block:: python
   :caption: インポートの仕方

   import ymworks

   solver = ymworks.sat.SatSolver()
   ...

   # または

   from ymworks.sat import SatSolver

   solver = SatSolver()
   ...


以下のコード例ではこのインポート記述は省略する．


簡単なSAT問題
--------------

以下のCNF式に対するSAT問題を考える．

.. math::

   (a \lor b) \land (\lnot b \lor \lnot c) \land (\lnot a \lor c)

この式は例えば :math:`a = 1, b = 0, c = 1` という割り当てで全ての
節を充足することができる．
この問題を `SatSolver` を用いて解くには以下のようなコードを用いる．

.. code-block:: python

   # SATソルバの生成
   solver = SatSolver();

   # 変数(正リテラル)の生成
   a = solver.new_variable();
   b = solver.new_variable();
   c = solver.new_variable();

   # 節の追加
   solver.add_clause(a, b);
   solver.add_clause(~b, ~c);
   solver.add_clause(~a, c);

   # 解を求める．
   res = solver.solve();

   if res == SatBool3.true:
       # 解が求められた．
       model = solver.model();
   elif res == SatBool3.false:
       # 充足不能
   elif res == SatBool3.x:
       # アボート
       # 時間制約などの制約違反が起こった．

まず， `SatSolver` インスタンスの生成を行う．
この例では引数を与えずにデフォルトパラメータを用いているが，
様々な初期化オプションが存在する．
詳しくは `SatInitParam` を参照すること．

変数は `SatSolver.new_variable()` で生成する．
返り値の型は `SatLiteral` でこの関数は常に正リテラルを返す．
負リテラルを作るには演算子 `~` を用いる．

`SatSolver` に節を追加するには `SatSolver.add_clause()`
を用いる．
この例では `solver.add_clause(~a, b)` のように2つの `SatLiteral`
を引数にしている．
`SatSolver.add_clause()` は可変引数を受け取ることができるように
なっているので任意の要素数のリテラルを与えることができる．
また， `[~a, b, c]` のようにリテラルのリストを与えることもできる．
さらに以下のようにリテラルとリストを混在させることも可能である．

.. code-block:: pythone
   :caption: add_clause() でリテラルとリストを混在させる．

   solver.add_clause(a, [b, c], (d, e))

結果は `solver.add_clause(a, b, c, d, e)` と同一となる．
この例で示されているようにリストだけでなくタプルも使用可能である．
厳密には `シーケンス` プロトコルを実装している型ならなんでもよい．

`SatSolver.solve()` で現在保持しているCNF式に対する充足可能性
を調べる．
返り値の型は `SatBool3` で `SatBool3.true` が充足可，
`SatBool3.false` が充足不可，
`SatBool3.x` が不明を表す．
通常は充足可か充足不可となるが，
時間やメモリ制限を超えた場合は途中で処理が中断するため
不明となる．

直前の `SatSolver.solve()` が充足可だった場合には，
CNF式を充足させる割り当てを `SatSolver.model()`
を用いて取得することができる．
この関数の返り値は `SatModel` である．
`SatModel` は意味的には `SatLiteral` をキーにして
対応する値( `SatBool3` )を保持する辞書である．
結果の値は `SatBool3` となっているが，
通常の `SatSolver.solve()` 関数の結果は
`SatBool3.true` か `SatBool3.false` の二択となっている．

`SatSolver.model()` の値は `SatSolver.solve()` を呼ぶ度に変化するので
以前の結果をあとで参照する場合には `m = solver.model()` のように
変数にコピーしておくこと．
