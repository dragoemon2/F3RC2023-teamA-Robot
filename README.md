  
#  F3RC2023-teamA-Robot

メインボードf446に書き込むプログラムです．R1とR2のどちらもこのプログラムを使って動かします．
R1とR2の切り替えは`settings.hpp`の`#define USING_R1`のところを0か1にすることで行います．
デバッグ用のプログラムなどは`debug.cpp`の`R1::debug()`または`R2::debug()`に書いて，`main.cpp`からその関数を呼び出してください

本番で最も調整を行うであろう動線の部分はそれぞれ，`R1.cpp`，`R2.cpp`のそれぞれ`R1::run()`と`R2::run()`の部分です．書き方は察してください

```cpp

void R1::run(unsigned int movement_id){
    auto_moving = true;
    printf("auto_movement %d\n", movement_id);
    //動線をここに書く．

    switch (movement_id)
    {
    //ペットボトル置き場手前まで移動する
    case GOTO_FIRST_PRODUCT_STORAGE:
        driveBase.localization.setPosition(WEST_WALL_X + ROBOTSIZE, NORTH_WALL_Y - ROBOTSIZE, -PI/2); //壁当てでスタート
        //lasers.pair.activate(WEST, -WEST_WALL_X);
        //lasers.front.activate(SOUTH, -SOUTH_WALL_Y);
        //bottleArm.down(false);
        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 200, SOUTH_WALL_Y + ROBOTSIZE + 400, -PI/2); //ペットボトル置き場手前

        break;

    case GOTO_FIRST_CONTAINER_STORAGE:
        //ペットボトル起き場まで
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 300, -PI/2);

        //ペットボトル回収
        //bottleArm.close(true);
        //bottleArm.up(false);

        lasers.front.inactivate();
        lasers.pair.inactivate();

        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 550 + 300, -PI/2);
        driveBase.goTo(WEST_WALL_X+850+38+ROBOTSIZE+100, SOUTH_WALL_Y + ROBOTSIZE + 550 + 300, -PI/2);
        //lasers.front.activate(SOUTH, SOUTH_WALL_Y);
        driveBase.goTo(WEST_WALL_X+850+38+ROBOTSIZE+100, SOUTH_WALL_Y + ROBOTSIZE + 450, -PI/2);//コンテナ置き場手前に移動
        break;

    case GOTO_FIRST_DELIVERY:
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 300, -PI/2);
        
        //containerArm.close(true);
        //bottleArm.down(false);
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + 1200, -PI/2);
        driveBase.rotateTo(0);
        //bottleArm.open(true);

        driveBase.goTo(WEST_WALL_X + 1900, SOUTH_WALL_Y + 1200, 0);
        break;
    
    case GOTO_SECOND_PRODUCT_STORAGE:
        if(mm.mode != COMPLETELY_AUTO_MODE){
            driveBase.localization.setPosition(driveBase.localization.posX, SOUTH_WALL_Y + 1200, 0);
        }
        

        driveBase.goTo(WEST_WALL_X + 2000, SOUTH_WALL_Y + 1200, -PI/2);

        //受け渡し
        //R1から受け渡し検知信号を受信
        wait_seconds(1); 
        //containerArm.open(false);

        driveBase.rotateTo(-PI/2);

        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 200, SOUTH_WALL_Y + ROBOTSIZE + 750, -PI/2);
        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 200, SOUTH_WALL_Y + ROBOTSIZE + 400, -PI/2);

        break;

    case GOTO_SECOND_CONTAINER_STORAGE:
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 200, -PI/2);

        //bottleArm.close(true);
        //bottleArm.up(false);

        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 550 + 300, -PI/2);
        driveBase.goTo(WEST_WALL_X+850+38+ROBOTSIZE+100, SOUTH_WALL_Y + ROBOTSIZE + 550 + 300, -PI/2);
        driveBase.goTo(WEST_WALL_X+850+38+ROBOTSIZE+100, SOUTH_WALL_Y + ROBOTSIZE + 450, -PI/2);//コンテナ置き場手前に移動

        break;

    case GOTO_SECOND_DELIVERY:
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 200, -PI/2);
        
        //containerArm.close(true);
        //bottleArm.down(false);
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + 1200, -PI/2);
        driveBase.rotateTo(0);
        //bottleArm.open(true);
        driveBase.goTo(WEST_WALL_X + 1900, SOUTH_WALL_Y + 1200, 0);
        break;

    case GOTO_THIRD_PRODUCT_STORAGE:
        if(mm.mode != COMPLETELY_AUTO_MODE){
            driveBase.localization.setPosition(driveBase.localization.posX, SOUTH_WALL_Y + 1200, 0);
        }
        

        driveBase.goTo(WEST_WALL_X + 2000, SOUTH_WALL_Y + 1200, -PI/2);

        //受け渡し
        //R1から受け渡し検知信号を受信
        wait_seconds(1); 
        //containerArm.open(false);

        driveBase.rotateTo(-PI/2);

        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 200, SOUTH_WALL_Y + ROBOTSIZE + 750, -PI/2);
        driveBase.goTo(WEST_WALL_X + ROBOTSIZE + 200, SOUTH_WALL_Y + ROBOTSIZE + 400, -PI/2);

        break;

    case GOTO_THIRD_CONTAINER_STORAGE:
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 100, -PI/2);
        
        //ペットボトル回収
        //bottleArm.close(true);
        //bottleArm.up(false);

        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 550 + 300, -PI/2);
        driveBase.goTo(WEST_WALL_X+850+38+ROBOTSIZE+700, SOUTH_WALL_Y + ROBOTSIZE + 550 + 300, -PI/2);
        driveBase.goTo(WEST_WALL_X+850+38+ROBOTSIZE+700, SOUTH_WALL_Y + ROBOTSIZE + 450, -PI/2);//コンテナ置き場手前に移動
        break;

    case GO_FRONTOF_POSTZONE:
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + ROBOTSIZE + 200, -PI/2);//コンテナ置き場手前に移動
        //containerArm.close(true);
        //bottleArm.down(false);
        driveBase.goTo(driveBase.localization.posX, SOUTH_WALL_Y + 1200, -PI/2);
        //bottleArm.open(true);
        driveBase.goTo(WEST_WALL_X+2200, SOUTH_WALL_Y + 1200, -PI/2);

        
        break;

    case GOTO_POSTZONE1:
        if(mm.mode != COMPLETELY_AUTO_MODE){
            driveBase.localization.setPosition(WEST_WALL_X+ROBOTSIZE+2126+250, driveBase.localization.posY, -PI/2);
        }else{
            driveBase.goTo(WEST_WALL_X+ROBOTSIZE+2126+250, SOUTH_WALL_Y + ROBOTSIZE + 150, -PI/2);
        }
        
        driveBase.goTo(WEST_WALL_X+ROBOTSIZE+2126+250, SOUTH_WALL_Y + ROBOTSIZE + 200, -PI/2);
        break;
        
    
    default:
        printf("pass");
        break;
    }



    auto_moving = false;
}

```
