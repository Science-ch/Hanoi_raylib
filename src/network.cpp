#include <winsock2.h>
#include "network.h"
#include "gamestate.h"
#include <iostream>
#include <string.h>

// #define DEBUG_MODE


using namespace std;
int server()
{
	WSADATA wsaData;
	WORD wsaVersion=MAKEWORD(2,0);
	WSAStartup(wsaVersion,&wsaData);
    //����socket 
    SOCKET sockfd=socket(AF_INET,SOCK_STREAM,0);
    
    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
    addr.sin_port=htons(port);
    //��socket 
    if(bind(sockfd,(sockaddr*)&addr,sizeof(addr))==SOCKET_ERROR)
    {
		cout<<"���׽���ʧ��"<<endl;
    	WSACleanup(); 
    	return 0;
	}
    //����socket
	if(listen(sockfd,100)<0)
	{
		cout<<"�����׽���ʧ��"<<endl;
		WSACleanup(); 
		return 0;
	}
	else cout<<"��127.0.0.1:"<<port<<"�ϼ���"<<endl;
	sockaddr_in clientaddr;
	int clientlen=sizeof(SOCKADDR);
	SOCKET clientfd=accept(sockfd,(sockaddr*)&clientaddr,&clientlen); 
	if(clientfd==SOCKET_ERROR)
	{
		cout<<"����ʧ��"<<endl;
		WSACleanup(); 
    	return 0;
	}
	b:
	char *send_buf,*recv_buf,sendl[5]={0};
	send_buf=(char*)malloc((size_t)layer+9);
	recv_buf=(char*)malloc((size_t)layer+9);
	int recv_len=0,send_len=0;
	recv_len=recv(clientfd,recv_buf,5,0);
	if(strlen(recv_buf)<=0)
	{
		cout<<"�Ͽ�����"<<endl;
		closesocket(sockfd);
		closesocket(clientfd);
		WSACleanup();
	}
	else if(recv_buf[0]=='l')
	{
		sprintf(sendl,"%d",layer);
		send(clientfd,sendl,5,0);
	}
	else 
	{
		free(send_buf);
		free(recv_buf);
		goto b;
	}
	connected=1;
	int j,k;
	while(1)
	{
		recv_len=recv(clientfd,recv_buf,layer+8,0);
		if(recv_len!=layer+8)
		{
			cout<<"������Ϣʧ��"<<endl;
			closesocket(sockfd);
			closesocket(clientfd);
			WSACleanup();
			free(send_buf);
			free(recv_buf);
			return 0;
		}
		#ifdef DEBUG_MODE
			cout<<"recv bytes:"<<recv_len<<endl;
		#endif
        mtx.lock();
        other_tower[0].clear();
        other_tower[1].clear();
        other_tower[2].clear();
		j=3;
		for(int i=0;i<recv_buf[0];i++)
		{
            other_tower[0].push_back((int)recv_buf[j]);
            j++;			
		}
        for(int i=0;i<recv_buf[1];i++)
        {
            other_tower[1].push_back((int)recv_buf[j]);
            j++;			
        }
        for(int i=0;i<recv_buf[2];i++)
        {
            other_tower[2].push_back((int)recv_buf[j]);
            j++;			
        }
		selected_tower2=(int)recv_buf[layer+3];
		count2=*(int*)(recv_buf+layer+4);
		mtx.unlock();
		for(int i=0;i<3;i++)
            send_buf[i] = (char)self_tower[i].size();
        j=3;
		for(int i=0;i<3;i++)
		{
            for (int dat : self_tower[i])
            {
                send_buf[j] = (char)dat;
                j++;
            }
		}
		send_buf[layer+3]=(char)selected_tower;
		send_buf[layer+4]=(char)game_restart;
		if (game_restart) game_restart_send = 1;
		*(int*)(send_buf+layer+5) = count;
        mtx.unlock();
		send_len=send(clientfd,send_buf,layer+9,0);
        if(send_len!=layer+9)
		{
			cout<<"������Ϣʧ��"<<endl;
			closesocket(sockfd);
			closesocket(clientfd);
			WSACleanup();
			free(send_buf);
			free(recv_buf);
			return 0;
		}
		#ifdef DEBUG_MODE
			cout<<"send bytes:"<<send_len<<endl;
		#endif

		strcpy(recv_buf,"");
	}
	closesocket(sockfd);
	closesocket(clientfd);
	WSACleanup();
}

