#include "R2.hpp"

enum R2_MOVEMENT{
    GOTO_FIRST_RECEIVE,
    WAIT_FOR_FIRST_RECEIVE,
    GOTO_POSTZONE2,
    GOTO_SECOND_RECEIVE,
    WAIT_FOR_SECOND_RECEIVE,
    GOTO_POSTZONE3,
};


R2::R2(){

}


void R2::game(){
    for(int i=0;i<=R2_MOVEMENT::GOTO_POSTZONE3;i++){
        run(i);
    }
}


void R2::run(unsigned int movement_id){
    switch (movement_id)
    {
    case R2_MOVEMENT::GOTO_FIRST_RECEIVE:
        /* code */
        break;

    case R2_MOVEMENT::WAIT_FOR_FIRST_RECEIVE:
        /* code */
        break;

    case R2_MOVEMENT::GOTO_POSTZONE2:
        /* code */
        break;

    case R2_MOVEMENT::GOTO_SECOND_RECEIVE:
        /* code */
        break; 

    case R2_MOVEMENT::WAIT_FOR_SECOND_RECEIVE:
        /* code */
        break;

    case R2_MOVEMENT::GOTO_POSTZONE3:
        /* code */
        break;

    default:
        break;
    }
}