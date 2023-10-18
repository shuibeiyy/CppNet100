#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<WinSock2.h>

int main()
{	
	//启动windows socket 2.x环境（这一部分是windows独有的）
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//-------------
	//--用Socket API建立简易TCP客户端
	//1、建立一个socket
	//2、连接服务器connect
	//3、接收服务器信息recv
	//4、关闭套接字closesocket
	//--用Socket API建立简易TCP服务端
	//1、建立一个socket
	//2、绑定用于接收客户端连接的网络端口 bind
	//3、监听网络端口 listen
	//4、等待接收客户端连接 accept
	//5、向客户端发送数据 send
	//6、关闭套接字closesocket
	//--------------
	//清除Windows socket环境
	WSACleanup();
	return 0;
}
