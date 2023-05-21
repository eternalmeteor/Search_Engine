#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "myhead.h"
#include "ProtocolParser.h"

using std::string;
using std::cin;
 
void test() 
{
	int clientfd = ::socket(AF_INET, SOCK_STREAM, 0);
	if(clientfd < 0) 
    {
		perror("socket");
		return ;
	}

	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8888);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t length = sizeof(serverAddr);

	if(::connect(clientfd,(struct sockaddr*)&serverAddr, length) < 0) 
    {
		perror("connect");
		close(clientfd);
		return;
	}
	printf("conn has connected!...\n");

	while(1)
    {
		json j;
		int cmd = 0;
		string cmdline;
		cout << "请输入要执行的操作：1.关键字推荐；2.网页查询:";
		getline(cin, cmdline);
		// cout << cmdline << endl;
		try {
			cmd = stoi(cmdline);
		} catch (std::invalid_argument&) {
			// 处理不能转换成整数的异常
			std::cout << "您的输入有误，请重新输入！" << std::endl;
			continue;
		}
		if(cmd != 1 && cmd != 2) {
			std::cout << "您的输入有误，请重新输入！" << std::endl;
			continue;
		}
		string line;
		cout << "请输入要查询的语句:";
		getline(cin, line);
		if(line.size() == 0) 
		{
			std::cout << "您的输入有误，请重新输入！" << std::endl;
			continue;
		}
		switch(cmd)
		{
			case 1:
			{
				j["cmd"] = KeyRecommand;
				j["word"] = line;
				break;
			}
			case 2:
			{
				j["cmd"] = WebPageSearch;
				j["word"] = line;
				break;
			}
			default:
			{
				std::cout << "您的输入有误，请重新输入！" << std::endl;
				break;
			}
		}
		// if(cmd == 1) {
		// 	j["cmd"] = KeyRecommand;
		// 	j["word"] = line;
		// } else {
		// 	j["cmd"] = WebPageSearch;
		// 	j["word"] = line;	
		// }
		//1. 客户端先发数据
		string buf; 
		buf = ProtocolParser::Json2string(j);
		buf.push_back('\n');
		int ret = send(clientfd, buf.data(), buf.size(), 0);
		
		char buff[10000] = {0};
		ret = recv(clientfd, buff, sizeof(buff), 0);
		j = json::parse(buff);
		vector<string> lines;
		ProtocolParser::json2vector(j, lines);
		for(auto iter : lines)
		{ 
			cout << iter << endl;
		}
	}

	close(clientfd);
} 
 
int main(int argc, char *argv[])
{
	test();
	return 0;
}
