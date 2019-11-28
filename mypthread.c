#include "include.h"
#include "mac_test.h"
/*线程发送至串口函数*/
void pthread_u_send(char* str)
{
	cJSON* root=NULL;
	root = cJSON_Parse(str);//需要释放（1）
	if( strcmp(root->child->string,"heart_jump") ==0 )//heart jump
	{
		if(!identify_flag)
		{
			char *mac = malloc(13);
			memset(mac,0,13);
			get_mac(mac);
			char *str_json = malloc(256);
			memset(str_json,0,256);
			int sn_fd = open("/root/sn.txt",O_RDWR|O_CREAT,0777);
			memset(gw_sn,0,100);
			int sn_fd_len = read(sn_fd,gw_sn,100);
			close(sn_fd);
			if(json_checker(gw_sn)==0 && sn_fd_len >0)
			{
				cJSON *sn_root = cJSON_Parse(gw_sn);
				cJSON *this_gw_sn = cJSON_GetObjectItem(sn_root,"sn");
				create_secret_str(str_json,this_gw_sn->valuestring,mac);
				send(cd,str_json,strlen(str_json),0);
				cJSON_Delete(sn_root);
				sn_root=NULL;
			}
			else 
			{
				sn_pwd_str(str_json,mac);
				send(cd,str_json,strlen(str_json),0);
			}
			free(mac);
			mac=NULL;
			free(str_json);
			str_json=NULL;
		}
	}
	else
	{
		cJSON *while_root = root->child;
		while(while_root != NULL)
		{
			if(strcmp(while_root->string,"api")==0)
			{
				cJSON *api = NULL;
				api=cJSON_GetObjectItem(root,"api");
				
				if( strcmp("devCmd",api->valuestring) == 0 )//common device control
				{
					dev_com_con(root);
				}
				else if( strcmp("roomDevCmd",api->valuestring) == 0 )//room open all or close all
				{
					pthread_mutex_lock(&mutex_v);
					dev_room_con(root);
					pthread_mutex_unlock(&mutex_v);
				}
				else if( strcmp("secuDevCmd",api->valuestring) == 0 )//secure device control
				{
					dev_sec_con(root);
				}
				else if( strcmp("switchSecuModel",api->valuestring) == 0 )//switch secure model
				{
					switchSecuModel(root);
				}
				else if( strcmp("gw_getScenBas",api->valuestring) == 0 )//get scene list
				{
					cJSON *server_ret = cJSON_GetObjectItem(root,"retcode");
					if(server_ret->valueint==0)
					down_scene_list(str);
				}
				else if( strcmp("gw_getScenList_mx",api->valuestring) == 0 )//get scene mx
				{
					cJSON *server_ret = cJSON_GetObjectItem(root,"retcode");
					if(server_ret->valueint==0)
					down_scene_mx(str);
				}
				else if( strcmp("gw_get_devlist",api->valuestring) == 0 )//get dev list
				{
					cJSON *server_ret = cJSON_GetObjectItem(root,"retcode");
					if(server_ret->valueint==0)
					down_dev_list(str);
				}
				else if( strcmp("gw_getBindList_mx",api->valuestring) == 0 )//get multi_bind list
				{
					cJSON *server_ret = cJSON_GetObjectItem(root,"retcode");
					if(server_ret->valueint==0)
					down_bind_list(str);
				}
				else if( strcmp("gw_getTimerList",api->valuestring) == 0 )//get timer list
				{
					cJSON *server_ret = cJSON_GetObjectItem(root,"retcode");
					if(server_ret->valueint==0)
					down_timer_list(str);
				}
				else if( strcmp("gw_getSecuModelSet",api->valuestring) == 0 )//get secure list
				{
					cJSON *server_ret = cJSON_GetObjectItem(root,"retcode");
					if(server_ret->valueint==0)
					down_sec_list(str);
				}
				else if( strcmp("gw_get_voi_conf",api->valuestring) == 0 )//get voice list
				{
					cJSON *server_ret = cJSON_GetObjectItem(root,"retcode");
					if(server_ret->valueint==0)
					down_voice_list(str);
				}
				else if( strcmp("gw_get_room",api->valuestring) == 0 )//get room list
				{
					cJSON *server_ret = cJSON_GetObjectItem(root,"retcode");
					if(server_ret->valueint==0)
					down_room_list(str);
				}
				else if( strcmp("gw_identify",api->valuestring) == 0 )//get identify
				{
					get_identify(root);
				}
				else if( strcmp("dev_net_cmd",api->valuestring) == 0 )//go to net or forbid to net
				{
					let_go_to_net(root);
				}
				else if( strcmp("get_device_signal",api->valuestring) == 0 )//get sigal
				{
					get_signal();
				}
				else if( strcmp("delete_dev_notice",api->valuestring) == 0 )//delete dev
				{
					up_dev_describe(root);
				}
				else if( strcmp("execute_scen",api->valuestring) == 0 )//run scene
				{
					cJSON *scene_data = cJSON_GetObjectItem(root,"data");
					cJSON *scene_id = cJSON_GetObjectItem(scene_data,"scen_id");
					run_scene(scene_id->valuestring);
				}
				else if( strcmp("voice_cmd",api->valuestring) == 0 )//app 语音控制
				{
					voice_con(root);
				}
				else if( strcmp("get_hw_code",api->valuestring) == 0 )//get hw code
				{
					get_hw_code_ser(root);
				}
				else if( strcmp("notice_up_gw",api->valuestring) == 0 )//Remote upgrade
				{
					if(!down_gw_flag)
					{
						down_gw_flag=1;
						down_file(root);
						down_gw_flag=0;
					}
				}
				else if( strcmp("cmd_study_hw",api->valuestring) == 0 )//红外学习命令
				{
					hw_study_or_complish_con(root,1);
				}
				else if( strcmp("one_key_code",api->valuestring) == 0 )//红外一键对码
				{
					hw_study_or_complish_con(root,0);
				}
				else if( strcmp("hw_dev_cmd",api->valuestring) == 0 )//红外控制
				{
					dev_hw_con(root);
				}
				else if( strcmp("notice_update_lid",api->valuestring) == 0 )//红外code更新
				{
					up_hw_code(root);
				}
				else if( strcmp("get_lid_ver",api->valuestring) == 0 )//红外code_ve更新
				{
					get_new_lid_ver(root);
				}
				else if( strcmp("gw_get_sn",api->valuestring) == 0 )//获取sn
				{
					get_sn_from_server(root);
				}
				else if( strcmp("gw_notice_sn",api->valuestring) == 0 )//sn反馈
				{
					get_sn_feedback(root);
				}
				else if( strcmp("gw_upDevState",api->valuestring) == 0 )
				{
					down_dev_state_list(str);
				}
				else if( strcmp("set_dev_timer",api->valuestring) == 0 )
				{
					dev_set_time(root,str);
				}
				break;
			}
			while_root = while_root->next;
		}
	}
	cJSON_Delete(root);
	root=NULL;
	free(str);
	str=NULL;
	pthread_exit((void *)0);
}

