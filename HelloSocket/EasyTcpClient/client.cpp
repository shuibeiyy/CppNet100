#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<Windows.h>
#include<WinSock2.h>
#include<stdio.h>


int main()
{
	//启动windows socket 2.x环境（这一部分是windows独有的）
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//---------------------------
	
	//--用Socket API建立简易TCP客户端
	//***1(1.0版本base)、建立一个socket，三个参数分别为:
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == _sock)
	{
		printf("建立socket失败\n");
	}
	else printf("建立socket成功\n");

	//***2(1.0版本base)、连接服务器connect
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (SOCKET_ERROR == ret) printf("连接server失败\n");
	else printf("连接server成功\n");

	//2.1(1.1版本持续处理请求功能)
	while (true)
	{
		char wriBuf[128] = {};
		scanf("%s", wriBuf);
		if (0 == strcmp(wriBuf, "exit"))
		{
			printf("收到exit命令,任务结束。");
			break;
		}
		else
		{
			send(_sock, wriBuf, strlen(wriBuf) + 1, 0);
		}

		//***3(1.0版本base)、接收服务器信息recv
		char recvBuf[128] = {};     //接收的数据缓存区
		int res = recv(_sock, recvBuf, 128, 0);
		if (res > 0) printf("接收到数据：%s \n", recvBuf);
	}

	//***4(1.0版本base)、关闭套接字closesocket
	closesocket(_sock);


	//---------------------------
	//清除Windows socket环境
	WSACleanup();
	printf("已退出,任务结束");
	getchar();
	return 0;
}