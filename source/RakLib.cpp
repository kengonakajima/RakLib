#include "RakLib.h"

#include <cassert>
#include <sstream>

#include "packets/Info.h"
#include "packets/Reply1.h"
#include "packets/Reply2.h"
#include "packets/Request1.h"
#include "packets/Request2.h"
#include "packets/UnConnectedPing.h"
#include "packets/UnConnectedPong.h"
#include "Session.h"

namespace RakLib {
	RakLib::RakLib(SessionManager* sessionManager, const std::string& ip, uint16 port) {
		this->ip = ip;
		this->port = port;
		this->running = false;
		this->sessionManager = sessionManager;
	}

	// I think we can replace this with an hash of the ip (maybe + port)
	std::string RakLib::getSessionIdentifier(const std::string& ip, uint16 port) {
		std::string retval = ip;
		return retval.append(":" + port);
	}

	void RakLib::start() {
		assert(!this->running);

		this->running = true;
		this->socket = std::make_unique<UDPSocket>(this->ip, this->port);
		this->mainThread = std::thread(&RakLib::run, this);
	}

	void RakLib::run() {
		while (this->running) {
			std::unique_ptr<Packet> pck = this->socket->receive();
			uint8 pid = pck->getBuffer()[0];

			switch (pid) {
			case Packets::UNCONNECTED_PONG:
			case Packets::UNCONNECTED_PONG_2:
			{
				UnConnectedPing ping(pck.get());
				ping.decode();

				std::stringstream identifierStream; // "MCPE;Minecraft Server!;34;0.12.2;2;20"
				identifierStream << sessionManager->getType() << ";";
				identifierStream << sessionManager->getName() << ";";
				identifierStream << sessionManager->getProtocol() << ";";
				identifierStream << sessionManager->getGameVersion() << ";";
				identifierStream << sessionManager->getActivePlayers() << ";";
				identifierStream << sessionManager->getMaxPlayer();

				UnConnectedPong pong(this->sessionManager->getIdentifier(), ping.pingID, identifierStream.str());
				pong.encode();

				pong.ip = pck->ip;
				pong.port = pck->port;

				this->socket->send(&pong);
			}
			break;

			case Packets::CONNECTION_REQUEST_1:
			{
				Request1 request(pck.get());
				request.decode();

				Reply1 reply(this->sessionManager->useSecurity(), this->sessionManager->getIdentifier(), request.mtuSize);
				reply.encode();

				reply.ip = pck->ip;
				reply.port = pck->port;

				this->socket->send(&reply);
			}
			break;

			case Packets::CONNECTION_REQUEST_2:
			{
				Request2 request(pck.get());
				request.decode();

				Reply2 reply(this->sessionManager->getIdentifier(), pck->port, request.mtuSize, request.security);
				reply.encode();

				reply.ip = pck->ip;
				reply.port = pck->port;

				this->socket->send(&reply);
				this->sessionManager->addSession(pck->ip, pck->port, request.clientID, reply.mtuSize);
			}
			break;

			default:
			{
				Session* session = this->sessionManager->getSession(pck->ip, pck->port);
				if (session == nullptr) {
					break;
				}
				
				session->receivePacket(std::move(pck));
			}
			break;

			}
		}
	}

	void RakLib::sendPacket(Packet* packet) {
		this->socket->send(packet);
	}

	void RakLib::stop() {
		assert(!this->running);

		this->running = false;
		this->mainThread.join();
	}
}