/*线程发送至服务器函数*/
void pthread_v_send(uint8_t *u_data)
{
	int u_data_len=u_data[13]*256+u_data[14]+15;
	int i = 0;
	char *u_data_str = NULL;
	u_data_str = (char*)malloc(u_data_len*2+1);
	memset(u_data_str,0,u_data_len*2+1);
	hex_to_str(u_data,u_data_str,u_data_len);
	
	if(u_data[10] == 0x45 || u_data[10] == 0x46)//设备控制
	{
		dev_com_con_u(i,u_data_len,u_data,u_data_str);
	}
	else if(u_data[10]== 0x47)//离线语音控制
	{
		if(u_data[19] != 0x01 && u_data[19] != 0x02)
			voice_com_con_u(i,u_data,u_data_str);
	}
	else if(u_data[10] == 0x10)//设备上线，入网
	{
		if(go_net_flag==1)
			dev_online(i,u_data_len,u_data_str);
	}
	else if(u_data[10]==0x15)//设备状态
	{
		dev_status_u(u_data,i,u_data_len,u_data_str);
	}
	else if(u_data[10] == 0x40)//设备描述
	{
		if(go_net_flag==1 && NET_FLAG)
			dev_descrip_u(i,u_data_len,u_data_str);
	}
	else if(u_data[10] == 0x50)//kt控制
	{
		dev_com_kt_con(i,u_data_len,u_data,u_data_str);
	}
	else if(u_data[10] == 0x52)//学习or匹配
	{
		hw_study_or_one_key(u_data_len,u_data,u_data_str);
	}
	else if(u_data[10] ==0x20 || u_data[10] ==0x21)//设备允许入网
	{
		dev_into_net(u_data);
	}
	else if(u_data[10]==0x81)
	{
		dev_sensor_81(i,u_data_len,u_data_str);
	}
	else if(u_data[10]==0x82)
	{
		dev_sensor_82(u_data,i,u_data_len,u_data_str);
	}
	else if(u_data[10]==0x60)
	{
		get_signal_u(u_data_str);
	}
	else if(u_data[10]==0x65)
	{
		power_65(u_data,i,u_data_len,u_data_str);
	}
	else if(u_data[10]==0x90)
	{
		dev_lock_con(i,u_data_len,u_data,u_data_str);
	}
	else if(u_data[10] == 0x30)
	{
		memcpy(zigbee_channel,u_data_str+30,2);
		zigbee_channel[2] = '\0';
		memcpy(zigbee_id,u_data_str+32,4);
		zigbee_id[4] = '\0';
	}
	free (u_data_str);
	u_data_str=NULL;
	free(u_data);
	u_data=NULL;
	pthread_exit((void *)0);
}
/*线程串口接收函数*/
void pthread_usart_receive(void)
{
	int i,len,arr_len=0,data_len,flag_break;
	uint8_t r_buff[1024];//串口接收缓冲区
	uint8_t rc_buff[10240];//中间转换缓冲区
	
	memset(r_buff,0,1024);
	memset(rc_buff,0,10240);
	
	while(1)
	{
		len = read(fd,r_buff,sizeof(r_buff));

		if(len>0)
		{
			for(i=0;i<len;i++)
			{
				rc_buff[arr_len]=r_buff[i];
				arr_len+=1;
			}
			memset(r_buff,0,1024);
		}
		while(1)
		{
			if(arr_len > 15)
			{
				flag_break = 0;
				for(i=0;i<arr_len;i++)
				{	
					if((rc_buff[i]==0x5A)&&(rc_buff[i+1]==0xA5))
					{
						if(i>0)
						{
							delete_len_from_arr(rc_buff,i,&arr_len);
						}
						if(arr_len > 15)
						{
							data_len = rc_buff[13]+rc_buff[14]+15;
							
							if(arr_len >= data_len)
							{
								uint8_t *my_u_data = NULL;
								my_u_data =(uint8_t*)malloc(data_len);//解析数据接收缓冲区
								memset(my_u_data,0,data_len);

								for(i=0;i<data_len;i++)
									my_u_data[i]=rc_buff[i];
/*
								printf("usart receive is:");
								for(i=0;i<data_len;i++)
								{
									my_u_data[i]=rc_buff[i];
									printf("%.2x  ",my_u_data[i]);
								}
								printf("\n");
*/
								up_resend(my_u_data);//更新重发列表
								
								pth_creat_my(pthread_v_send,my_u_data);
								
								delete_len_from_arr(rc_buff,data_len,&arr_len);
							}
							else
								flag_break = 1;
						}
						break;
					}
					else if(i==arr_len-1)
						flag_break = 1;
				}
				if(flag_break) break;
			}
			else
				break;
		}
	}
}