int client()
{
    WSADATA wsaData;
	WORD wsaVersion=MAKEWORD(2,0);
	WSAStartup(wsaVersion,&wsaData);
    //����socket 
    SOCKET sockfd=socket(AF_INET,SOCK_STREAM,0); 
    sockaddr_in serveraddr;
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.S_un.S_addr=inet_addr(ip_address.c_str());
    serveraddr.sin_port=htons(port);
    //��������
    if(connect(sockfd,(SOCKADDR*)&serveraddr,sizeof(SOCKADDR))==SOCKET_ERROR)
    {
		cout<<"����������ʧ��"<<endl;
    	WSACleanup();		 
    	return 0;
	}
	else cout<<"���������ӳɹ�"<<endl;	
	char getl[5]={0};	
	int recv_len=0,send_len=0;
	send_len=send(sockfd,"l",1,0);
	if(send_len<0)
	{
		cout<<"������Ϣʧ��"<<endl;
		return 0;
	}
	recv_len=recv(sockfd,getl,5,0);
	if(recv_len<0)exit(0);
	layer=atoi(getl);
	char *send_buf,*recv_buf;
	send_buf=(char*)malloc((size_t)layer+9);
	recv_buf=(char*)malloc((size_t)layer+9);
	connected=1;
	int j;
	while(1)
	{
		strcpy(recv_buf,"");
		Sleep(50);
		mtx.lock();
		for(int i=0;i<3;i++)
			send_buf[i]=(char)self_tower[i].size();
		j=3;
		for(int i=0;i<3;i++)
		{
            for (int dat : self_tower[i])
            {
                send_buf[j] = (char)dat;
                j++;
            }
		}
		j=layer+3;
		send_buf[j]=(char)selected_tower;
		*(int*)(send_buf+j+1) = count;
		mtx.unlock();
		send_len=send(sockfd,send_buf,layer+8,0);
		#ifdef DEBUG_MODE
			cout<<"send bytes:"<<send_len<<endl;
		#endif
		if(send_len!=layer+8)
		{
			cout<<"������Ϣʧ��"<<endl;
			closesocket(sockfd);
			WSACleanup();
			free(send_buf);
			free(recv_buf);
			return 0;
		}
		recv_len=recv(sockfd,recv_buf,layer+9,0);
		if(recv_len!=layer+9)
		{
			cout<<"������Ϣʧ��"<<endl;
			closesocket(sockfd);
			WSACleanup();
			free(send_buf);
			free(recv_buf);
			return 0;
		}
		#ifdef DEBUG_MODE
			cout<<"recv bytes:"<<recv_len<<endl;
		#endif
        mtx.lock();
        other_tower[0].clear();
        other_tower[1].clear();
        other_tower[2].clear();
		j=3;
		for(int i=0;i<recv_buf[0];i++)
		{
            other_tower[0].push_back((int)recv_buf[j]);
            j++;			
		}
        for(int i=0;i<recv_buf[1];i++)
        {
            other_tower[1].push_back((int)recv_buf[j]);
            j++;			
        }
        for(int i=0;i<recv_buf[2];i++)
        {
            other_tower[2].push_back((int)recv_buf[j]);
            j++;			
        }
		selected_tower2=(int)recv_buf[layer+3];
		game_restart=(bool)recv_buf[layer+4];
		count2=*(int*)(recv_buf+layer+5);
        mtx.unlock();
    }
	closesocket(sockfd);
	WSACleanup();
	return 0; 
}

void close_cmd()
{
    HWND hConsole = GetConsoleWindow();
    if (hConsole != NULL)
    {
        ShowWindow(hConsole, SW_HIDE);
    }
}

// ��Ϸʤ������
void win()
{
    while (1)
    {
        while (!game_running)Sleep(50);
        while (!game_won)
        {
            self[0] = self_tower[1].size();
            self[1] = self_tower[2].size();
            other[0] = other_tower[1].size();
            other[1] = other_tower[2].size();
            if (self[0] == layer || self[1] == layer) 
            {
                game_won = 1; // �Լ�ʤ��
                game_running = 0;
				first_press = 0;
            }
            if (other[0] == layer || other[1] == layer) 
            {
                game_won = 2; // �Է�ʤ��
                game_running = 0;
				first_press = 0;
            }
			Sleep(50);
        }   
        
    }
    
}