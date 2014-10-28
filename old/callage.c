#include "main.h"
#include "motors.h"
#include "message.h"
#include "callage.h"
#include "debug.h"



 void callage()
{
	int i =0;
	long int lastD, lastG, D,G;
	AD_motors_set_speed(1000,1000);
	motors_get_qei(&G, &D);
	pause_ms(50);
	
	//On regarde si les codeurs sont arretés
	while(1)
	{
			lastD = D;
			lastG = G;
			motors_get_qei(&G, &D);
			
			if((D-lastD) == 0 && (G-lastG)==0)
				break;
				
			pause_ms(50);
			
			i=80;
	}	
	
	AD_motors_set_speed(-1000,-1000);
	pause_s(1);
	
	
	
	AD_motors_set_speed(-1000,1000);
	
	pause_ms(1500);
	
	AD_motors_set_speed(1000,1000);
	motors_get_qei(&G, &D);
	pause_ms(50);
	
	
	while(1)
	{
			lastD = D;
			lastG = G;
			motors_get_qei(&G, &D);
			
			if((D-lastD) == 0 && (G-lastG)==0)
				break;
				
			pause_ms(50);
			
			i=80;
	}	
	
	AD_motors_set_speed(0,0);
 	i = 100;
 	
 	while(1)
 	{
	 	i=100;	
	 }	
}  