/*线程接收服务器函数*/
void pthread_client_receive(void)
{
	int flag=0,ret_recv,str_len=0,i,num=0;
	char c_send[8192];//客户端数据缓冲区
	memset(str_from_server,0,819200);
	memset(c_send,0,8192);
	signal(6,reconnect);
	while(1)
	{
		ret_recv=recv(cd,c_send,8192,0);

		if(ret_recv>0)
		{	
			for(i=0;i<ret_recv;i++)
			{
				str_from_server[str_len]=c_send[i];
				str_len+=1;
			}
			memset(c_send,0,sizeof(c_send));
		}
		else if(ret_recv == 0)
		{
			num++;
			if(num>5)
			{
				printf("server is gone\n");
				kill_gateway();
			}

		}
		else
		{
			sleep(1);
			continue;
		}
		while(1)
		{

			if(str_len>15)
			{

				flag=0;

				for(i=14;i<str_len;i++)
				{
					if(str_from_server[i]=='\n')
						{
							flag=i;
							break;
						}	
				}

				if(flag>=15)
				{
					char *r_str = NULL;
					r_str=(char *)malloc(flag+1);//解析数据接收缓冲区

					memset(r_str,0,flag+1);

					memcpy(r_str,str_from_server,flag+1);
					r_str[flag]='\0';
					replace_character(r_str);
					
					if(json_checker(r_str)==0)
					{
						first = time(NULL);
						pth_creat_my(pthread_u_send,r_str);
					}
					else
					{
						free(r_str);
						r_str=NULL;
					}
					
					delete_len_from_str(str_from_server,flag+1,&str_len);
				}
				else if(flag==14)
				{
					delete_len_from_str(str_from_server,flag+1,&str_len);
				}
				else
					break;
			}
			else
			{
				break;
			}
		}
	}
}
/*重连函数*/
void reconnect(int num)
{
	shutdown(cd,SHUT_RDWR);
	close(cd);
	client_init();
}
/*心跳机制*/
void heart_jump(void)
{
	int time_diff=0;
	first=time(NULL);
	second=time(NULL);
	while(1)
	{
		sleep(1);
		second=time(NULL);
		if( (abs((unsigned int)difftime(second,first))-time_diff) > 2)
		{
			first = time(NULL);
		}
		time_diff = abs((unsigned int)difftime(second,first));
		if(time_diff > 30)
		{
			if(identify_flag==1) 
			{
				printf("first but not heart_jump\n");
				kill_gateway();
			}
			first=second=time(NULL);
			pthread_kill(id_client,6);
		}
	}
}
void re_send_success_or_failed(RSD *p,char *dev_id,char *dev_type,char *data_str,int ret_num,char *message)
{
	cJSON *root = cJSON_CreateObject();//创建项目
	cJSON *data_object = cJSON_CreateObject();
	cJSON *cmd_dev = cJSON_CreateObject();
	cJSON_AddNumberToObject(root,"retcode",ret_num);
	cJSON_AddStringToObject(root,"message",message);
	cJSON_AddStringToObject(root,"api","devStateRes");
	cJSON_AddItemToObject(root,"data",data_object);
	cJSON_AddStringToObject(data_object,"dev_id",dev_id);
	cJSON_AddStringToObject(data_object,"dev_type",dev_type);
	if(p->cmd[10] == 0x45 && p->cmd[12] == 0)
		cJSON_AddStringToObject(data_object,"control_type","1");
	else if(p->cmd[10] == 0x45 && p->cmd[12] == 0x47)
		cJSON_AddStringToObject(data_object,"control_type","3");
	else if(p->cmd[10] == 0x45 && p->cmd[12] == 0x45)
		cJSON_AddStringToObject(data_object,"control_type","4");
	cJSON_AddStringToObject(cmd_dev,"dev_state",data_str);
	cJSON_AddItemToObject(data_object,"dev_state",cmd_dev);
	char *send_char = cJSON_PrintUnformatted(root);
	int my_len = strlen(send_char);
	char *my_send_char = (char *)malloc(my_len+2);
	memset(my_send_char,0,my_len+2);
	memcpy(my_send_char,send_char,my_len);
	strcat(my_send_char,"\n\0");
	send(cd,my_send_char,my_len+1,0);
	free(send_char);
	send_char = NULL;
	free(my_send_char);
	my_send_char = NULL;
	cJSON_Delete(root);
	root = NULL;
}
/************************************重发*************************************************/
void re_send(void)
{
	RSD *p = NULL;
	struct timeval time_val;
	int time_diff;
	time_t resend_time;
	while(1)
	{
		p = resend_head;
		gettimeofday(&time_val,NULL);
		resend_time = time_val.tv_sec*1000+time_val.tv_usec/1000;//获取大循环进来时的时间
		if(p!=NULL)
		{
			while(p!=NULL)
			{
				pthread_mutex_lock(&mutex_resend);
				time_diff = resend_time - p->now_time;//获取大循环和小循环的时间差
				if( time_diff > 2000 && p->now_times > 0)
				{
					p->now_times++;
					if(p->now_times>=6)//更新发送的发送次数，表明失败放弃。
					{
						p->now_times=0;
						if( NET_FLAG && p->cmd[10] != 0x60 )
						{
							pthread_mutex_lock(&mutex_zl);
							cJSON *device_state_list_data = cJSON_Parse(device_state_list);//遍历设备状态列表
							pthread_mutex_unlock(&mutex_zl);
							if(device_state_list_data != NULL)
							{
								if(device_state_list_data->child != NULL)
								{
									cJSON *my_device_state_list_data = cJSON_GetObjectItem(device_state_list_data,"data");
									cJSON *while_device_state_list_data = my_device_state_list_data->child;
									if(while_device_state_list_data==NULL)
									{
										int cmd_len_failed = p->cmd[13]*256+p->cmd[14];
										char *data_str = (char *)malloc(2*cmd_len_failed+1);
										memset(data_str,0,2*cmd_len_failed+1);
										hex_to_str(p->cmd+15,data_str,cmd_len_failed);
										data_str[2*cmd_len_failed] = '\0';
										re_send_success_or_failed(p,p->dev_id,p->dev_type,data_str,-1,"fail");
										free(data_str);
										data_str = NULL;
									}
									else
									{
										while(while_device_state_list_data != NULL)
										{
											if(strcmp(while_device_state_list_data->string,p->dev_id)==0)
											{
												cJSON *dev_state_cjson = cJSON_GetObjectItem(my_device_state_list_data,p->dev_id);
												int flag_have = 0,num = 0,j = 0;
												num = cJSON_GetArraySize(dev_state_cjson);
												cJSON *arr_for=NULL;
												for(j=0;j<num;j++)
												{
													arr_for = cJSON_GetArrayItem(dev_state_cjson,j);
													if(!strcmp(arr_for->string,"dev_state"))
													{
														flag_have=1;
														break;
													}
												}
												if(flag_have==1)
												{
													cJSON *my_dev_state = cJSON_GetObjectItem(dev_state_cjson,"dev_state");
													re_send_success_or_failed(p,p->dev_id,p->dev_type,my_dev_state->valuestring,0,"success");
												}
												else
												{
													int cmd_len_failed = p->cmd[13]*256+p->cmd[14];
													char *data_str = (char *)malloc(2*cmd_len_failed+1);
													memset(data_str,0,2*cmd_len_failed+1);
													hex_to_str(p->cmd+15,data_str,cmd_len_failed);
													data_str[2*cmd_len_failed] = '\0';
													re_send_success_or_failed(p,p->dev_id,p->dev_type,data_str,-1,"fail");
													free(data_str);
													data_str = NULL;
												}
												break;
											}
											while_device_state_list_data = while_device_state_list_data->next;
											if(while_device_state_list_data==NULL)
											{
												int cmd_len_failed = p->cmd[13]*256+p->cmd[14];
												char *data_str = (char *)malloc(2*cmd_len_failed+1);
												memset(data_str,0,2*cmd_len_failed+1);
												hex_to_str(p->cmd+15,data_str,cmd_len_failed);
												data_str[2*cmd_len_failed] = '\0';
												re_send_success_or_failed(p,p->dev_id,p->dev_type,data_str,-1,"fail");
												free(data_str);
												data_str = NULL;
											}
										}
									}
								}
							}
							cJSON_Delete(device_state_list_data);
							device_state_list_data = NULL;
						}
						pthread_mutex_unlock(&mutex_resend);
					}
					else
					{
						int cmd_len = p->cmd[13]+p->cmd[14]+15;
						gettimeofday(&time_val,NULL);
						p->now_time = time_val.tv_sec*1000+time_val.tv_usec/1000;
						usart_send(fd, p->cmd,cmd_len);
						pthread_mutex_unlock(&mutex_resend);
						usleep(200000);
					}
				}
				else
					pthread_mutex_unlock(&mutex_resend);
				p = p->next;
			}
		}
		sleep(1);//每次的大循环延时1s
	}
}
/*重发增加到重发链表*/
void resend_zt(int len,uint8_t *cmd,char *dev_id,char *dev_type)
{
	RSD *p = NULL;
	struct timeval time_val;
	p = resend_head;
	pthread_mutex_lock(&mutex_resend);
	if(p==NULL)
	{
		resend_d = (RSD*)malloc(sizeof(RSD));
		memset(resend_d,0,sizeof(RSD));
		gettimeofday(&time_val,NULL);
		resend_d->now_time = time_val.tv_sec*1000+time_val.tv_usec/1000;//记录时间
		resend_d->now_times = 1;
		memcpy(resend_d->cmd,cmd,len);
		memcpy(resend_d->dev_id,dev_id,strlen(dev_id)+1);
		memcpy(resend_d->dev_type,dev_type,strlen(dev_type)+1);
		resend_head = resend_z = resend_d;
		resend_d->next = NULL;
	}
	else
	{
		while( p )
		{
		
			if(!mac_and_port_judge(p,cmd))
			{
				p->now_times = 1;//记录当前的发送次数
				gettimeofday(&time_val,NULL);
				p->now_time = time_val.tv_sec*1000+time_val.tv_usec/1000;//记录时间
				memset(p->cmd,0,64);
				memcpy(p->cmd,cmd,len);
				memset(p->dev_id,0,20);
				memcpy(p->dev_id,dev_id,strlen(dev_id)+1);
				memset(p->dev_type,0,10);
				memcpy(p->dev_type,dev_type,strlen(dev_type)+1);
				break;
			}
			else if(p->next == NULL)
			{
				resend_d = (RSD*)malloc(sizeof(RSD));
				memset(resend_d,0,sizeof(RSD));
				gettimeofday(&time_val,NULL);
				resend_d->now_time = time_val.tv_sec*1000+time_val.tv_usec/1000;//记录时间
				resend_d->now_times = 1;//记录当前的发送次数
				memcpy(resend_d->cmd,cmd,len);
				memcpy(resend_d->dev_id,dev_id,strlen(dev_id)+1);
				memcpy(resend_d->dev_type,dev_type,strlen(dev_type)+1);
				p->next = resend_d;
				resend_d->next = NULL;
				break;
			}
			p = p->next;
		}
	}
	pthread_mutex_unlock(&mutex_resend);
}

