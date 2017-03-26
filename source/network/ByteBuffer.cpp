#include "ByteBuffer.h"

#include <cassert>

#include "Swap.h"

namespace RakLib {
	ByteBuffer::ByteBuffer() : buffer(nullptr), length(0), position(0) {}

	ByteBuffer::ByteBuffer(uint8* buffer, uint32 size) : buffer(buffer), length(size), position(0) {
		assert(buffer != nullptr && size != 0);
	}

	void ByteBuffer::putByte(uint8 v) {
		assert(this->position + sizeof(uint8) <= this->length);
		this->buffer[this->position++] = v;
	}

	void ByteBuffer::putByte(uint8* bytes, uint32 size) {
		assert(this->position + size <= this->length);
		memcpy(this->buffer + this->position, bytes, size);
		this->position += size;
	}

	void ByteBuffer::putChar(int8 c) {
		assert(this->position + sizeof(int8) <= this->length);
		this->buffer[this->position++] = c;
	}

	void ByteBuffer::putBool(bool value) {
		assert(this->position + sizeof(bool) <= this->length);
		this->buffer[this->position++] = value ? 0x01 : 0x00;
	}

	void ByteBuffer::putShort(int16 v) {
		assert(this->position + sizeof(int16) <= this->length);
#if COMMON_LITTLE_ENDIAN
		uint16 swapped = Common::swap16((uint16)v);
		memcpy(this->buffer + this->position, &swapped, sizeof(uint16));
#else
		memcpy(this->buffer + this->position, &v, sizeof(int16));
#endif
		this->position += sizeof(int16);
	}

	void ByteBuffer::putUShort(uint16 v) {
		assert(this->position + sizeof(uint16) <= this->length);
#if COMMON_LITTLE_ENDIAN
		uint16 swapped = Common::swap16(v);
		memcpy(this->buffer + this->position, &swapped, sizeof(uint16));
#else
		memcpy(this->buffer + this->position, &v, sizeof(uint16));
#endif
		this->position += sizeof(uint16);
	}

	void ByteBuffer::putTriad(int24 v) {
		assert(this->position + 3 <= this->length);
		this->buffer[this->position++] = (uint8)(v >> 16 & 0xFF);
		this->buffer[this->position++] = (uint8)(v >> 8 & 0xFF);
		this->buffer[this->position++] = (uint8)(v & 0xFF);
	}

	void ByteBuffer::putLTriad(int24 v) {
		assert(this->position + 3 <= this->length);
		this->buffer[this->position++] = (uint8)(v & 0xFF);
		this->buffer[this->position++] = (uint8)(v >> 8 & 0xFF);
		this->buffer[this->position++] = (uint8)(v >> 16 & 0xFF);
	}

	void ByteBuffer::putInt(int32 v) {
		assert(this->position + sizeof(int32) <= this->length);
#if COMMON_LITTLE_ENDIAN
		uint32 swapped = Common::swap32((uint32)v);
		memcpy(this->buffer + this->position, &swapped, sizeof(uint32));
#else
		memcpy(this->buffer + this->position, &v, sizeof(int32));
#endif
		this->position += sizeof(int32);
	}

	void ByteBuffer::putUInt(uint32 v) {
		assert(this->position + sizeof(uint32) <= this->length);
#if COMMON_LITTLE_ENDIAN
		uint32 swapped = Common::swap32(v);
		memcpy(this->buffer + this->position, &swapped, sizeof(uint32));
#else
		memcpy(this->buffer + this->position, &v, sizeof(uint32));
#endif
		this->position += sizeof(uint32);
	}

	void ByteBuffer::putLong(int64 v) {
		assert(this->position + sizeof(int64) <= this->length);
#if COMMON_LITTLE_ENDIAN
		uint64 swapped = Common::swap64((uint64)v);
		memcpy(this->buffer + this->position, &swapped, sizeof(uint64));
#else
		memcpy(this->buffer + this->position, &v, sizeof(int64));
#endif
		this->position += sizeof(int64);
	}

	void ByteBuffer::putULong(uint64 v) {
		assert(this->position + sizeof(uint64) <= this->length);
#if COMMON_LITTLE_ENDIAN
		uint64 swapped = Common::swap64(v);
		memcpy(this->buffer + this->position, &swapped, sizeof(uint64));
#else
		memcpy(this->buffer + this->position, &v, sizeof(uint64));
#endif
		this->position += sizeof(uint64);
	}

	void ByteBuffer::putFloat(f32 v) {
		assert(this->position + sizeof(f32) <= this->length);
#if COMMON_LITTLE_ENDIAN
		f32 swapped = Common::swapf(v);
		memcpy(this->buffer + this->position, &swapped, sizeof(f32));
#else
		memcpy(this->buffer + this->position, &v, sizeof(f32));
#endif
		this->position += sizeof(f32);
	}

	void ByteBuffer::putDouble(f64 v) {
		assert(this->position + sizeof(f64) <= this->length);
#if COMMON_LITTLE_ENDIAN
		f64 swapped = Common::swapd(v);
		memcpy(this->buffer + this->position, &swapped, sizeof(f64));
#else
		memcpy(this->buffer + this->position, &v, sizeof(f64));
#endif
		this->position += sizeof(f64);
	}

