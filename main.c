#include "mcc_generated_files/mcc.h"
#include <xc.h>
#include <stdlib.h>

int DICE[6]={0b000010, 0b000001, 0b000011, 0b100001, 0b100011, 0b110001}; // サイコロの点灯パターン

//unsigned xs;

void mainLoop(void);
void randomSeed(long val);

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // 乱数の初期化
    randomSeed(0);
    
    // WPM initialize //
        // Fosc = 125kHz 
        TRISA2 = 1;              // disable the CCP1 pin output driver by setting the associated TRIS bit
        PR2 = 31;                 // load the PR2 resister with the WPM period value (31 + 1) x 4 / 125e3 = 1.0ms
        CCP1CON = 0b00001100;    // Configure the CCP1 module for the PWM mode by loading the CCP1CON register with the appropriate values.
        CCPR1L = 16/4;           // Load the CCPR1L register and the DC1B1 bits of the CCP1CON register, with the PWM duty cycle value. duty 50% 
        // Configure and start Timer2:
        TMR2IF = 0;              //  Clear the TMR2IF interrupt flag bit of the PIR1 register. See Note below.
        T2CON = 0b00000000;      // Configure the T2CKPS bits of the T2CON register with the Timer prescale value.
        TMR2ON = 1;              //  Enable the Timer by setting the TMR2ON bit of the T2CON register.
    // Enable PWM output pin:
    //  Wait until the Timer overflows and the TMR2IF bit of the PIR1 register is set. See Note below.
    //  Enable the CCP1 pin output driver by clearing the associated TRIS bit.
            
    while (1)
    {
        // Add your application code
        mainLoop();
    }
}


void mainLoop(void){
int val_dice;
int prev_dice;
        
    for (int a = 30 ; a > 0 ; a--){ // サイコロの目を変える回数
        val_dice = rand() % 6  ;
        if (val_dice != prev_dice) {
                PORTA = DICE[val_dice];
                TRISA2=0;
                __delay_ms(1);
                TRISA2=1;
                prev_dice = val_dice;
        }
    }
    TRISA2=0;
    __delay_ms(600);
    TRISA2=1;
    
    for (int i=0; i<255 ; i++){
        __delay_ms(10);
    }
    PORTA=0;
    SLEEP();
}

void randomSeed(long val)
{
     unsigned int temp;
     static long randomx;
     TRISA2=1;
     ANSA2=1;
     WPUA2=0;
     ADCON0=0b00001001;
     if (val == 0) {
          while(1) {
               GO_nDONE = 1 ;      // PICにアナログ値読取り開始を指示
               while(GO_nDONE) ;   // PICが読取り完了するまで待つ
               temp = ( ADRESH << 8 ) | ADRESL ;  // 10ビットの分解能力です
               if (temp > 0) break ;
          }
          randomx = temp ;
     } else randomx = val ;   // 指定数値をそのまま初期値とする
     ADCON0=0;
     WPUA2=1;
     ANSA2=0;
     TRISA2=0;
     srand(randomx);
}
