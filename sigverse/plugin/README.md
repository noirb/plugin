(1) controller
--------------
コントローラのソースコードのディレクトリです。  

(2) plugin
----------
サービスプロバイダのソースコードのディレクトリです。
実装したサービスプロバイダの機能は次のようなものです。

(3) common
----------
ControllerとPluginの両方に必要なソースコードのディレクトリです。  
SensorDataクラスの実装が含まれています。  
注意点などを次に列挙します。

* Windows と Ubuntu の両方で使用する。
* 実装には boost が含まれているので、ソースコードのビルドのために boost をインストールする必要がある。
* Ubuntu への boost のインストール例（端末で打つコマンド）を次に示す。

` sudo apt-get install boost-all-dev`

(4) controller/common
----------
Controller で共通する機能を持つクラスを実装したソースコードを格納しています。

* AvatarController クラスの実装が含まれています。
 * 上記3つのControllerは、AvatarController クラスを継承しています。
* Posture クラスの実装が含まれています。
* Posture クラスを継承する ManNiiPosture クラスの実装が含まれています。
 * man-nii の姿勢の情報は、 ManNiiPosture クラスで扱います。

(5) plugin/common
----------
Plugin に共通する機能（クラス）を実装しているプログラムのソースコードを格納しています。  

