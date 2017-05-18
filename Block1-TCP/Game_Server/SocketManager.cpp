#include "SocketManager.h"



SocketManager::SocketManager()
{

}


SocketManager::~SocketManager()
{
	for (auto it : _clients)
		it.socket->disconnect();

	_clients.clear();
}


void SocketManager::ServerInit()
{
	std::cout << "Server ON" << std::endl;
	sf::Socket::Status status = listener.listen(CONNECT_PORT);
	if (status != sf::Socket::Done) {
		std::cout << "Connection failed on port " << (int)CONNECT_PORT << std::endl;
		return;
	}
	selector.add(listener);
	SayConnections();
}


void SocketManager::SocketReceive() {
	std::size_t bytesReceived;

	while (1) {
		if (selector.wait()) {
			if (selector.isReady(listener)) {
				sf::TcpSocket* client = new sf::TcpSocket;
				if (listener.accept(*client) == sf::Socket::Done) {
					Client tClient;
					tClient.socket = client;
					_clients.push_back(tClient);
					selector.add(*client);
					std::cout << "New client connected: "; SayConnections();
				}
				else { 
					delete client; 
				}
			}
			else {
				std::vector<Client*> eraseList;
				for (auto it : _clients) {
					if (selector.isReady(*it.socket)) {
						sf::TcpSocket::Status st;
						st = it.socket->receive(&it.buffer, sizeof(it.buffer), bytesReceived);
						if (st == sf::TcpSocket::Disconnected) {
							std::cout << "Client disconnected, ";
							eraseList.push_back(&it);
						}
					}
				}
				eraseList.clear();
				SayConnections();
			}
		}
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

void SocketManager::SendMessage(std::string message)
{
	size_t sentBytes;

	for (auto it : _clients) {
		it.socket->send(message.c_str(), message.length() + 1, sentBytes);
	}
}

void SocketManager::SendMessage(std::string message, int i)
{
	size_t sentBytes;
	std::cout << "Sent: " << message << std::endl;
	clientSockets[i]->send(message.c_str(), message.length() + 1, sentBytes);
}

char* SocketManager::getBuffer(int i)
{
	return buffer[i];
}

void SocketManager::EraseBuffer(int i) {
	char t[2] = "\0";
	strcpy_s(buffer[i], t);
}


void SocketManager::SayConnections()
{
	std::cout << clientSockets.size() << " clients connected now" << std::endl;
}
