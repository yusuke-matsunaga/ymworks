チュートリアル
~~~~~~~~~~~~~~~

簡単なSAT問題
--------------

以下のCNF式に対するSAT問題を考える．

.. math::

   (a \lor b) \land (\lnot b \lor \lnot c) \land (\lnot a \lor c)

この式は例えば :math:`a = 1, b = 0, c = 1` という割り当てで全ての
節を充足することができる．
この問題を `SatSolver` を用いて解くには以下のようなコードを用いる．

.. code-block:: cpp

   // SATソルバの生成
   SatSolver solver();

   // 変数(正リテラル)の生成
   auto a = solver.new_variable();
   auto b = solver.new_variable();
   auto c = solver.new_variable();

   // 節の追加
   solver.add_clause(a, b);
   solver.add_clause(~b, ~c);
   solver.add_clause(~a, c);

   // 解を求める．
   auto res = solver.solve();

   if ( res == SatBool3::True ) {
       // 解が求められた．
       auto model = solver.model();
   }
   else if ( res == SatBool3::False ) {
       // 充足不能
   }
   else if ( res == SatBool3::X ) {
       // アボート
       // 時間制約などの制約違反が起こった．
   }

まず， `SatSolver` インスタンスの生成を行う．
この例では引数を与えずにデフォルトパラメータを用いているが，
様々な初期化オプションが存在する．
詳しくは `SatInitParam` を参照すること．

変数は `SatSolver::new_variable()` で生成する．
返り値の型は `SatLiteral` でこの関数は常に正リテラルを返す．
負リテラルを作るには演算子 `~` を用いる．

`SatSolver` に節を追加するには `SatSolver::add_clause()`
を用いる．
この例では `solver.add_clause(~a, b)` のように2つの `SatLiteral`
を引数にしている．
`SatSolver::add_clause()` はオーバーロードされた別バージョンが
用意されていて `SatLiteral` を1個から6個まで指定することが
できる．
また， `std::vector<SatLiteral>&` を指定することもできる．
例えば `solver.add_clause(~a, b)` と同様のことは以下のコードでも
行える．

.. code-block:: cpp

   std::vector<SatLiteral> lits{~a, b};
   solver.add_clause(lits);

`SatSolver::solve()` で現在保持しているCNF式に対する充足可能性
を調べる．
返り値の型は `SatBool3` で `SatBool3::True` が充足可，
`SatBool3::False` が充足不可，
`SatBool3::X` が不明を表す．
通常は充足可か充足不可となるが，
時間やメモリ制限を超えた場合は途中で処理が中断するため
不明となる．

直前の `SatSolver::solve()` が充足可だった場合には，
CNF式を充足させる割り当てを `SatSolver::model()`
を用いて取得することができる．
この関数の返り値は `const SatModel&` である．
`SatModel` は意味的には `SatLiteral` をキーにして
対応する値( `SatBool3` )を保持する辞書である．
結果の値は `SatBool3` となっているが，
通常の `SatSolver::solve()` 関数の結果は
`SatBool3::True` か `SatBool3::False` の二択となっている．
なお， `SatSolver::model()` は `SatSolver` 内部のデータメンバ
への参照を返しているため，
その後の `SatSolver::solve()` の起動により内容が変化する．
外部でこの結果を利用する時は一旦コピーを作成する必要がある．

.. code-block:: cpp
   :caption: 危険な例

   // 充足解を求める．
   auto r = solver.solve();
   if ( r != SatBool3::True ) {
     return;
   }

   // 参照をそのまま使う．
   auto& model = solver.model();
   // 今は大丈夫

   // 新たな節を追加
   solver.add_clause(x, y, z);

   // 新しい問題に対する充足解を求める．
   auto r1 = solver.solve();

   // この時点で model は別の解に変化している．

上のコード例では最初の問題に対する充足解のつもりで `model`
という変数を使っているが，
2度めの `SatSolver::solve()` を呼んだ時点でその内容が変化している．
これを防ぐには以下のようなコードを用いる．