/************************************人体检测*************************************************/
void human_check(void)
{
	HB *p = NULL;
	time_t human_time;
	int num,m,flag_have;
	cJSON *arr_for = NULL;
	while(1)
	{
		p = human_head;
		human_time = time(NULL);
		while(p!=NULL)
		{
			pthread_mutex_lock(&mutex_human);
			if(human_time - p->now_time >= 45 && p->flag == 1)
			{
				p->flag = 0;;
				my_human_file();
				if(strcmp(p->type,"060102")==0)//
				{
					run_scene_u_triger(p->id,"00");
					cJSON *root_u = cJSON_CreateObject();//创建项目
					cJSON *data_object = cJSON_CreateObject();
					cJSON_AddNumberToObject(root_u,"retcode",0);
					cJSON_AddStringToObject(root_u,"message","success");
					cJSON_AddStringToObject(root_u,"api","devStateRes");
					cJSON_AddItemToObject(root_u,"data",data_object);
					cJSON_AddStringToObject(data_object,"dev_id",p->id);
					cJSON_AddStringToObject(data_object,"dev_type","060102");
					cJSON_AddStringToObject(data_object,"control_type","2");
					
					pthread_mutex_lock(&mutex_zl);
					cJSON *device_state_list_data = cJSON_Parse(device_state_list);//遍历设备状态列表
					if(device_state_list_data != NULL)
					{
						if(device_state_list_data->child != NULL)
						{
							cJSON *my_device_state_list_data = cJSON_GetObjectItem(device_state_list_data,"data");
							cJSON *while_device_state_list_data = my_device_state_list_data->child;
							if(while_device_state_list_data==NULL)
							{
								cJSON *my_date = cJSON_CreateObject();//创建项目
								cJSON_AddItemToObject(my_device_state_list_data,p->id,my_date);
								cJSON_AddStringToObject(my_date,"dev_state",00);
								char *device_state_list_char = cJSON_PrintUnformatted(device_state_list_data);
								memset(device_state_list,0,BUFFSIZE);
								memcpy(device_state_list,device_state_list_char,strlen(device_state_list_char));
								int state_fd = open("/root/device_state_list.txt",O_RDWR|O_CREAT|O_TRUNC,0777);
								write(state_fd,device_state_list_char,strlen(device_state_list_char));
								close(state_fd);
								free(device_state_list_char);
								device_state_list_char=NULL;
								
								cJSON *cmd_dev = cJSON_Duplicate(my_date,1);
								cJSON_AddItemToObject(data_object,"dev_state",cmd_dev);
								char *send_char = cJSON_PrintUnformatted(root_u);
								int my_len = strlen(send_char);
								char *my_send_char = (char *)malloc(my_len+2);
								memset(my_send_char,0,my_len+2);
								memcpy(my_send_char,send_char,my_len);
								strcat(my_send_char,"\n\0");
								if(NET_FLAG)
								send(cd,my_send_char,my_len+1,0);
								free(send_char);
								send_char = NULL;
								free(my_send_char);
								my_send_char=NULL;
								cJSON_Delete(root_u);
								root_u = NULL;
							}
							else
							{
								while(while_device_state_list_data != NULL)
								{
									if(strcmp(while_device_state_list_data->string,p->id)==0)
									{
										cJSON *dev_state_cjson = cJSON_GetObjectItem(my_device_state_list_data,p->id);
										num = cJSON_GetArraySize(dev_state_cjson);
										flag_have=0;
										for(m=0;m<num;m++)
										{
											arr_for = cJSON_GetArrayItem(dev_state_cjson,m);
											if(!strcmp(arr_for->string,"dev_state"))
											{
												flag_have = 1;
												break;
											}
										}
										if(flag_have == 1)
										{
											cJSON *my_dev_state = cJSON_GetObjectItem(dev_state_cjson,"dev_state");
											memset(my_dev_state->valuestring,0,strlen(my_dev_state->valuestring));
											memcpy(my_dev_state->valuestring,"00",2);
											char *device_state_list_char = cJSON_PrintUnformatted(device_state_list_data);
											memset(device_state_list,0,BUFFSIZE);
											memcpy(device_state_list,device_state_list_char,strlen(device_state_list_char));
											int state_fd = open("/root/device_state_list.txt",O_RDWR|O_CREAT|O_TRUNC,0777);
											write(state_fd,device_state_list_char,strlen(device_state_list_char));
											close(state_fd);
											free(device_state_list_char);
											device_state_list_char = NULL;
											
											cJSON *cmd_dev = cJSON_Duplicate(dev_state_cjson,1);
											cJSON_AddItemToObject(data_object,"dev_state",cmd_dev);
											char *send_char = cJSON_PrintUnformatted(root_u);
											int my_len = strlen(send_char);
											char *my_send_char = (char *)malloc(my_len+2);
											memset(my_send_char,0,my_len+2);
											memcpy(my_send_char,send_char,my_len);
											strcat(my_send_char,"\n\0");
											if(NET_FLAG)
											send(cd,my_send_char,my_len+1,0);
											free(send_char);
											send_char = NULL;
											free(my_send_char);
											my_send_char=NULL;
											cJSON_Delete(root_u);
											root_u = NULL;
										}
										else
										{
											cJSON_AddStringToObject(dev_state_cjson,"dev_state","00");
											char *device_state_list_char = cJSON_PrintUnformatted(device_state_list_data);
											memset(device_state_list,0,BUFFSIZE);
											memcpy(device_state_list,device_state_list_char,strlen(device_state_list_char));
											int state_fd = open("/root/device_state_list.txt",O_RDWR|O_CREAT|O_TRUNC,0777);
											write(state_fd,device_state_list_char,strlen(device_state_list_char));
											close(state_fd);
											free(device_state_list_char);
											device_state_list_char = NULL;
											
											cJSON *cmd_dev = cJSON_Duplicate(dev_state_cjson,1);
											cJSON_AddItemToObject(data_object,"dev_state",cmd_dev);
											char *send_char = cJSON_PrintUnformatted(root_u);
											int my_len = strlen(send_char);
											char *my_send_char = (char *)malloc(my_len+2);
											memset(my_send_char,0,my_len+2);
											memcpy(my_send_char,send_char,my_len);
											strcat(my_send_char,"\n\0");
											if(NET_FLAG)
											send(cd,my_send_char,my_len+1,0);
											free(send_char);
											send_char = NULL;
											free(my_send_char);
											my_send_char=NULL;
											cJSON_Delete(root_u);
											root_u = NULL;
										}
										break;
									}
									while_device_state_list_data = while_device_state_list_data->next;
									if(while_device_state_list_data == NULL)
									{
										cJSON *my_date = cJSON_CreateObject();//创建项目
										cJSON_AddItemToObject(my_device_state_list_data,p->id,my_date);
										cJSON_AddStringToObject(my_date,"dev_state","00");
										char *device_state_list_char = cJSON_PrintUnformatted(device_state_list_data);
										memset(device_state_list,0,BUFFSIZE);
										memcpy(device_state_list,device_state_list_char,strlen(device_state_list_char));
										int state_fd = open("/root/device_state_list.txt",O_RDWR|O_CREAT|O_TRUNC,0777);
										write(state_fd,device_state_list_char,strlen(device_state_list_char));
										close(state_fd);
										free(device_state_list_char);
										device_state_list_char=NULL;
										
										cJSON *cmd_dev = cJSON_Duplicate(my_date,1);
										cJSON_AddItemToObject(data_object,"dev_state",cmd_dev);
										char *send_char = cJSON_PrintUnformatted(root_u);
										int my_len = strlen(send_char);
										char *my_send_char = (char *)malloc(my_len+2);
										memset(my_send_char,0,my_len+2);
										memcpy(my_send_char,send_char,my_len);
										strcat(my_send_char,"\n\0");
										if(NET_FLAG)
										send(cd,my_send_char,my_len+1,0);
										free(send_char);
										send_char = NULL;
										free(my_send_char);
										my_send_char=NULL;
										cJSON_Delete(root_u);
										root_u = NULL;
									}
								}
							}
						}
					}
					pthread_mutex_unlock(&mutex_zl);
					cJSON_Delete(device_state_list_data);
					device_state_list_data = NULL;
				}
			}
			pthread_mutex_unlock(&mutex_human);
			p = p->next;
		}
		sleep(5);//每次的大循环延时2s
	}
}
/*人体登记到链表*/
void human_zt(char *mac,char *port,char *id,char *type,time_t num)
{
	HB *p = NULL;
	p = human_head;
	pthread_mutex_lock(&mutex_human);
	if(p==NULL)
	{
		human_d = (HB*)malloc(sizeof(HB));
		memset(human_d,0,sizeof(HB));
		if(num == 0)
			human_d->now_time = time(NULL);//记录时间
		else
			human_d->now_time = num;
		human_d->flag = 1;
		memcpy(human_d->mac,mac,17);
		memcpy(human_d->port,port,3);
		memcpy(human_d->id,id,strlen(id)+1);
		memcpy(human_d->type,type,strlen(type)+1);
		human_head = human_z = human_d;
		human_d->next = NULL;
	}
	else
	{
		while( p )
		{
			if(!mac_and_port_judge_human(p,mac,port))
			{
				if(num == 0)
					p->now_time = time(NULL);//记录时间
				else
					p->now_time = num;
				p->flag = 1;
				memset(p->id,0,20);
				memset(p->type,0,10);
				memcpy(p->id,id,strlen(id)+1);
				memcpy(p->type,type,strlen(type)+1);
				break;
			}
			else if(p->next == NULL)
			{
				human_d = (HB*)malloc(sizeof(HB));
				memset(human_d,0,sizeof(HB));
				if(num == 0)
					human_d->now_time = time(NULL);//记录时间
				else
					human_d->now_time = num;
				human_d->flag = 1;
				memcpy(human_d->mac,mac,17);
				memcpy(human_d->port,port,3);
				memcpy(human_d->id,id,strlen(id)+1);
				memcpy(human_d->type,type,strlen(type)+1);
				p->next = human_d;
				human_d->next = NULL;
				break ;
			}
			p = p->next;
		}
	}
	pthread_mutex_unlock(&mutex_human);
}
void my_delay(void)
{
	struct tm* p;
	DELAY *p_delay = NULL;
	time_t mytime;
	int first_flag = 1;
	uint8_t final_cmd[16];
	while(1)
	{
		p_delay = delay_head;
		time(&mytime);
		p = localtime(&mytime);
		while( p_delay )
		{
			pthread_mutex_lock(&mutex_delay);
			if( p_delay->flag)
			{
				if(p_delay->delay_time[0] == p->tm_hour && p_delay->delay_time[1] == p->tm_min && p_delay->delay_time[2] == p->tm_sec)
				{
					memset(final_cmd,0,16);
					cmd_mix(p_delay->dev_mac,p_delay->dev_port,p_delay->cmd,final_cmd);
					resend_zt(16,final_cmd,p_delay->dev_id,p_delay->dev_type);
					usart_send(fd,final_cmd,16);
					p_delay->flag = 0;
					my_delay_file();
				}
				else if(p_delay->delay_time[0] == p->tm_hour && p_delay->delay_time[1] == p->tm_min && first_flag && p_delay->delay_time[2] < p->tm_sec)
				{
					memset(final_cmd,0,16);
					cmd_mix(p_delay->dev_mac,p_delay->dev_port,p_delay->cmd,final_cmd);
					resend_zt(16,final_cmd,p_delay->dev_id,p_delay->dev_type);
					usart_send(fd,final_cmd,16);
					p_delay->flag = 0;
					my_delay_file();
				}
			}
			pthread_mutex_unlock(&mutex_delay);
			p_delay = p_delay->next;
		}
		first_flag = 0;
		sleep(1);
	}
}
void delay_zt(char *cmd,char *mac,char *port,char *dev_id,char *dev_type,int delay_hour,int delay_min,int delay_sec,int del_flag)//del_flag 0:1  set:del
{
	DELAY *p = NULL;
	p = delay_head;
	pthread_mutex_lock(&mutex_delay);
	if(p==NULL && !del_flag)
	{
		delay_d = (DELAY *)malloc(sizeof(DELAY));
		memset(delay_d,0,sizeof(DELAY));
		memcpy(delay_d->cmd,cmd,3);
		memcpy(delay_d->dev_mac,mac,17);
		memcpy(delay_d->dev_port,port,3);
		memcpy(delay_d->dev_id,dev_id,strlen(dev_id)+1);
		memcpy(delay_d->dev_type,dev_type,strlen(dev_type)+1);
		delay_d->delay_time[0] = delay_hour;
		delay_d->delay_time[1] = delay_min;
		delay_d->delay_time[2] = delay_sec;
		delay_d->flag = 1;
		delay_head = delay_z = delay_d;
		delay_d->next = NULL;
	}
	else
	{
		while( p )
		{
			if(!mac_and_port_judge_delay(p,mac,port))
			{
				memset(p->cmd,0,3);
				memcpy(p->cmd,cmd,3);
				memset(p->dev_id,0,20);
				memcpy(p->dev_id,dev_id,strlen(dev_id)+1);
				memset(p->dev_type,0,10);
				memcpy(p->dev_type,dev_type,strlen(dev_type)+1);
				p->delay_time[0] = delay_hour;
				p->delay_time[1] = delay_min;
				p->delay_time[2] = delay_sec;
				if(!del_flag)
					p->flag = 1;
				else if(del_flag)
					p->flag = 0;
				break;
			}
			else if(p->next == NULL && !del_flag)
			{
				delay_d = (DELAY *)malloc(sizeof(DELAY));
				memset(delay_d,0,sizeof(DELAY));
				memcpy(delay_d->cmd,cmd,3);
				memcpy(delay_d->dev_mac,mac,17);
				memcpy(delay_d->dev_port,port,3);
				memcpy(delay_d->dev_id,dev_id,strlen(dev_id)+1);
				memcpy(delay_d->dev_type,dev_type,strlen(dev_type)+1);
				delay_d->delay_time[0] = delay_hour;
				delay_d->delay_time[1] = delay_min;
				delay_d->delay_time[2] = delay_sec;
				delay_d->flag = 1;
				p->next = delay_d;
				delay_d->next = NULL;
				break;
			}
			p = p->next;
		}
	}
	pthread_mutex_unlock(&mutex_delay);
}
/*更新重发列表*/
void up_resend(uint8_t *data)
{
	RSD *p1 = NULL;
	p1 = resend_head;
	pthread_mutex_lock(&mutex_resend);
	while(p1)
	{
		if(!mac_and_port_judge(p1,data))
		{
			p1->now_times = 0;//清零更新
			break;
		}
		p1 = p1->next;
	}
	pthread_mutex_unlock(&mutex_resend);
}

