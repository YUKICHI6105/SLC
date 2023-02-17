# Step_Leg_Control
ステッピングモーターを使ったオムニ系足回りコード  
joy_nodeと併用で動きます。パッケージ名はSLC。  

##Launchファイル
SLC3.launch
SLC4.launch

joy_nodeと一緒に起動させます。


##SLC3_node
3輪オムニの制御用コード  

使用コントローラー:logcool F710?(Dモード)  

Bボタン mode_vel?に変更  
Aボタン mode_disable?に変更  

右スティック 倒した方向に移動   
LB　左回転  
RB　右回転  

#SLC4_node
4輪オムニの制御用コード  

使用コントローラー:logcool F710?(Dモード)  

Bボタン mode_vel?に変更  
Aボタン mode_disable?に変更  

右スティック 倒した方向に移動   
LB　左回転  
RB　右回転  
