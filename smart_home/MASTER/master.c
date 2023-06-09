﻿/*
 * AVRGCC1.c
 *
 * Created: 24/05/2022 02:17:15 م
 *  Author: MIK-PC
 */ 

#include <avr/io.h>
#include "keypad.h"
#include "lcd.h"
#include "EEPROM.h"
#include "STD_MESSAGES.h"
#include "smart_home_cofig.h"
#include "Master_MainFun.h"

struct_modecnf OWNER_MODE_STRUCT={"OWNER",
								  OWNER_MODE,
								  ADMIN_PASS_ADD,
								  OWNER_LED_PORT,
								  OWNER_PIN
							    };
								
struct_modecnf GUEST_MODE_STRUCT={"GUEST",
								  GUEST_MODE,
								  Guest_PASS_ADD,
								  GUEST_LED_PORT,
								  GUEST_PIN
							    };		
uint8 pass[4];
extern uint8 pass_count;
extern volatile char key_pressed;
uint8 Mode = NO_MODE;
uint8 SPI_RESPONSE;

int main(void)
{
	
	LCD_INISTIALIZATION();
	SPI_vInitMaster();//initializes the communication protocol of SPI
while (1)
{
	SEND_COMND(0x01); //clear lcd and curser goto loc(1.1) automatically
	GO_LOC(2,2);
	SEND_STRING("wellcome to smart");
	GO_LOC(3,5);
	SEND_STRING("home system");
	_delay_ms(1000);
while (1)
{
	if (EEPROM_ui8ReadByteFromAddress(Login_status)==0xFF)
	{		
		vEnterFirstTime();
	} 
	while (Mode==NO_MODE)
	{
	SEND_COMND(0x01); //clear lcd and curser goto loc(1.1) automatically
	GO_LOC(2,5);
	SEND_STRING("select mode");
	GO_LOC(3,3);
	SEND_STRING("0:Owner 1:Guest");
	_delay_ms(2000);				
	key_pressed=keyfind();
	if (key_pressed=='0')
	{
		SelectModePass(&OWNER_MODE_STRUCT);
	} 
	else if (key_pressed=='1')
	{
		SelectModePass(&GUEST_MODE_STRUCT);
	}
	}			
	SEND_COMND(0x01); //clear lcd and curser goto loc(1.1) automatically
	GO_LOC(1,3);	
	SEND_STRING("1:ROOM1 2:ROOM2");
	GO_LOC(2,3);
	SEND_STRING("3:ROOM3 ");	
	
	if (Mode==OWNER_MODE)
	{
			SEND_STRING("4:TV");		
			GO_LOC(3,3);
			SEND_STRING("5:AIR_Condition");
	}
	if (Mode==OWNER_MODE)
	{
		GO_LOC(4,4);
	} 
	else
	{
		GO_LOC(3,4);
	}

	SEND_STRING("EXIT PRESS:* ");
		
	key_pressed=keyfind();
	if (key_pressed=='*')
	{
		Mode=NO_MODE;
		PORTC&=(~(1<<OWNER_PIN));
		PORTC&=(~(1<<GUEST_PIN));
		break;
	}
	else
	{
	u8EnterRoonConfig(key_pressed);		
	}										
}
	}			
}