/*********************************定时器********************************************/

void my_timer(void)
{
	struct tm* p;
	time_t mytime;
	int num,i,n,flag_go,first_num = 1;
	char *time_str=NULL,*time_comp=NULL;
	char my_mon[62];
	char my_day[120];
	char my_hour[5];
	char my_min[5];
	char my_week[15];
	while(1)
	{
		pthread_mutex_lock(&mutex_time);
		cJSON *time_list_root = cJSON_Parse(timer_list);
		pthread_mutex_unlock(&mutex_time);
		if(time_list_root==NULL)
			sleep(10);
		else if(time_list_root->child == NULL)
			sleep(10);
		else
		{
			time(&mytime);
			p = localtime(&mytime);
			p->tm_mon+=1;
			if(!p->tm_wday) p->tm_wday += 7;
			if(p->tm_sec ==0 || p->tm_sec ==1 || p->tm_sec ==2 || first_num)
			{
				first_num = 0;
				cJSON *time_data = cJSON_GetObjectItem(time_list_root,"data");
				cJSON *time_list = cJSON_GetObjectItem(time_data,"timer_list");
				num = cJSON_GetArraySize(time_list);
				cJSON *arr_for = NULL;
				cJSON *ennable = NULL;
				cJSON *time_root = NULL;
				for(i=0;i<num;i++)
				{
					arr_for = cJSON_GetArrayItem(time_list,i);
					ennable = cJSON_GetObjectItem(arr_for,"is_enable");
					if(!strcmp(ennable->valuestring,"1"))
					{
						time_root = cJSON_GetObjectItem(arr_for,"timer_date");
						time_str = strtok(time_root->valuestring," ");
						n=flag_go=0;
						while(time_str!=NULL)
						{
							n+=1;
							if(n==1)
							{
								memset(my_min,0,5);
								memcpy(my_min,time_str,(int)strlen(time_str));
							}
							else if(n==2)
							{
								memset(my_hour,0,5);
								memcpy(my_hour,time_str,(int)strlen(time_str));
							}
							else if(n==3)
							{
								memset(my_day,0,120);
								memcpy(my_day,time_str,(int)strlen(time_str));
							}
							else if(n==4)
							{
								memset(my_mon,0,62);
								memcpy(my_mon,time_str,(int)strlen(time_str));
							}
							else if(n==5)
							{
								memset(my_week,0,15);
								memcpy(my_week,time_str,(int)strlen(time_str));
							}
							time_str = strtok(NULL," ");
						}
						if(atoi(my_min) == p->tm_min) flag_go = 1;
						else continue;
						flag_go = 0;
						if(atoi(my_hour) == p->tm_hour) flag_go = 1;
						else continue;
						flag_go = 0;
						if(!strcmp(my_day,"*")) flag_go = 1;
						else if(strstr(my_day,","))
						{
							time_comp = strtok(my_day,",");
							while(time_comp != NULL)
							{
								if(atoi(time_comp) == p->tm_mday)
								{
									flag_go = 1;
									break;
								}
								time_comp = strtok(NULL,",");
							}
							if(flag_go == 0) continue;
						}
						else if(atoi(my_day) == p->tm_mday) flag_go = 1;
						else continue;
						flag_go = 0;
						if(!strcmp(my_mon,"*")) flag_go = 1;
						else if(strstr(my_mon,","))
						{
							time_comp = strtok(my_mon,",");
							while(time_comp != NULL)
							{
								if(atoi(time_comp) == p->tm_mon)
								{
									flag_go = 1;
									break;
								}
								time_comp = strtok(NULL,",");
							}
							if(flag_go == 0) continue;
						}
						else if(atoi(my_mon) == p->tm_mon) flag_go = 1;
						else continue;
						flag_go = 0;
						if(!strcmp(my_week,"*")) flag_go = 1;
						else if(strstr(my_week,","))
						{
							time_comp = strtok(my_week,",");
							while(time_comp != NULL)
							{
								if(atoi(time_comp) == p->tm_wday)
								{
									flag_go = 1;
									break;
								}
								time_comp = strtok(NULL,",");
							}
							if(flag_go == 0) continue;
						}
						else if(atoi(my_week) == p->tm_wday) flag_go = 1;
						else continue;
						if(flag_go==1)
						{
							cJSON *scene_id = cJSON_GetObjectItem(arr_for,"scen_id");
							run_scene(scene_id->valuestring);
							cJSON *push = cJSON_GetObjectItem(arr_for,"is_push");
							cJSON *timer_id_my = cJSON_GetObjectItem(arr_for,"timer_id");
							cJSON *send_root = cJSON_CreateObject();
							cJSON_AddStringToObject(send_root,"co","orange");
							cJSON_AddStringToObject(send_root,"api","gw_up_msg");
							cJSON *data_timer = cJSON_CreateObject();
							cJSON_AddItemToObject(send_root,"data",data_timer);
							cJSON_AddStringToObject(data_timer,"id",timer_id_my->valuestring);
							cJSON_AddStringToObject(data_timer,"msg_type","0204");
							if(strcmp(push->valuestring,"1")==0)
								cJSON_AddStringToObject(data_timer,"is_push","1");
							else
								cJSON_AddStringToObject(data_timer,"is_push","0");
							char *send_char = cJSON_PrintUnformatted(send_root);
							int my_len = strlen(send_char);
							char *my_send_char = (char *)malloc(my_len+2);
							memset(my_send_char,0,my_len+2);
							memcpy(my_send_char,send_char,my_len);
							strcat(my_send_char,"\n\0");
							if(NET_FLAG)
							send(cd,my_send_char,my_len+1,0);
							free(send_char);
							send_char=NULL;
							free(my_send_char);
							my_send_char=NULL;
							cJSON_Delete(send_root);
							send_root=NULL;
						}
					}
				}
				sleep(60-p->tm_sec);
			}
			else
				sleep(60-p->tm_sec);
		}
		cJSON_Delete(time_list_root);
		time_list_root=NULL;
	}
}

