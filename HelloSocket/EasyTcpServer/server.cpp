#define WIN32_LEAN_AND_MEAN           //解决Windows.h中已有的sock旧版和WinSock2.h重定义问题，也可以吧WinSock2.h放在Windows.h之前
#define _WINSOCK_DEPRECATED_NO_WARNINGS   //解决70行inet_ntoa函数不可用问题
#include<Windows.h>
#include<WinSock2.h>
#include<stdio.h>

int main()
{
	//启动windows socket 2.x环境（这一部分是windows独有的）
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//--------------------------------------
	//--用Socket API建立简易TCP服务端

	//***1(1.0版本base)、建立一个socket
	//int af（指定协议域ipv4或者v6），int type（socket类型：流格式套接字(SOCK_STREAM是面向连接的，丢失会重发，tcp)，数据报格式套接字(SOCK_DGRAM是无连接的，不计较丢失，udp)），int protocol指定传输协议（tcp、udp等）
	//这里选择ipv4，流格式套接字，TCP协议
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//***2(1.0版本base)、绑定用于接收客户端连接的网络端口 bind
	//创建socket地址结构体实例；
	sockaddr_in _sin = {};
	//设定协议域ipv4，保持与_sock一致
	_sin.sin_family = AF_INET;
	//设定网络端口号；主机的数据类型可能和网络字节序的数据类型不一样，需要将主机字节序转换为网络字节序：htons函数（host to net unsigned short)
	_sin.sin_port = htons(4567);
	//设置绑定的服务端ip地址。这里表示本机的任意ip地址都可以（默认写法）；
	_sin.sin_addr.S_un.S_addr = INADDR_ANY;
	//这里另一种写法，可以指定本地内网127，也可以指定公网ip，如果指定127那么通过公网就无法访问到内网
	//_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1")
	//这里使用的sockaddr_in和sockaddr内容一致，只是结构体内写法不同，sockaddr_in方便我们写参数
	if (SOCKET_ERROR == bind(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in)))  //绑定可能失败
	{
		printf("ERROR,绑定用于接收客户端连接的网络端口失败...\n");
	}
	else
	{
		printf("绑定成功...\n");
	}

	//***3(1.0版本base)、监听网络端口 listen
	//第二个参数backlog表示可以排队的最大连接数，socket()创建的socket默认是一个主动类型，listen则将socket变成被动类型，等待客户连接请求。
	if (SOCKET_ERROR == listen(_sock, 5))  //绑定可能失败
	{
		printf("ERROR,监听网络端口失败...\n");
	}
	else
	{
		printf("监听网络端口成功...\n");
	}

	//***4(1.0版本base)、等待接收客户端连接 accept
	//创建一个客户端的socket地址结构体
	sockaddr_in clientAddr = {};
	int nAddrlen = sizeof(clientAddr);  //计算这个结构体实例的长度
	//第一个参数是服务器端的socket描述字，accept返回已经连接的socket描述字
	//第二个参数是客户端的地址结构体实例，用于accept最后输出返回客户端的协议地址
	//第三个参数表示地址长度这个变量的位置
	SOCKET _cSock = INVALID_SOCKET;

	_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrlen);  //传入上面的参数
	if (INVALID_SOCKET == _cSock)
	{
		printf("ERROR,接收到无效客户端SOCKET...\n");
	}
	printf("新客户端加入：socket = %d ,IP: %s \n",(int)_cSock,inet_ntoa(clientAddr.sin_addr)); //输出新加入客户端的ip地址

	char _recvBuf[128] = {};
	while (true)
	{
		//4.1(1.1版本持续处理请求功能)接收客户端数据
		int nLen = recv(_cSock, _recvBuf, 128, 0);
		if (nLen <= 0)
		{
			printf("客户端已退出。\n");
			break;
		}
		printf("收到命令：%s \n", _recvBuf);
		if (0 == strcmp(_recvBuf, "getName")) //strcmp函数比较两个字符串，输出为一个int
		{
			char msgBuf[] = "Xiao Qiang.";
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		}
		else if (0 == strcmp(_recvBuf, "getAge"))
		{
			char msgBuf[] = "80.";
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		}
		else
		{
			char  msgBuf[] = "???";
			//***5(1.0版本base)、向客户端发送数据 send
			//第一个参数是发送目标客户端的socket描述字，第二个参数是一段字符串的指针，第三个参数是字符串长度（+1是结尾符号\0）,最后一个flags默认一般是0；
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		}

	}


	//***6(1.0版本base)、关闭套接字closesocket
	closesocket(_sock);
	//------------------------------------


	//清除Windows socket环境
	WSACleanup();
	printf("已退出,任务结束。\n");
	getchar();
	return 0;
}

