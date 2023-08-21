  
#  F3RC2023-teamA-Robot

いままで作ったプログラムを統合したやつです．

1. DrivingSystem (足回り・エンコーダー自己位置推定)
2. LaserLocalization (レーザー・レーザー自己位置推定)
3. Buzzer (ブザー，例外処理)
4. (Simulation) (モーター等の動作確認)

が入っています．今後

5. Communication (コントローラーとの通信)
6. Arm (サーボモーター・エアシリンダー)

を追加します．

統合はRobot.hppの`Robot`というクラス内に書きます．
`Robot`を継承したクラス`R1`,`R2`をつくり，そこにロボットの動きを記述する予定です．