/*********************************获取实时设备状态*******************************************/
void get_status(void)
{
	int status_flag;
	pthread_mutex_lock(&mutex_sl);
	cJSON *dev_list_data = cJSON_Parse(device_list);//遍历设备列表
	pthread_mutex_unlock(&mutex_sl);
	if(dev_list_data->child !=NULL)
	{
		cJSON *my_dev_list_data = cJSON_GetObjectItem(dev_list_data,"data");
		cJSON *my_dev_list_list = cJSON_GetObjectItem(my_dev_list_data,"dev_list");
		int	data_l =  cJSON_GetArraySize(my_dev_list_list);
	
		cJSON *data_arr_jx = NULL;
		cJSON *tem_mac = NULL;
		cJSON *tem_type = NULL;
		cJSON *tem_port = NULL;
		int i;
		uint8_t final_cmd_stastus[16];
		
		for(i=0;i<data_l;i++)
		{
			status_flag=0;
			data_arr_jx = cJSON_GetArrayItem(my_dev_list_list,i);
			tem_mac = cJSON_GetObjectItem(data_arr_jx,"mac");
			tem_port = cJSON_GetObjectItem(data_arr_jx,"dev_port");
			tem_type = cJSON_GetObjectItem(data_arr_jx,"dev_type");
			if(strcmp(tem_type->valuestring,"010101")==0)
				status_flag=1;
			else if(strcmp(tem_type->valuestring,"010102")==0)
				status_flag=1;
			else if(strcmp(tem_type->valuestring,"010103")==0)
				status_flag=1;
			else if(strcmp(tem_type->valuestring,"010104")==0)
				status_flag=1;
			else if(strcmp(tem_type->valuestring,"010105")==0)
				status_flag=1;
			else if(strcmp(tem_type->valuestring,"010201")==0)
				status_flag=1;
			else if(strcmp(tem_type->valuestring,"010301")==0)
				status_flag=1;
			else if(strcmp(tem_type->valuestring,"010501")==0)
				status_flag=1;
			else if(strcmp(tem_type->valuestring,"010601")==0)
				status_flag=1;
			else if(strcmp(tem_type->valuestring,"040101")==0)
				status_flag=1;
			else if(strcmp(tem_type->valuestring,"040102")==0)
				status_flag=1;
			else if(strcmp(tem_type->valuestring,"040201")==0)
				status_flag=1;
			else if(strcmp(tem_type->valuestring,"040301")==0)
				status_flag=1;
			else if(strcmp(tem_type->valuestring,"080301")==0)
				status_flag=1;
			else if(strcmp(tem_type->valuestring,"080401")==0)
				status_flag=1;
			if(status_flag==1)
			{
				memset(final_cmd_stastus,0,16);
				cmd_mix_get_stastus1(tem_mac->valuestring,tem_port->valuestring,"00",final_cmd_stastus);
				usart_send(fd,final_cmd_stastus,16);
				usleep(200000);
			}
			else
				continue;
		}
	}
	cJSON_Delete(dev_list_data);
	dev_list_data=NULL;
}
void gateway_send_heart_jump(void)
{
	while(1)
	{
		cJSON *heart_root = cJSON_CreateObject();
		cJSON_AddStringToObject(heart_root,"heart_jump","heart jump");
		char *send_char = cJSON_PrintUnformatted(heart_root);
		int my_len = strlen(send_char);
		char *my_send_char = (char *)malloc(my_len+2);
		memset(my_send_char,0,my_len+2);
		memcpy(my_send_char,send_char,my_len);
		strcat(my_send_char,"\n\0");
		if(NET_FLAG)
			send(cd,my_send_char,my_len+1,0);
		free(send_char);
		send_char=NULL;
		free(my_send_char);
		my_send_char=NULL;
		cJSON_Delete(heart_root);
		heart_root=NULL;
		sleep(10);
	}
}
