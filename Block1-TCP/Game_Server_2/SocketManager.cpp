#include "SocketManager.h"



SocketManager::SocketManager()
{
	idCounter = 0;
}


SocketManager::~SocketManager()
{
	for (auto it : _clients)
		it.socket->disconnect();

	_clients.clear();
}


void SocketManager::ServerInit(std::vector<Client> *clients)
{
	_clients = *clients;
	std::cout << "Server ON" << std::endl;
	sf::Socket::Status status = listener.listen(CONNECT_PORT);
	if (status != sf::Socket::Done) {
		std::cout << "Connection failed on port " << (int)CONNECT_PORT << std::endl;
		return;
	}
	selector.add(listener);
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
					tClient.player.id = idCounter++;
					tClient.playing = false;
					_clients.push_back(tClient);
					selector.add(*client);
					std::cout << "New client connected: " ;
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
			}
		}
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}