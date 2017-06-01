#include "SocketManager.h"



SocketManager::SocketManager()
{
	idCounter = 0;
}


SocketManager::~SocketManager()
{
	for (auto it : *_clients)
		it.socket->disconnect();

	_clients->clear();
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

void SocketManager::ServerInit(std::vector<Client> *clients)
{
	_clients = clients;
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
					_clients->push_back(*&Client(idCounter++, client));//Add new Client
					selector.add(*client);
					std::cout << "New client connected: "; SayConnections();
				}
				else { 
					delete client; 
				}
			}
			else {
				std::vector<int> eraseList;
				int i = 0;
				for (auto &it : *_clients) {
					if (selector.isReady(*it.socket)) {
						sf::TcpSocket::Status st = it.socket->receive(&it.buffer, sizeof(it.buffer), bytesReceived);
						
						if (st == sf::TcpSocket::Disconnected) {
							std::cout << "Client disconnected" << std::endl;
							eraseList.push_back(i);							
						}
					}
					i++;
				}
				for (int i = eraseList.size() - 1; i >= 0; i--)
				{
					_clients->at(eraseList[i]).socket->disconnect();
					_clients->erase(_clients->begin() + eraseList[i]);
				}
				eraseList.clear(); SayConnections();
			}
		}
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

void SocketManager::SendMessage(std::string message)
{
	size_t sentBytes;

	for (auto it : *_clients)
		it.socket->send(message.c_str(), message.length() + 1, sentBytes);
}

void SocketManager::SendMessage(std::string message, int i)
{
	size_t sentBytes;
	std::cout << "Sent: " << message << std::endl;
	for (auto it : *_clients) {
		if (it.player.id == i)
			it.socket->send(message.c_str(), message.length() + 1, sentBytes);
	}
}

void SocketManager::SendMessage(std::string message, sf::TcpSocket *sock)
{
	size_t sentBytes;
	std::cout << "Sent: " << message << std::endl;
	sock->send(message.c_str(), message.length() + 1, sentBytes);
}


char* SocketManager::getBuffer(int i)
{
	for (auto it : *_clients) {
		if (it.player.id == i)
			return it.buffer;
	}
	return 0;
}

void SocketManager::EraseBuffer(int i) {
	char t[2] = "\0"; 
	for (auto &it : *_clients) {
		if (it.player.id == i)
			strcpy_s(it.buffer, t);
	}
}


void SocketManager::SayConnections()
{
	std::cout << _clients->size() << " clients connected now" << std::endl;
}
