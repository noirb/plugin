#OculusRiftDK1_vs2010#

OculuRiftDK1_vs2010.sig を生成する手順を説明します．  

##必要な環境など##

OculuRiftDK1_vs2010.sln を使って OculuRiftDK1\_vs2010.sig をビルドするために，次の環境などが必要です．

* Microsoft Visual C++ 2010 Express（以降はVS2010と表記します）
* VS2010 用にビルドされた SIGService.lib  
* Oculus 用の SDK（OculusSDK）がインストールされていること（動作確認したバージョンは0.2.5）

##ビルドの手順##

ビルドする場合は，次の手順を参考にしてください．  

1. OculusRiftDK1\_vs2010.sln を VS2010 で開く．
2. プロジェクトのアクティブな構成を Release にする．
3. Includeディレクトリ、Libraryディレクトリ、ライブラリ、ビルド後イベントなどの記載・パスを見直す．
4. ビルドする．

##VS2010用のSIGService.libについて##
OculusRiftDK1 を使うプラグインをビルドする場合， VS2010 で使える SIGService.lib が必要です．  

