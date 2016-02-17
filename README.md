﻿基本的な使い方
-----------------

各種デバイス用の汎用プラグインを提供しています。
基本的に以下の３ステップを行います。

1. SIGViewerに登録するプラグイン(sig)を作成する
2. プラグインと通信するための共有ライブラリ(so)を作成する
3. コントローラ側で、作成した共有ライブラリ(so)を動的リンクして使用する



SIGViewerに登録するプラグイン(sig)を作成する
-----------------

srcs/sigverse/plugin/plugin/ の下の、各種デバイス名のフォルダ内にある、
ソリューションファイル(sln)をVisualStudioで開いてビルドし、作成します。


プラグインと通信するための共有ライブラリ(so)を作成する
-----------------

本リポジトリ直下のCMakeLists.txtを使用します。
事前にSIGServerをインストールして、SIGVERSE_PATHを設定しておく必要があります。
cmakeを行うことで、必要なヘッダファイルや共有ライブラリが、
SIGServerのインストールディレクトリ内に追加配備され、使用可能になります。

1. mkdir build
2. cd build
3. cmake ..
4. make
5. make install


コントローラ側で、作成した共有ライブラリ(so)を動的リンクして使用する
-----------------

前述の手順で共有ライブラリ(so)を追加配備しておけば、自作コントローラ等で使用可能です。
各種プラグインを使用するサンプルコントローラが、sample_controllersにあります。


