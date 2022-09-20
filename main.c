#include "mcc_generated_files/mcc.h"
#include <xc.h>
#include <stdlib.h>

int DICE[6]={0b000010, 0b000001, 0b000011, 0b100001, 0b100011, 0b110001}; // �T�C�R���̓_���p�^�[��

//unsigned xs;

void mainLoop(void);
void randomSeed(long val);

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // �����̏�����
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
        mainLoop();
    }
}


void mainLoop(void){
int val_dice;
int prev_dice;
        
    for (int a = 30 ; a > 0 ; a--){ // �T�C�R���̖ڂ�ς����
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
    
    // ��莞�Ԃ��o�߂�����X���[�v
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
     TRISA0=1;  // ���͂ɐݒ�
     ANSA0=1;   // �A�i���O���͂ɐݒ�
     WPUA0=0;   // �����v���A�b�v����
     ADCON0=0b00000101; // AN0��I��, ADC�L��
     if (val == 0) {
          while(1) {
               GO_nDONE = 1 ;      // PIC�ɃA�i���O�l�ǎ��J�n���w��
               while(GO_nDONE) ;   // PIC���ǎ�芮������܂ő҂�
               temp = ( ADRESH << 8 ) | ADRESL ;
               if (temp > 0) break ;
          }
          randomx = temp ;
     } else randomx = val ;   // �w�萔�l�����̂܂܏����l�Ƃ���
     ADCON0=0;  // ADC����
     WPUA0=1;   // �����v���A�b�v�L��
     ANSA0=0;   // �A�i���O���͖���
     TRISA0=0;  // �|�[�g���o�͂ɐݒ�
     srand(randomx);
}
