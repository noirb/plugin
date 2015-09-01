#Controller#

Controller の実装例を格納しています．  
SIGVerse の man-nii のアバターを，KinectV2 と OculusRiftDK1 で操作する Controller の実装例です．  

（Ubunutuの端末で）  
各ディレクトリ内で make すると，各 Controller でリンクする共有オブジェクトライブラリ（.so）を生成します．  
その後， sigserver.sh -w ./world.xml とコマンドを入力すると，SIGServer が起動し，  
 Plugin からのメッセージの受信待ち状態になります．  
その後，SIGViewer で Connect ボタンをクリックして START ボタンをクリックし， Services から各 Plugin をスタートさせると，  
各 Controller が動作します．

##ManNiiAvatarControllerByKinectV2##

KinectV2 を使ってアバターを操作するControllerの実装例です．  
Plugin から送られるメッセージを受け取った時の処理は，主に次の通りです．

* KinectV2_vs2013.sig を使用して送信されるメッセージを受け取る．
* 受け取ったメッセージに基づいて KinectV2SensorData クラスのインスタンスを生成（decode）する．
* KinectV2SensorData （KinectV2 で計測された全身のクォータニオン）を，man-nii のクォータニオンへ変換する．
* man-nii のクォータニオンを，SIGViewer 上のアバターへセットする．


##ManNiiAvatarControllerByOculus##

OculusRiftDK1 を使ってアバターを操作するControllerの実装例です．  
Plugin から送られるメッセージを受け取った時の処理は，主に次の通りです．

* OculusRiftDK1_vs2010.sig を使用して送信されるメッセージを受け取る．
* 受け取ったメッセージに基づいて OculusRiftDK1SensorData クラスのインスタンスを生成（decode）する．
* OculusRiftDK1SensorData （Oculus の姿勢を示すオイラー角）を，man-nii の首のクォータニオンへセットする．
* man-nii の首のクォータニオンを，SIGViewer 上のアバターへセットする．

##ControllerCommon##

上記3つの Controller で共通する機能を持つクラスを実装したソースコードを格納しています．

* AvatarController クラスの実装が含まれています．
 * 上記3つのControllerは，AvatarController クラスを継承しています．
* Posture クラスの実装が含まれています．
* Posture クラスを継承する ManNiiPosture クラスの実装が含まれています．
 * man-nii の姿勢の情報は， ManNiiPosture クラスで扱います．
