#include "Reply1.h"

namespace RakLib {
	Reply1::Reply1(bool security, int64 serverID, uint16 mtuSize) : LoginPacket(28), 
		security(security), serverID(serverID), mtuSize(mtuSize) {}

	void Reply1::encode() {
		this->putByte(Packets::CONNECTION_REPLY_1);
		this->putByte(this->getMagic(), 16);
		this->putLong(this->serverID);
		this->putBool(this->security);
		this->putUShort(this->mtuSize);
	}
}