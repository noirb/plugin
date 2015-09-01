基本的な使い方
-----------------

sigverseフォルダには、Controllerを作成する際に使用できる共通コードや、各種デバイス用の汎用的なPluginなどが格納されています。
samplesフォルダには、sigverseフォルダ中の共通コードを使用した、各種サンプルControllerを格納しています。

例えばOculus Rift DK1を使用したい場合、
sigverse/plugin/OculusRiftDK1_vs2010/OculusRiftDK1_vs2010.slnを使用して、Plugin(OculusRiftDK1_vs2010.sig)を作成し、
samples/ManNiiAvatarControllerByOculusDK1/Makefileを使用して、Controller(ManNiiAvatarControllerByOculusDK1.so)を作成します。

Pluginを使用して Oculus Rift DK1 のセンサーデータをControllerに送信します。
Controllerはそれを受信して、ヒューマンアバターの頭を動かします。

