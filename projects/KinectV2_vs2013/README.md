#KinectV2_vs2013#

KinectV2_vs2013.sig を生成する手順を説明します．  

##必要な環境など##

KinectV2_vs2013.sln を使って KinectV2_vs2013.sig をビルドするために，次の環境などが必要です．

* Windows8 （またはそれ以上のWindows OS）
* Visual Studio Express 2013 for Desktop （以降はVS2013と表記します）
* VS2013 用にビルドされた SIGService.lib  
※ VS2013 用の SIGService.lib をビルドする手順は，末尾に記述してあります．
* OpenCV がインストールされていること（本リポジトリでは 2.4.10 を使用していますが，2.4系ならどれでもOKだと思います）
* Kinect for Windows SDK 2.0 がインストールされていること

なお，「VS2013 用の～～」と表記しているものは，筆者の環境で「VS2013 を使ってビルドしたもの」という意味を含んでいます．  
おそらくは，VS2012 でビルドしたものでもOKだと思います．  

##ビルドの手順##

ビルドする場合は，次の手順を参考にしてください．  

1. GitHub からクローンしてきた KinectV2_vs2013.sln を VS2013 で開く．
2. プロジェクトのアクティブな構成を Release にする．
3. Includeディレクトリ、Libraryディレクトリ、ライブラリ、ビルド後イベントなどの記載・パスを見直す．
4. ビルドする．

