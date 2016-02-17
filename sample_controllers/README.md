本ディレクトリについて
-----------------
本プラグインを使用するコントローラの実装例を格納しています。  


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
* OculusRiftDK1SensorData （Oculus の姿勢を示すオイラー角）を、man-nii の首のクォータニオンへ変換する。
* man-nii の首のクォータニオンを、SIGViewer 上のアバターへセットする。

