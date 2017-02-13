#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <iostream>

int main()
{
	sf::IpAddress ip = "192.168.23.209";//sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	char connectionType, mode;
	char buffer[2000];
	std::size_t received;
	std::string text = "Connected to: ";

	std::cout << "Enter (s) for Server, Enter (c) for Client: ";
	std::cin >> connectionType;
	
	if (connectionType == 's')
	{
		sf::TcpListener listener;
		listener.listen(5000);
		listener.accept(socket);
		text += "Server";
		mode = 's';
		listener.close();
	}
	else if (connectionType == 'c')
	{
		socket.connect(ip, 5000);
		text += "Client";
		mode = 'r';
	}
	
	socket.send(text.c_str(), text.length() + 1);
	socket.receive(buffer, sizeof(buffer), received);
	
	std::cout << buffer << std::endl;

	bool done = false;
	while (!done)
	{
		if (mode == 's')
		{
			std::getline(std::cin, text);
			if (text.length() > 0)
			{
				socket.send(text.c_str(), text.length() + 1);
				mode = 'r';
				if (text == "exit")
				{
					break;
				}
			}
		}
		else if (mode == 'r')
		{
			socket.receive(buffer, sizeof(buffer), received);
			if (received > 0)
			{
				std::cout << "Received: " << buffer << std::endl;
				mode = 's';
				if (strcmp(buffer, "exit") == 0)
				{
					break;
				}
			}
		}
	}

	socket.disconnect();
	return 0;
}