	void ByteBuffer::putString(const std::string& str) {
		this->putUShort((uint16)str.length());

		assert(this->position + str.length() <= this->length);
		memcpy(this->buffer + this->position, str.data(), str.length());
		this->position += str.length();
	}

	// Read Methods
	uint8 ByteBuffer::getByte() {
		assert(this->position + sizeof(uint8) <= this->length);
		return this->buffer[this->position++];
	}

	uint8* ByteBuffer::getByte(uint32 size) {
		assert(this->position + size <= this->length);

		if (size == 0) {
			return nullptr;
		}

		uint8* retval = new uint8[size];
		memcpy(retval, this->buffer + this->position, size);
		this->position += size;

		return retval;
	}

	int8 ByteBuffer::getChar() {
		assert(this->position + sizeof(int8) <= this->length);
		return (int8)(this->buffer[this->position++] & 0xFF);
	}

	bool ByteBuffer::getBool() {
		assert(this->position + sizeof(bool) <= this->length);
		return this->buffer[this->position++] == 0x01 ? true : false;
	}

	int16 ByteBuffer::getShort() {
		assert(this->position + sizeof(int16) <= this->length);
		int16 value = 0;
		memcpy(&value, this->buffer + this->position, sizeof(int16));
		this->position += sizeof(int16);
#if COMMON_LITTLE_ENDIAN
		return (int16)Common::swap16((uint16)value);
#else
		return value;
#endif
	}

	uint16 ByteBuffer::getUShort() {
		assert(this->position + sizeof(uint16) <= this->length);
		uint16 value = 0;
		memcpy(&value, this->buffer + this->position, sizeof(uint16));
		this->position += sizeof(uint16);
#if COMMON_LITTLE_ENDIAN
		return Common::swap16(value);
#else
		return value;
#endif
	}

	int24 ByteBuffer::getTriad() {
		assert(this->position + 3 <= this->length);
		return this->buffer[this->position++] << 16 & 0xFF | this->buffer[this->position++] << 8 & 0xFF | this->buffer[this->position++] & 0xFF;
	}

	int24 ByteBuffer::getLTriad() {
		assert(this->position + 3 <= this->length);
		return this->buffer[this->position++] & 0xFF | this->buffer[this->position++] << 8 & 0xFF | this->buffer[this->position++] << 16 & 0xFF;
	}

	int32 ByteBuffer::getInt() {
		assert(this->position + sizeof(int32) <= this->length);
		int32 value = 0;
		memcpy(&value, this->buffer + this->position, sizeof(int32));
		this->position += sizeof(int32);
#if COMMON_LITTLE_ENDIAN
		return (int32)Common::swap32((uint32)value);
#else
		return value;
#endif
	}

	uint32 ByteBuffer::getUInt() {
		assert(this->position + sizeof(uint32) <= this->length);
		uint32 value = 0;
		memcpy(&value, this->buffer + this->position, sizeof(uint32));
		this->position += sizeof(uint32);
#if COMMON_LITTLE_ENDIAN
		return Common::swap32(value);
#else
		return value;
#endif
	}

	int64 ByteBuffer::getLong() {
		assert(this->position + sizeof(int64) <= this->length);
		int64 value = 0;
		memcpy(&value, this->buffer + this->position, sizeof(int64));
		this->position += sizeof(int64);
#if COMMON_LITTLE_ENDIAN
		return (int64)Common::swap64((uint64)value);
#else
		return value;
#endif
	}

	uint64 ByteBuffer::getULong() {
		assert(this->position + sizeof(uint64) <= this->length);
		uint64 value = 0;
		memcpy(&value, this->buffer + this->position, sizeof(uint64));
		this->position += sizeof(uint64);
#if COMMON_LITTLE_ENDIAN
		return Common::swap64(value);
#else
		return value;
#endif
	}

	f32 ByteBuffer::getFloat() {
		assert(this->position + sizeof(f32) <= this->length);
		f32 value = 0;
		memcpy(&value, this->buffer + this->position, sizeof(f32));
		this->position += sizeof(f32);
#if COMMON_LITTLE_ENDIAN
		return Common::swapf(value);
#else
		return value;
#endif
	}

	f64 ByteBuffer::getDouble() {
		f64 value = 0;
		memcpy(&value, this->buffer + this->position, sizeof(f64));
		this->position += sizeof(f64);
#if COMMON_LITTLE_ENDIAN
		return Common::swapd(value);
#else
		return value;
#endif
	}

	std::string ByteBuffer::getString() {
		uint16 size = getUShort();

		assert(this->position + size <= this->length);
		std::string retval;
		for (uint16 i = 0; i < size; ++i) {
			retval += (char)this->buffer[this->position++] & 0xFF;
		}

		return retval;
	}

	uint8& ByteBuffer::operator[] (uint32 index) const {
		assert(index < this->length);
		return this->buffer[index];
	}

	uint32 ByteBuffer::getLength() const {
		return this->length;
	}

	uint32 ByteBuffer::getPosition() const {
		return this->position;
	}

	uint8* ByteBuffer::getBuffer() const {
		return this->buffer;
	}

	void ByteBuffer::setPosition(uint32 newPosition) {
		this->position = newPosition;
	}

	void ByteBuffer::print() const {
		for (uint32 i = 0; i < this->length; ++i) {
			printf("%02X ", this->buffer[i]);
			if ((i + 1) % 8 == 0 || i == this->length - 1) {
				printf("\n");
			}
		}
	}
}