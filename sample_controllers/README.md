samples
-----------------
Controller の実装例を格納しています。  
本サンプルは、sigverseフォルダのコードを使用しています。
利用する際には、各フォルダ内にある Makefile 中の SIGVERSE_BUILD_PATH をsigverseフォルダの親フォルダに設定します。

（Ubunutuの端末で）  
各ディレクトリ内で make すると、各 Controller でリンクする共有オブジェクトライブラリ（.so）を生成します。  
以下に例を示します。

ManNiiAvatarControllerByKinectV2
-----------------
KinectV2 を使ってアバターを操作するControllerの実装例です。  
Plugin から送られるメッセージを受け取った時の処理は、主に次の通りです。

* KinectV2用Plugin から送信されるメッセージを受け取る。
* 受け取ったメッセージに基づいて KinectV2SensorData クラスのインスタンスを生成する。
* KinectV2SensorData （KinectV2 で計測された全身のクォータニオン）を、man-nii のクォータニオンへ変換する。
* man-nii のクォータニオンを、SIGViewer 上のアバターへセットする。

ManNiiAvatarControllerByOculusDK1
-----------------
OculusRiftDK1 を使ってアバターを操作するControllerの実装例です。  
Plugin から送られるメッセージを受け取った時の処理は、主に次の通りです。

* OculusRiftDK1用Plugin から送信されるメッセージを受け取る。
* 受け取ったメッセージに基づいて OculusRiftDK1SensorData クラスのインスタンスを生成する。
* OculusRiftDK1SensorData （Oculus の姿勢を示すオイラー角）を、man-nii の首のクォータニオンへセットする。
* man-nii の首のクォータニオンを、SIGViewer 上のアバターへセットする。