.. code-block:: cpp
   :caption: 正しい例

   auto r = solver.solve();
   if ( r != SatBool3::True ) {
     return;
   }

   // コピーを作る．
   auto model = solver.model();

   // 新たな節を追加
   solver.add_clause(x, y, z);

   // 新しい問題に対する充足解を求める．
   auto r1 = solver.solve();

   // model は変化していない．

違いは参照を表す `&` の有無だけなので注意すること．
`SatSolver::model()` の結果を参照ではなく実体としてコピーしているので
以降の動作に関係なく `model` は独立したインスタンスとなる．
ただし， `SatModel` は `SatSolver` が保持している全ての変数に対する
割り当て結果を保持した辞書なので問題のサイズに比例した容量を必要とする．
またコピーも同様に問題のサイズに比例した時間を必要とする．
充足解をその場で利用するだけならば，
最初の例のように参照を用いた方が効率がよい．

`SatModel` の内容を取り出すには `[]` 演算子を用いる．

.. code-block:: cpp
   :caption: SatModel の使い方

   auto val_a = model[a];
   auto val_b = model[b];
   auto val_c = model[c];

   // val_a = SatBool3::True
   // val_b = SatBool3::False
   // val_c = SatBool3::True
   // のようになっている．

   auto val_avar = model[~a];

   // val_avar = SatBool3::False のはず

`SatModel::operator[]()` の引数は `SatLiteral` なので
最後の行のように負リテラルを用いることもできる．
この場合，正リテラルに対する値の否定の値となる．

Tseitin 符号化
----------------

前述のようにSATソルバはCNF式を入力として用いている．
そこで，CNF式以外の形式の命題論理式が与えられた場合には
それをCNF式に変換する必要がある．
ここで言う変換とは `equisatisfiable` と呼ばれる関係を満たすもので，
変換前の論理式を :math:`\phi_1` ，
変換後の論理式を :math:`\phi_2`
とした時に :math:`\phi_1` が充足可能な場合にのみ
:math:`\phi_2` も充足可能となる，
という条件を満たすもののことである．
論理式が表す論理関数が等価なものではないことに注意．
このような一件複雑な変換を用いる理由は，
任意の命題論理式を等価なCNF式に変換すると最悪の場合，
式のサイズの指数乗に比例した節を必要とするからである．

このような変換で用いられるのが Tseitin 符号化である．
例えば :math:`(a \land b) \lor (c \land d)`
という論理式をCNF式に変換することを考える．
単純には分配則を用いて
:math:`(a \lor c) \land (a \lor d) \land (b \lor c) \land (b \lor d)`
という等価なCNF式に変換することも可能だが，
この方法では結果の節の数が変換前の論理式のサイズの指数乗に比例する．
そこで， :math:`s = a \land b` ， :math:`t = c \land d` という
補助的な変数を導入する．
:math:`s = a \land b` の関係をCNF式で表すと以下のようになる．

.. math::

   (a \lor \lnot s) \land
   (b \lor \lnot s) \land
   (\lnot a \lor \lnot b \lor s)

:math:`t = c \land d` の関係も同様に表される．
最後に元の式を表す :math:`(s \lor t)` を加えればよい．
論理積以外の演算も同様に変換することができる．

このような変換を Tseitin 符号化と呼ぶ．
`SatSolver` は Tseitin 符号化を行うメンバ関数を用意している．
例えば，先程の :math:`s = a \land b` に相当するCNF式を生成するには
以下のようなコードを用いる．

.. code-block:: cpp
   :caption: ANDゲートの Tseitin 符号化

   solver.add_andgate(s, a, b)

`SatSolver::add_andgate()` には入力数が2入力から4入力までのバージョン
と `std::vector<SatLiteral>&` を用いたバージョンが用意されている．

その他，BUFFゲート，NOTゲート，NANDゲート，ORゲート，NORゲート，XORゲート，
XNORゲート用のメンバ関数が用意されている．
