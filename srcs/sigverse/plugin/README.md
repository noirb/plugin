(1) plugin
----------
プラグインのソースコードのディレクトリです。

各種プラグインのフォルダ内に、VisualStudioでビルドするためのソリューションファイル(sln)が
含まれていますので、それを使用してプラグインファイルを作成します。
プラグインファイルの拡張子はsigです。

(2) controller
--------------
プラグインと通信を行うためのコントローラ側のソースコードのディレクトリです。  
共有ライブラリ(soファイル)生成には、本リポジトリ直下のCMakeLists.txtを使用します。

controller/commonには、Controller で共通する機能を持つクラスを実装したソースコードを格納しています。

* AvatarController クラスの実装が含まれています。
* Posture クラスの実装が含まれています。
* Posture クラスを継承する ManNiiPosture クラスの実装が含まれています。
 * man-nii の姿勢の情報は、 ManNiiPosture クラスで扱います。

(3) common
----------
pluginとcontrollerの両方に必要なソースコードのディレクトリです。
SensorDataクラスの実装が含まれています。
注意点などを次に列挙します。

* Windows と Ubuntu の両方で使用する。
* 実装には boost が含まれているので、ソースコードのビルドのために boost をインストールする必要がある。
