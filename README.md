# soft2-lifegame
![gif image](./readme1.gif) ![gif image](./readme2.gif)
## ファイルについて
**課題1-3は`mylife3.c`ですべて実行できるようになっています。**

色々ファイル混じってますが基本は
- `mylife3.c` (必須課題)
- `mylife4.c` ([発展課題についてはこちら](#hatten))

のみで完結します。いくつかのサンプルとしてlifファイルとrleファイルが何個か含まれています。

- `makeanime.c`
- `lzw.c`
- `fileopentesst.c`
- `giftest.c`
- `rletest.c`
  
は機能を追加するときに、試験用に作ったコードです。特に見なくても大丈夫ですが、機能ごとに見る分にはわかりやすい（かも）しれないです。
```
mylife3およびmylife4では実行時にランダム配置を指定するとrandom.lifが生成されるようになっているため、注意してください。
```
## 実行方法
コンパイル
```bash
gcc -o mylife\(3|4\) mylife\(3|4\).c
```
実行（ランダム配置）
```bash
./mylife\(3|4\)
```
実行（rleファイルまたはlifファイル指定）
```bash
./mylife3 hogehoge.\(rle|lif\)
```
<a id = "hatten"></a>
## 発展課題
ライフゲームの過程をアニメーションgifにして保存するようにしました。ファイル名は`lifegamerev2.gif`です。

基本の流れはこんな感じです。
- 通常のライフゲームを回す(課題1-3をそのまま流用)
- generationごとに盤面配列（`cell[height][width]`）を3倍に拡大して、1次元配列`code`に入れる。
- `code`に含まれている`0, 1`の列を**lzw圧縮**を用いて圧縮
- あとはgifのフォーマットどおりに`gif`配列に必要な情報を挿入
- `fwrite`を用いて書き込み。

gifの仕様上（関数を作っていないため）、対応画像サイズは255×255までです。
### 色の変え方
関数`main`内の`gifdata`
```c
  unsigned char gifdata[1000000] = {
      'G',  'I',  'F',  '8',  '9',  'a',  width*3, 0x00, height*3, 0x00, 0x80, 0x00,
      0x00, 0xb0, 0xc4, 0xde, 0xFF, 0xFF, 0xFF,  0x21, 0xFF,   11,   'N',  'E',
      'T',  'S',  'C',  'A',  'P',  'E',  '2',   '.',  '0',    3,    1,    0,
      0,    0,    0x21, 0xF9, 4,    4,    10,    0,    0,      0};
```
のうち、`0xb0,0xC4,...,0xFF*3`となっているところが16進数カラーコードです。最初3つが背景、次の3つがライフを表します。今回は、2色画像にしか対応していませんが、原理的には`0xFF`のあとにまだ2色追加することができます。本来のgif画像は256色まで対応出来るので、その気があれば256色対応も可能ですが、その際は`makegif`関数の中も少し弄らないといけません。

### lzw圧縮について
lzw圧縮は辞書データを用いた圧縮です。C++には`map`があるので、簡単に検索、登録が出来るのですが、C言語にはないので、簡易的な構造体を作りました。辞書番号をインデックスとし、各`node`に、文字列データ＝valueを用意しました。検索は線形探査なのでO(N)です。本来`map`では、二分木を使ってO(logN)で検索をやっていたはずですが、今回は01の列を登録するだけなので、それほど数も多くないと判断し、線形探査で妥協です。

### lzw圧縮後
lzwに圧縮すると、次は圧縮時の辞書データのbit数に応じて変換した10進数を2進数に変換します。2進数に変換したものを、早いものから順に、右詰め（リトルエンディアン）で、byteに変換していき、これを10進数に変換したものを`imageboard`に順番に入れます。
10→2→右詰め→10進数となっていて、正直効率は良くないですが、仕方ない…？

### overflowについて
genの範囲を変えることで、gifにする世代数を調整出来ますが、上記の通り、`gifdata`のサイズが100万しかないので、世代数もしくは、画像のサイズ次第で割と簡単にあふれます。その時は`gifdata`をヒープにして、`gifindex`に応じて`realloc`すれば`overflow`は避けられます。が、面倒だと思うので大人しく収まる範囲内で遊ぶのが良いと思います。

### 実行時間について
計測は正確にはしてないですが、多分100世代で3分くらいかかります。どうしても早くみたい！というときは、`mylife4lite.c`を使えば、画像の拡大をしないので、サイズも小さくなり、100世代で1分半くらいで終わります。ファイル名は`lifegamelite.gif`です。とりあえずドキュメントに書かれた通りにオーダーも考えずに実装したため、こんな感じになってしまってます…。ちなみに`mylife4lite.c`の画像サイズは極めて小さい(40×70)ですが、`VSCode`の内蔵イメージビューアーでは`ニアレストネイバー法`を拡大縮小に用いているようで（多分）、拡大してもドットが綺麗に見えるため、別に、`mylife4.c`実行時とさほど変わらない結果が得られます。

![gif image](./readme1.gif)

こっちが拡大したもので

![gif image2](./readme2.gif)

こっちが、ピクセル数そのままのやつ

## 参考にしたサイトたち
- [GIF公式document](https://www.w3.org/Graphics/GIF/spec-gif89a.txt)
- [GIFのバイナリを読んでみた from Qiita](https://qiita.com/7shi/items/33117c6c369d37dc6cdd)
- [GIF画像ファイルフォーマット from 略して仮](http://menyukko.ifdef.jp/cauldron/dtgifformat.html)
- [LZW圧縮アルゴリズム\[実践的なGIFファイルの作成\] from プチモンテ](https://www.petitmonte.com/math_algorithm/lzw_gif.html)
- [GIF from wikipedia (英語版)](https://en.wikipedia.org/wiki/GIF#File_format)
- [GIFフォーマットの詳細 from とほほのWWW入門](https://www.tohoho-web.com/wwwgif.htm#ApplicationExtension)

## 補足情報
バイナリエディタとして`Stirling`を使いました。（サポートが22年前に切れていて驚き。Windowsの互換性の高さよ。）

[ダウンロードはこちらから](https://www.vector.co.jp/soft/win95/util/se079072.html)


## はじめてバイナリをいじってみた感想
**Photoshop**ぱねえ。