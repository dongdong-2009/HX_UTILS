
#include "hx_atcmd.h"
#include "hx_utils.h"
#include "string.h"
#include "stdio.h"

/*
at+write_config=622,wifi_mode=AP&power_mode=full&ap_channel=1&ap_ssid=RAK_00
0001&ap_secu_en=0......\r\n
<+++> >U< <U> ==>
at+read_config
OKpower_mode=full&wifi_mode=AP&sta_ssid=RAK415_STATION&sta_secu_en=1&sta_psk=1234567890&sta_dhcp_en=1&sta_ipaddr=192.168.7.1&sta_netmask=255.255.255.0&sta_gatew
ay=192.168.7.1&sta_dnssever1=0.0.0.0&sta_dnssever2=0.0.0.0&ap_channel=9&ap_ssid=RAK415_AP&ap_secu_en=0&ap_ipaddr=192.168.7.1&ap_netmask=255.255.255.0&uart_baudr
ate=115200&uart_datalen=8&uart_parity_en=0&uart_stoplen=1&uart_rtscts_en=0&uart_recvlenout=500&uart_timeout=20&socket_multi_en=0&socketA_type=ltcp&socketA_local
port=25000&socketA_tcp_timeout=0&user_name=admin&user_password=admin&module_name=RAK415&module_group=RAK415

client mode 
at+read_config
OKpower_mode=full&wifi_mode=STA&sta_ssid=whyyy&sta_secu_en=1&sta_psk=yy123456789&sta_dhcp_en=1&sta_ipaddr=192.168.7.1&sta_netmask=255.255.255.0&sta_gateway=192.
168.7.1&sta_dnssever1=0.0.0.0&sta_dnssever2=0.0.0.0&ap_channel=9&ap_ssid=RAK415_AP&ap_secu_en=0&ap_ipaddr=192.168.7.1&ap_netmask=255.255.255.0&uart_baudrate=115
200&uart_datalen=8&uart_parity_en=0&uart_stoplen=1&uart_rtscts_en=0&uart_recvlenout=500&uart_timeout=20&socket_multi_en=0&socketA_type=tcp&socketA_destport=9020
&socketA_localport=25000&socketA_destip=180.89.58.27&socketA_tcp_timeout=0&user_name=admin&user_password=admin&module_name=RAK415&module_group=RAK415

at+rssi
ERROR-2
RSSI=-47dB

*/
static int rak415_leave_cmdmode(void)
{
	int res;
	char buff[100];
	hx_trys(i,5){
		atc_rxclr();
		atc_printf("at+easy_txrx\r\n");
		hx_trys(j,3){
			res = atc_gets_timeout(buff,100,500);
			if(res>0 && strcmp(buff,"OK")==0){
				return 0;
			}
		}
	}
	return -1;
}
static int rak415_enter_cmdmode(void)
{
	int res;
	char buff[100];
	
	hx_trys(i,2){
		atc_rxclr();
		hx_trys(j,2){
			
			atc_put("+++");
			int c = 0;
			res = atc_getc_timeout(&c,200);
			if(res == 0){
				if(c=='U'){
					break;
				}
			}
		}
		hx_delay_ms(500);	//<3S
		atc_putc('U');
		res = atc_gets_timeout(buff,100,2500);
		if(res>0 && strstr(buff,"OK")){
			return 0;
		}
		hx_delay_ms(500);
	}
	return -1;
}

// normal return <0 xdB , >=0 is not connect
int rak415_rssi(void)
{
	int res = rak415_enter_cmdmode();
	if(res)
		return res;
	
	int ret = 1;
	char buff[50];
	atc_rxclr();
	atc_puts("at+rssi");
	hx_trys(i,3){
		do{
			res = atc_gets_timeout(buff,50,500);
			if(res>0){
				if(strstr(buff,"OK")){
					continue;
				}else if(strstr(buff,"ERROR")){
					ret = 0;
					goto end;
				}else{
					sscanf(buff,"RSSI=%ddB",&res);
					ret = res;
					goto end;
				}
			}
		}while(res);
	}
end:
	res = rak415_leave_cmdmode();
	if(res){
		hx_delay_ms(200);
		rak415_leave_cmdmode();
	}
	return ret;
}
int rak415_reset(void)
{
	int res = rak415_enter_cmdmode();
	if(res)
		return res;
	
	res = -1;
	char buff[50];
	atc_rxclr();
	atc_puts("at+reset");
	hx_trys(i,3){
		res = atc_gets_timeout(buff,50,1000);
		if(res>0){
			if(strstr(buff,"OK")){
				return 0;
			}
		}
	}
	return -1;
}

static int rak415_write_config(const char *params)
{
	int res;
	char buff[100];
	hx_trys(i,5){
		atc_rxclr();
		atc_printf("at+write_config=%d,%s\r\n",(int)(strlen(params)),params);
		hx_trys(j,3){
			res = atc_gets_timeout(buff,100,500);
			if(res>0 ){
				if(strcmp(buff,"OK")==0){
					return 0;
				}
			}
		}
	} 
	return -1;
}

static const char *default_params = 
		"power_mode=full"
		"&wifi_mode=STA"
		"&sta_ssid=whyyy"
		"&sta_secu_en=1"
		"&sta_psk=yy123456789"
		"&sta_dhcp_en=1"
	
//		"&sta_ipaddr=192.168.7.1&"
//		"sta_netmask=255.255.255.0"
//		"&sta_gateway=192.168.7.1"
//		"&sta_dnssever1=0.0.0.0"
//		"&sta_dnssever2=0.0.0.0"
	
//		"&ap_channel=9"
//		"&ap_ssid=RAK415_AP"
//		"&ap_secu_en=0"
//		"&ap_ipaddr=192.168.7.1"
//		"&ap_netmask=255.255.255.0"
	
//		"&uart_baudrate=115200"
//		"&uart_datalen=8"
//		"&uart_parity_en=0"
//		"&uart_stoplen=1"
//		"&uart_rtscts_en=0"
//		"&uart_recvlenout=500"
//		"&uart_timeout=20"
		"&socket_multi_en=0"
		"&socketA_type=tcp"
		"&socketA_destport=9020"
		"&socketA_localport=25000"
		"&socketA_destip=180.89.58.27"
		"&socketA_tcp_timeout=0"
//		"&user_name=admin"
//		"&user_password=admin"
//		"&module_name=RAK415"
//		"&module_group=RAK415"
		;

//if param_file set NULL,that use deault.
int rak415_init(const char *param_file)
{
	int res;
	char buff[100];

	res = rak415_enter_cmdmode();
	if(res){	
		atc_printf("\r\n");
		hx_delay_ms(200);
		atc_printf("at+easy_txrx\r\n");
		hx_delay_ms(3000);
		return res;
	}
	hx_trys(i,5){
		atc_printf("at+ascii=1\r\n");
		hx_trys(j,3){
			res = atc_gets_timeout(buff,100,500);
			if(res>0 && strcmp(buff,"OK")==0){
				goto next2;
			}
		}
	}
	return -2;
next2:
		
	//res = rak415_write_config(default_params);
	//if(res)
	//	return -5;
	res = rak415_write_config(param_file);
	if(res)
		return -6;
	

	res = rak415_leave_cmdmode();
	if(res){
		hx_delay_ms(200);
		rak415_leave_cmdmode();
	}
	
	return 0;
}

