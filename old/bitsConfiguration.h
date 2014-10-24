/* 
 * File:   bitsConfiguration.h
 * Author: Dan
 *
 * Created on 21 mars 2013, 13:40
 */

#ifndef BITSCONFIGURATION_H
#define	BITSCONFIGURATION_H


// DSPIC33FJ128MC802 Configuration Bit Settings



 int FBS __attribute__((space(prog), address(0xF80000))) = 0xCF ;
//_FBS(
//    BWRP_WRPROTECT_OFF & // Boot Segment Write Protect (Boot Segment may be written)
//    BSS_NO_FLASH &       // Boot Segment Program Flash Code Protection (No Boot program Flash segment)
//    RBS_NO_RAM           // Boot Segment RAM Protection (No Boot RAM)
//);
 int FSS __attribute__((space(prog), address(0xF80002))) = 0xCF ;
//_FSS(
//    SWRP_WRPROTECT_OFF & // Secure Segment Program Write Protect (Secure segment may be written)
//    SSS_NO_FLASH &       // Secure Segment Program Flash Code Protection (No Secure Segment)
//    RSS_NO_RAM           // Secure Segment Data RAM Protection (No Secure RAM)
//);
 int FGS __attribute__((space(prog), address(0xF80004))) = 0x7 ;
//_FGS(
//    GWRP_OFF &           // General Code Segment Write Protect (User program memory is not write-protected)
//    GSS_OFF              // General Segment Code Protection (User program memory is not code-protected)
//);
 int FOSCSEL __attribute__((space(prog), address(0xF80006))) = 0x81 ;
//_FOSCSEL(
//    FNOSC_FRCPLL &       // Oscillator Mode (Internal Fast RC (FRC) w/ PLL)
//    IESO_ON              // Internal External Switch Over Mode (Start-up device with FRC, then automatically switch to user-selected oscillator source when ready)
//);
 int FOSC __attribute__((space(prog), address(0xF80008))) = 0xE7 ;
//_FOSC(
//    POSCMD_NONE &        // Primary Oscillator Source (Primary Oscillator Disabled)
//    OSCIOFNC_OFF &       // OSC2 Pin Function (OSC2 pin has clock out function)
//    IOL1WAY_ON &         // Peripheral Pin Select Configuration (Allow Only One Re-configuration)
//    FCKSM_CSDCMD         // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)
//);
 //int FWDT __attribute__((space(prog), address(0xF8000A))) = 0xFF7F ;
_FWDT(
    WDTPOST_PS32768 &    // Watchdog Timer Postscaler (1:32,768)
    WDTPRE_PR128 &       // WDT Prescaler (1:128)
    WINDIS_OFF &         // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
    FWDTEN_OFF           // Watchdog Timer Enable (Watchdog timer enabled/disabled by user software)
);
 int FPOR __attribute__((space(prog), address(0xF8000C))) = 0xF7 ;
//_FPOR(
//    FPWRT_PWR128 &       // POR Timer Value (128ms)
//    ALTI2C_OFF &         // Alternate I2C  pins (I2C mapped to SDA1/SCL1 pins)
//    LPOL_ON &            // Motor Control PWM Low Side Polarity bit (PWM module low side output pins have active-high output polarity)
//    HPOL_ON &            // Motor Control PWM High Side Polarity bit (PWM module high side output pins have active-high output polarity)
//    PWMPIN_ON            // Motor Control PWM Module Pin Mode bit (PWM module pins controlled by PORT register at device Reset)
//);
 int FICD __attribute__((space(prog), address(0xF8000E))) = 0xFFDF ;
//_FICD(
//    ICS_PGD1 &           // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
//    JTAGEN_OFF           // JTAG Port Enable (JTAG is Disabled)
//);


#endif	/* BITSCONFIGURATION_H */

