/*******************************************************************************

    This file is part of the up2p.
    Copyright wilddog.com
    All right reserved.

    File:    protocol_uart.c

    No description

    TIME LIST:
    CREATE  skyli    

*******************************************************************************/

#include "protocol_uart.h"
#include <stdio.h>
#include <stdlib.h>
/* todo
* 从队列中取出数据，并解析
**/
int uart_parse(u8 *p_pack,const u16 len){
	UART_PACKET *p_rec = NULL;
	if(len < sizeof(UART_PACKET)){
		// no match
		return -1;
	}
	p_rec = (UART_PACKET*)p_pack;
	if( p_rec->version != UART_VERSION){
		// no match dump the package
		return -1;
	}
	return uart_cmd_handl(p_pack);
}
/* todo
* 底层接收
*/
int _uart_recv(u8 *p_rec,u16 len){
	//todo put packet to queue
	uart_parse(p_rec,len);
}
int uart_pack_alloc(u8 **pp_packet,u16 cmd,const u8 *p_data,const u16 len){

	
	UART_PACKET *p_packet = malloc(sizeof(UART_PACKET)+len);
	if(!p_packet)
		return -1;
	//protocol packet
	p_packet->version = UART_VERSION;
	p_packet->cmd = cmd;
	p_packet->len = len;

	
	if(!p_data | len ){
		memcpy(p_packet->payload,p_data,len);

		
	}
	
	*pp_packet = p_packet;

	
	return (sizeof(UART_PACKET)+len);
}

// 响应串口数据
int uart_cmd_handl(u8 *p_rec){
	UART_PACKET *p_pack = (UART_PACKET*)p_rec;

	switch(p_pack->cmd){
		case CMD_SOCKET_ON:

			break;
		case CMD_SOKET_OFF:

			break;
		case CMD_LIGHT_UP:
		{
			UART_Light_T *p_light = (UART_Light_T*)p_pack->payload;
			printf("change %d light to %d \n",p_light->no,p_light->intensity);
		}
		default: 
			break;
	}
	return 0;
}
void main(void)
{
	UART_Light_T test_light;
	u8 *p_send = NULL;
	test_light.no = 10;
	test_light.intensity = 99;
	
	int slen = uart_pack_alloc(&p_send,CMD_SOCKET_ON,&test_light,sizeof(UART_Light_T));

	if(!slen | !p_send )
		return -1;

	_uart_recv(p_send,slen);

	
	free(p_send);
	p_send = NULL;
	

}