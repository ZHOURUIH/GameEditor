using System;
using System.Text;

public class BinaryUtility
{
	public static bool readBool(byte[] buffer, ref int index, out bool success)
	{
		success = buffer.Length >= index + sizeof(bool);
		if (!success)
		{
			return false;
		}
		return buffer[index++] != 0;
	}
	public static sbyte readSByte(byte[] buffer, ref int index, out bool success)
	{
		success = buffer.Length >= index + sizeof(sbyte);
		if (!success)
		{
			return 0;
		}
		return (sbyte)buffer[index++];
	}
	public static byte readByte(byte[] buffer, ref int index, out bool success)
	{
		success = buffer.Length >= index + sizeof(byte);
		if (!success)
		{
			return 0;
		}
		return buffer[index++];
	}
	public static short readShort(byte[] buffer, ref int index, out bool success)
	{
		success = buffer.Length >= index + sizeof(short);
		if (!success)
		{
			return 0;
		}
		int pre = index;
		index += sizeof(short);
		return bytesToShort(buffer[pre + 0], buffer[pre + 1]);
	}
	public static short readShortBigEndian(byte[] buffer, ref int index, out bool success)
	{
		success = buffer.Length >= index + sizeof(short);
		if (!success)
		{
			return 0;
		}
		int pre = index;
		index += sizeof(short);
		return bytesToShortBigEndian(buffer[pre + 0], buffer[pre + 1]);
	}
	public static ushort readUShort(byte[] buffer, ref int index, out bool success)
	{
		success = buffer.Length >= index + sizeof(ushort);
		if (!success)
		{
			return 0;
		}
		int pre = index;
		index += sizeof(ushort);
		return bytesToUShort(buffer[pre + 0], buffer[pre + 1]);
	}
	public static ushort readUShortBigEndian(byte[] buffer, ref int index, out bool success)
	{
		success = buffer.Length >= index + sizeof(ushort);
		if (!success)
		{
			return 0;
		}
		int pre = index;
		index += sizeof(ushort);
		return bytesToUShortBigEndian(buffer[pre + 0], buffer[pre + 1]);
	}
	public static int readInt(byte[] buffer, ref int index, out bool success)
	{
		success = buffer.Length >= index + sizeof(int);
		if (!success)
		{
			return 0;
		}
		int pre = index;
		index += sizeof(int);
		return bytesToInt(buffer[pre + 0], buffer[pre + 1], buffer[pre + 2], buffer[pre + 3]);
	}
	public static int readIntBigEndian(byte[] buffer, ref int index, out bool success)
	{
		success = buffer.Length >= index + sizeof(int);
		if (!success)
		{
			return 0;
		}
		int pre = index;
		index += sizeof(int);
		return bytesToIntBigEndian(buffer[pre + 0], buffer[pre + 1], buffer[pre + 2], buffer[pre + 3]);
	}
	public static uint readUInt(byte[] buffer, ref int index, out bool success)
	{
		success = buffer.Length >= index + sizeof(uint);
		if (!success)
		{
			return 0;
		}
		int pre = index;
		index += sizeof(uint);
		return bytesToUInt(buffer[pre + 0], buffer[pre + 1], buffer[pre + 2], buffer[pre + 3]);
	}
	public static uint readUIntBigEndian(byte[] buffer, ref int index, out bool success)
	{
		success = buffer.Length >= index + sizeof(uint);
		if (!success)
		{
			return 0;
		}
		int pre = index;
		index += sizeof(uint);
		return bytesToUIntBigEndian(buffer[pre + 0], buffer[pre + 1], buffer[pre + 2], buffer[pre + 3]);
	}
	public static long readLong(byte[] buffer, ref int index, out bool success)
	{
		success = buffer.Length >= index + sizeof(long);
		if (!success)
		{
			return 0;
		}
		int pre = index;
		index += sizeof(long);
		return bytesToLong(buffer[pre + 0], buffer[pre + 1], buffer[pre + 2], buffer[pre + 3],
							buffer[pre + 4], buffer[pre + 5], buffer[pre + 6], buffer[pre + 7]);
	}
	public static long readLongBigEndian(byte[] buffer, ref int index, out bool success)
	{
		success = buffer.Length >= index + sizeof(long);
		if (!success)
		{
			return 0;
		}
		int pre = index;
		index += sizeof(long);
		return bytesToLongBigEndian(buffer[pre + 0], buffer[pre + 1], buffer[pre + 2], buffer[pre + 3],
									buffer[pre + 4], buffer[pre + 5], buffer[pre + 6], buffer[pre + 7]);
	}
	public static ulong readULong(byte[] buffer, ref int index, out bool success)
	{
		success = buffer.Length >= index + sizeof(ulong);
		if (!success)
		{
			return 0;
		}
		int pre = index;
		index += sizeof(ulong);
		return bytesToULong(buffer[pre + 0], buffer[pre + 1], buffer[pre + 2], buffer[pre + 3],
							buffer[pre + 4], buffer[pre + 5], buffer[pre + 6], buffer[pre + 7]);
	}
	public static ulong readULongBigEndian(byte[] buffer, ref int index, out bool success)
	{
		success = buffer.Length >= index + sizeof(ulong);
		if (!success)
		{
			return 0;
		}
		int pre = index;
		index += sizeof(ulong);
		return bytesToULongBigEndian(buffer[pre + 0], buffer[pre + 1], buffer[pre + 2], buffer[pre + 3],
									buffer[pre + 4], buffer[pre + 5], buffer[pre + 6], buffer[pre + 7]);
	}
	public static float readFloat(byte[] buffer, ref int index, out bool success, byte[] helpBuffer = null)
	{
		success = buffer.Length >= index + sizeof(float);
		if (!success)
		{
			return 0.0f;
		}
		int pre = index;
		index += sizeof(float);
		byte[] floatBuffer = helpBuffer;
		if (floatBuffer == null)
		{
			floatBuffer = new byte[4];
		}
		floatBuffer[0] = buffer[pre + 0];
		floatBuffer[1] = buffer[pre + 1];
		floatBuffer[2] = buffer[pre + 2];
		floatBuffer[3] = buffer[pre + 3];
		return bytesToFloat(floatBuffer);
	}
	public static float readFloatBigEndian(byte[] buffer, ref int index, out bool success, byte[] helpBuffer = null)
	{
		success = buffer.Length >= index + sizeof(float);
		if (!success)
		{
			return 0.0f;
		}
		int pre = index;
		index += sizeof(float);
		byte[] floatBuffer = helpBuffer;
		if (floatBuffer == null)
		{
			floatBuffer = new byte[4];
		}
		floatBuffer[0] = buffer[pre + 3];
		floatBuffer[1] = buffer[pre + 2];
		floatBuffer[2] = buffer[pre + 1];
		floatBuffer[3] = buffer[pre + 0];
		return bytesToFloat(floatBuffer);
	}
	// readCount表示读取的bool的个数,小于0表示按照destBuffer数组长度读取
	public static bool readBools(byte[] buffer, ref int index, bool[] destBuffer, int readCount = -1)
	{
		int boolCount = readCount < 0 ? destBuffer.Length : readCount;
		if (boolCount > destBuffer.Length)
		{
			return false;
		}
		for (int i = 0; i < boolCount; ++i)
		{
			destBuffer[i] = readBool(buffer, ref index, out bool success);
			if (!success)
			{
				return false;
			}
		}
		return true;
	}
	public static bool readBytes(byte[] buffer, ref int index, byte[] destBuffer, int bufferSize = -1, int destBufferSize = -1, int readSize = -1)
	{
		if (bufferSize == -1)
		{
			bufferSize = buffer.Length;
		}
		if (destBufferSize == -1)
		{
			destBufferSize = destBuffer.Length;
		}
		if (readSize == -1)
		{
			readSize = destBuffer.Length;
		}
		if (destBufferSize < readSize || readSize + index > bufferSize)
		{
			return false;
		}
		readSize = MathUtility.getMin(readSize, destBufferSize);
		if (readSize > 0)
		{
			memcpy(destBuffer, buffer, 0, index, readSize);
			index += readSize;
		}
		return true;
	}
	// readCount表示要读取的short个数,小于0表示使用数组长度
	public static bool readShorts(byte[] buffer, ref int index, short[] destBuffer, int readCount = -1)
	{
		int shortCount = readCount < 0 ? destBuffer.Length : readCount;
		if (shortCount > destBuffer.Length)
		{
			return false;
		}
		for (int i = 0; i < shortCount; ++i)
		{
			destBuffer[i] = readShort(buffer, ref index, out bool success);
			if (!success)
			{
				return false;
			}
		}
		return true;
	}
	// readCount表示要读取的short个数,小于0表示使用数组长度
	public static bool readShortsBigEndian(byte[] buffer, ref int index, short[] destBuffer, int readCount = -1)
	{
		int shortCount = readCount < 0 ? destBuffer.Length : readCount;
		if (shortCount > destBuffer.Length)
		{
			return false;
		}
		for (int i = 0; i < shortCount; ++i)
		{
			destBuffer[i] = readShortBigEndian(buffer, ref index, out bool success);
			if (!success)
			{
				return false;
			}
		}
		return true;
	}
	// readCount表示要读取的ushort个数,小于0表示使用数组长度
	public static bool readUShorts(byte[] buffer, ref int index, ushort[] destBuffer, int readCount = -1)
	{
		int ushortCount = readCount < 0 ? destBuffer.Length : readCount;
		if (ushortCount > destBuffer.Length)
		{
			return false;
		}
		for (int i = 0; i < ushortCount; ++i)
		{
			destBuffer[i] = readUShort(buffer, ref index, out bool success);
			if (!success)
			{
				return false;
			}
		}
		return true;
	}
	// readCount表示要读取的ushort个数,小于0表示使用数组长度
	public static bool readUShortsBigEndian(byte[] buffer, ref int index, ushort[] destBuffer, int readCount = -1)
	{
		int ushortCount = readCount < 0 ? destBuffer.Length : readCount;
		if (ushortCount > destBuffer.Length)
		{
			return false;
		}
		for (int i = 0; i < ushortCount; ++i)
		{
			destBuffer[i] = readUShortBigEndian(buffer, ref index, out bool success);
			if (!success)
			{
				return false;
			}
		}
		return true;
	}
	// readCount表示要读取的int个数,小于0表示使用destBuffer数组长度
	public static bool readInts(byte[] buffer, ref int index, int[] destBuffer, int readCount = -1)
	{
		int intCount = readCount < 0 ? destBuffer.Length : readCount;
		if (intCount > destBuffer.Length)
		{
			return false;
		}
		for (int i = 0; i < intCount; ++i)
		{
			destBuffer[i] = readInt(buffer, ref index, out bool success);
			if (!success)
			{
				return false;
			}
		}
		return true;
	}
	// readCount表示要读取的int个数,小于0表示使用destBuffer数组长度
	public static bool readIntsBigEndian(byte[] buffer, ref int index, int[] destBuffer, int readCount = -1)
	{
		int intCount = readCount < 0 ? destBuffer.Length : readCount;
		if (intCount > destBuffer.Length)
		{
			return false;
		}
		for (int i = 0; i < intCount; ++i)
		{
			destBuffer[i] = readIntBigEndian(buffer, ref index, out bool success);
			if (!success)
			{
				return false;
			}
		}
		return true;
	}
	// readCount表示要读取的uint个数,小于0表示使用数组长度
	public static bool readUInts(byte[] buffer, ref int index, uint[] destBuffer, int readCount = -1)
	{
		int uintCount = readCount < 0 ? destBuffer.Length : readCount;
		if (uintCount > destBuffer.Length)
		{
			return false;
		}
		for (int i = 0; i < uintCount; ++i)
		{
			destBuffer[i] = readUInt(buffer, ref index, out bool success);
			if (!success)
			{
				return false;
			}
		}
		return true;
	}
	// readCount表示要读取的uint个数,小于0表示使用数组长度
	public static bool readUIntsBigEndian(byte[] buffer, ref int index, uint[] destBuffer, int readCount = -1)
	{
		int uintCount = readCount < 0 ? destBuffer.Length : readCount;
		if (uintCount > destBuffer.Length)
		{
			return false;
		}
		for (int i = 0; i < uintCount; ++i)
		{
			destBuffer[i] = readUIntBigEndian(buffer, ref index, out bool success);
			if (!success)
			{
				return false;
			}
		}
		return true;
	}
	// readCount表示要读取的long个数,小于0表示使用数组长度
	public static bool readLongs(byte[] buffer, ref int index, long[] destBuffer, int readCount = -1)
	{
		int longCount = readCount < 0 ? destBuffer.Length : readCount;
		if (longCount > destBuffer.Length)
		{
			return false;
		}
		for (int i = 0; i < longCount; ++i)
		{
			destBuffer[i] = readLong(buffer, ref index, out bool success);
			if (!success)
			{
				return false;
			}
		}
		return true;
	}
	// readCount表示要读取的long个数,小于0表示使用数组长度
	public static bool readLongsBigEndian(byte[] buffer, ref int index, long[] destBuffer, int readCount = -1)
	{
		int longCount = readCount < 0 ? destBuffer.Length : readCount;
		if (longCount > destBuffer.Length)
		{
			return false;
		}
		for (int i = 0; i < longCount; ++i)
		{
			destBuffer[i] = readLongBigEndian(buffer, ref index, out bool success);
			if (!success)
			{
				return false;
			}
		}
		return true;
	}
	// readCount表示要读取的ulong个数,小于0表示使用数组长度
	public static bool readULongs(byte[] buffer, ref int index, ulong[] destBuffer, int readCount = -1)
	{
		int longCount = readCount < 0 ? destBuffer.Length : readCount;
		if (longCount > destBuffer.Length)
		{
			return false;
		}
		for (int i = 0; i < longCount; ++i)
		{
			destBuffer[i] = readULong(buffer, ref index, out bool success);
			if (!success)
			{
				return false;
			}
		}
		return true;
	}
	// readCount表示要读取的ulong个数,小于0表示使用数组长度
	public static bool readULongsBigEndian(byte[] buffer, ref int index, ulong[] destBuffer, int readCount = -1)
	{
		int longCount = readCount < 0 ? destBuffer.Length : readCount;
		if (longCount > destBuffer.Length)
		{
			return false;
		}
		for (int i = 0; i < longCount; ++i)
		{
			destBuffer[i] = readULongBigEndian(buffer, ref index, out bool success);
			if (!success)
			{
				return false;
			}
		}
		return true;
	}
	// readCount表示读取的float的数量,小于0表示使用数组的长度
	public static bool readFloats(byte[] buffer, ref int index, float[] destBuffer, int readCount = -1)
	{
		int floatCount = readCount < 0 ? destBuffer.Length : readCount;
		if (floatCount > destBuffer.Length)
		{
			return false;
		}
		for (int i = 0; i < floatCount; ++i)
		{
			destBuffer[i] = readFloat(buffer, ref index, out bool success);
			if (!success)
			{
				return false;
			}
		}
		return true;
	}
	// readCount表示读取的float的数量,小于0表示使用数组的长度
	public static bool readFloatsBigEndian(byte[] buffer, ref int index, float[] destBuffer, int readCount = -1)
	{
		int floatCount = readCount < 0 ? destBuffer.Length : readCount;
		if (floatCount > destBuffer.Length)
		{
			return false;
		}
		for (int i = 0; i < floatCount; ++i)
		{
			destBuffer[i] = readFloatBigEndian(buffer, ref index, out bool success);
			if (!success)
			{
				return false;
			}
		}
		return true;
	}
	public static bool writeBool(byte[] buffer, ref int index, bool value)
	{
		if (buffer.Length < index + sizeof(bool))
		{
			return false;
		}
		buffer[index++] = (byte)(value ? 1 : 0);
		return true;
	}
	public static bool writeByte(byte[] buffer, ref int index, byte value)
	{
		if (buffer.Length < index + sizeof(byte))
		{
			return false;
		}
		buffer[index++] = value;
		return true;
	}
	public static bool writeShort(byte[] buffer, ref int index, short value)
	{
		if (buffer.Length < index + sizeof(short))
		{
			return false;
		}
		// 为了获得最快速度,不使用for循环
		buffer[index++] = (byte)(((0xFF << (8 * 0)) & value) >> (8 * 0));
		buffer[index++] = (byte)(((0xFF << (8 * 1)) & value) >> (8 * 1));
		return true;
	}
	public static bool writeShortBigEndian(byte[] buffer, ref int index, short value)
	{
		if (buffer.Length < index + sizeof(short))
		{
			return false;
		}
		// 为了获得最快速度,不使用for循环
		buffer[index++] = (byte)(((0xFF << (8 * 1)) & value) >> (8 * 1));
		buffer[index++] = (byte)(((0xFF << (8 * 0)) & value) >> (8 * 0));
		return true;
	}
	public static bool writeUShort(byte[] buffer, ref int index, ushort value)
	{
		if (buffer.Length < index + sizeof(ushort))
		{
			return false;
		}
		// 为了获得最快速度,不使用for循环
		buffer[index++] = (byte)(((0xFF << (8 * 0)) & value) >> (8 * 0));
		buffer[index++] = (byte)(((0xFF << (8 * 1)) & value) >> (8 * 1));
		return true;
	}
	public static bool writeUShortBigEndian(byte[] buffer, ref int index, ushort value)
	{
		if (buffer.Length < index + sizeof(ushort))
		{
			return false;
		}
		// 为了获得最快速度,不使用for循环
		buffer[index++] = (byte)(((0xFF << (8 * 1)) & value) >> (8 * 1));
		buffer[index++] = (byte)(((0xFF << (8 * 0)) & value) >> (8 * 0));
		return true;
	}
	public static bool writeInt(byte[] buffer, ref int index, int value)
	{
		if (buffer.Length < index + sizeof(int))
		{
			return false;
		}
		// 为了获得最快速度,不使用for循环
		buffer[index++] = (byte)(((0xFF << (8 * 0)) & value) >> (8 * 0));
		buffer[index++] = (byte)(((0xFF << (8 * 1)) & value) >> (8 * 1));
		buffer[index++] = (byte)(((0xFF << (8 * 2)) & value) >> (8 * 2));
		buffer[index++] = (byte)(((0xFF << (8 * 3)) & value) >> (8 * 3));
		return true;
	}
	public static bool writeIntBigEndian(byte[] buffer, ref int index, int value)
	{
		if (buffer.Length < index + sizeof(int))
		{
			return false;
		}
		// 为了获得最快速度,不使用for循环
		buffer[index++] = (byte)(((0xFF << (8 * 3)) & value) >> (8 * 3));
		buffer[index++] = (byte)(((0xFF << (8 * 2)) & value) >> (8 * 2));
		buffer[index++] = (byte)(((0xFF << (8 * 1)) & value) >> (8 * 1));
		buffer[index++] = (byte)(((0xFF << (8 * 0)) & value) >> (8 * 0));
		return true;
	}
	public static bool writeUInt(byte[] buffer, ref int index, uint value)
	{
		if (buffer.Length < index + sizeof(uint))
		{
			return false;
		}
		// 为了获得最快速度,不使用for循环
		buffer[index++] = (byte)(((0xFF << (8 * 0)) & value) >> (8 * 0));
		buffer[index++] = (byte)(((0xFF << (8 * 1)) & value) >> (8 * 1));
		buffer[index++] = (byte)(((0xFF << (8 * 2)) & value) >> (8 * 2));
		buffer[index++] = (byte)(((0xFF << (8 * 3)) & value) >> (8 * 3));
		return true;
	}
	public static bool writeUIntBigEndian(byte[] buffer, ref int index, uint value)
	{
		if (buffer.Length < index + sizeof(uint))
		{
			return false;
		}
		// 为了获得最快速度,不使用for循环
		buffer[index++] = (byte)(((0xFF << (8 * 3)) & value) >> (8 * 3));
		buffer[index++] = (byte)(((0xFF << (8 * 2)) & value) >> (8 * 2));
		buffer[index++] = (byte)(((0xFF << (8 * 1)) & value) >> (8 * 1));
		buffer[index++] = (byte)(((0xFF << (8 * 0)) & value) >> (8 * 0));
		return true;
	}
	public static bool writeLong(byte[] buffer, ref int index, long value)
	{
		if (buffer.Length < index + sizeof(long))
		{
			return false;
		}
		// 为了获得最快速度,不使用for循环
		buffer[index++] = (byte)(((0xFF << (8 * 0)) & value) >> (8 * 0));
		buffer[index++] = (byte)(((0xFF << (8 * 1)) & value) >> (8 * 1));
		buffer[index++] = (byte)(((0xFF << (8 * 2)) & value) >> (8 * 2));
		buffer[index++] = (byte)(((0xFF << (8 * 3)) & value) >> (8 * 3));
		buffer[index++] = (byte)(((0xFF << (8 * 4)) & value) >> (8 * 4));
		buffer[index++] = (byte)(((0xFF << (8 * 5)) & value) >> (8 * 5));
		buffer[index++] = (byte)(((0xFF << (8 * 6)) & value) >> (8 * 6));
		buffer[index++] = (byte)(((0xFF << (8 * 7)) & value) >> (8 * 7));
		return true;
	}
	public static bool writeLongBigEndian(byte[] buffer, ref int index, long value)
	{
		if (buffer.Length < index + sizeof(long))
		{
			return false;
		}
		// 为了获得最快速度,不使用for循环
		buffer[index++] = (byte)(((0xFF << (8 * 7)) & value) >> (8 * 7));
		buffer[index++] = (byte)(((0xFF << (8 * 6)) & value) >> (8 * 6));
		buffer[index++] = (byte)(((0xFF << (8 * 5)) & value) >> (8 * 5));
		buffer[index++] = (byte)(((0xFF << (8 * 4)) & value) >> (8 * 4));
		buffer[index++] = (byte)(((0xFF << (8 * 3)) & value) >> (8 * 3));
		buffer[index++] = (byte)(((0xFF << (8 * 2)) & value) >> (8 * 2));
		buffer[index++] = (byte)(((0xFF << (8 * 1)) & value) >> (8 * 1));
		buffer[index++] = (byte)(((0xFF << (8 * 0)) & value) >> (8 * 0));
		return true;
	}
	public static bool writeULong(byte[] buffer, ref int index, ulong value)
	{
		if (buffer.Length < index + sizeof(ulong))
		{
			return false;
		}
		// 为了获得最快速度,不使用for循环
		buffer[index++] = (byte)((((ulong)0xFF << (8 * 0)) & value) >> (8 * 0));
		buffer[index++] = (byte)((((ulong)0xFF << (8 * 1)) & value) >> (8 * 1));
		buffer[index++] = (byte)((((ulong)0xFF << (8 * 2)) & value) >> (8 * 2));
		buffer[index++] = (byte)((((ulong)0xFF << (8 * 3)) & value) >> (8 * 3));
		buffer[index++] = (byte)((((ulong)0xFF << (8 * 4)) & value) >> (8 * 4));
		buffer[index++] = (byte)((((ulong)0xFF << (8 * 5)) & value) >> (8 * 5));
		buffer[index++] = (byte)((((ulong)0xFF << (8 * 6)) & value) >> (8 * 6));
		buffer[index++] = (byte)((((ulong)0xFF << (8 * 7)) & value) >> (8 * 7));
		return true;
	}
	public static bool writeULongBigEndian(byte[] buffer, ref int index, ulong value)
	{
		if (buffer.Length < index + sizeof(ulong))
		{
			return false;
		}
		// 为了获得最快速度,不使用for循环
		buffer[index++] = (byte)((((ulong)0xFF << (8 * 7)) & value) >> (8 * 7));
		buffer[index++] = (byte)((((ulong)0xFF << (8 * 6)) & value) >> (8 * 6));
		buffer[index++] = (byte)((((ulong)0xFF << (8 * 5)) & value) >> (8 * 5));
		buffer[index++] = (byte)((((ulong)0xFF << (8 * 4)) & value) >> (8 * 4));
		buffer[index++] = (byte)((((ulong)0xFF << (8 * 3)) & value) >> (8 * 3));
		buffer[index++] = (byte)((((ulong)0xFF << (8 * 2)) & value) >> (8 * 2));
		buffer[index++] = (byte)((((ulong)0xFF << (8 * 1)) & value) >> (8 * 1));
		buffer[index++] = (byte)((((ulong)0xFF << (8 * 0)) & value) >> (8 * 0));
		return true;
	}
	public static bool writeFloat(byte[] buffer, ref int index, float value)
	{
		if (buffer.Length < index + sizeof(float))
		{
			return false;
		}
		byte[] valueByte = toBytes(value);
		// 为了获得最快速度,不使用for循环
		buffer[index++] = valueByte[0];
		buffer[index++] = valueByte[1];
		buffer[index++] = valueByte[2];
		buffer[index++] = valueByte[3];
		return true;
	}
	public static bool writeFloatBigEndian(byte[] buffer, ref int index, float value)
	{
		if (buffer.Length < index + sizeof(float))
		{
			return false;
		}
		byte[] valueByte = toBytes(value);
		// 为了获得最快速度,不使用for循环
		buffer[index++] = valueByte[3];
		buffer[index++] = valueByte[2];
		buffer[index++] = valueByte[1];
		buffer[index++] = valueByte[0];
		return true;
	}
	// writeCount表示要写入的bool个数,小于0表示将整个数组全部写入
	public static bool writeBools(byte[] buffer, ref int index, bool[] sourceBuffer, int writeCount = -1)
	{
		bool ret = true;
		int boolCount = writeCount < 0 ? sourceBuffer.Length : writeCount;
		for (int i = 0; i < boolCount; ++i)
		{
			ret = writeBool(buffer, ref index, sourceBuffer[i]) && ret;
		}
		return ret;
	}
	public static bool writeBytes(byte[] buffer, ref int index, byte[] sourceBuffer, int bufferSize = -1, int sourceBufferSize = -1, int writeSize = -1)
	{
		if (bufferSize == -1)
		{
			bufferSize = buffer.Length;
		}
		if (sourceBufferSize == -1)
		{
			sourceBufferSize = sourceBuffer.Length;
		}
		if (writeSize == -1)
		{
			writeSize = sourceBuffer.Length;
		}
		if (writeSize > sourceBufferSize || writeSize + index > bufferSize)
		{
			return false;
		}
		memcpy(buffer, sourceBuffer, index, 0, writeSize);
		index += writeSize;
		return true;
	}
	// writeCount表示要写入的short个数,小于0表示写入整个数组
	public static bool writeShorts(byte[] buffer, ref int index, short[] sourceBuffer, int writeCount = -1)
	{
		bool ret = true;
		int count = writeCount < 0 ? sourceBuffer.Length : writeCount;
		for (int i = 0; i < count; ++i)
		{
			ret = writeShort(buffer, ref index, sourceBuffer[i]) && ret;
		}
		return ret;
	}
	// writeCount表示要写入的short个数,小于0表示写入整个数组
	public static bool writeShortsBigEndian(byte[] buffer, ref int index, short[] sourceBuffer, int writeCount = -1)
	{
		bool ret = true;
		int count = writeCount < 0 ? sourceBuffer.Length : writeCount;
		for (int i = 0; i < count; ++i)
		{
			ret = writeShortBigEndian(buffer, ref index, sourceBuffer[i]) && ret;
		}
		return ret;
	}
	// writeCount表示要写入的ushort个数,小于0表示写入整个数组
	public static bool writeUShorts(byte[] buffer, ref int index, ushort[] sourceBuffer, int writeCount = -1)
	{
		bool ret = true;
		int count = writeCount < 0 ? sourceBuffer.Length : writeCount;
		for (int i = 0; i < count; ++i)
		{
			ret = writeUShort(buffer, ref index, sourceBuffer[i]) && ret;
		}
		return ret;
	}
	// writeCount表示要写入的ushort个数,小于0表示写入整个数组
	public static bool writeUShortsBigEndian(byte[] buffer, ref int index, ushort[] sourceBuffer, int writeCount = -1)
	{
		bool ret = true;
		int count = writeCount < 0 ? sourceBuffer.Length : writeCount;
		for (int i = 0; i < count; ++i)
		{
			ret = writeUShortBigEndian(buffer, ref index, sourceBuffer[i]) && ret;
		}
		return ret;
	}
	// writeCount表示要写入的int个数,小于0表示写入整个数组
	public static bool writeInts(byte[] buffer, ref int index, int[] sourceBuffer, int writeCount = -1)
	{
		bool ret = true;
		int count = writeCount < 0 ? sourceBuffer.Length : writeCount;
		for (int i = 0; i < count; ++i)
		{
			ret = writeInt(buffer, ref index, sourceBuffer[i]) && ret;
		}
		return ret;
	}
	// writeCount表示要写入的int个数,小于0表示写入整个数组
	public static bool writeIntsBigEndian(byte[] buffer, ref int index, int[] sourceBuffer, int writeCount = -1)
	{
		bool ret = true;
		int count = writeCount < 0 ? sourceBuffer.Length : writeCount;
		for (int i = 0; i < count; ++i)
		{
			ret = writeIntBigEndian(buffer, ref index, sourceBuffer[i]) && ret;
		}
		return ret;
	}
	// writeCount表示要写入的uint个数,小于0表示写入整个数组
	public static bool writeUInts(byte[] buffer, ref int index, uint[] sourceBuffer, int writeCount = -1)
	{
		bool ret = true;
		int count = writeCount < 0 ? sourceBuffer.Length : writeCount;
		for (int i = 0; i < count; ++i)
		{
			ret = writeUInt(buffer, ref index, sourceBuffer[i]) && ret;
		}
		return ret;
	}
	// writeCount表示要写入的uint个数,小于0表示写入整个数组
	public static bool writeUIntsBigEndian(byte[] buffer, ref int index, uint[] sourceBuffer, int writeCount = -1)
	{
		bool ret = true;
		int count = writeCount < 0 ? sourceBuffer.Length : writeCount;
		for (int i = 0; i < count; ++i)
		{
			ret = writeUIntBigEndian(buffer, ref index, sourceBuffer[i]) && ret;
		}
		return ret;
	}
	// writeCount表示要写入的long的个数,小于0表示写入整个数组
	public static bool writeLongs(byte[] buffer, ref int index, long[] sourceBuffer, int writeCount = -1)
	{
		bool ret = true;
		int count = writeCount < 0 ? sourceBuffer.Length : writeCount;
		for (int i = 0; i < count; ++i)
		{
			ret = writeLong(buffer, ref index, sourceBuffer[i]) && ret;
		}
		return ret;
	}
	// writeCount表示要写入的long的个数,小于0表示写入整个数组
	public static bool writeLongsBigEndian(byte[] buffer, ref int index, long[] sourceBuffer, int writeCount = -1)
	{
		bool ret = true;
		int count = writeCount < 0 ? sourceBuffer.Length : writeCount;
		for (int i = 0; i < count; ++i)
		{
			ret = writeLongBigEndian(buffer, ref index, sourceBuffer[i]) && ret;
		}
		return ret;
	}
	// writeCount表示要写入的ulong的个数,小于0表示写入整个数组
	public static bool writeULongs(byte[] buffer, ref int index, ulong[] sourceBuffer, int writeCount = -1)
	{
		bool ret = true;
		int count = writeCount < 0 ? sourceBuffer.Length : writeCount;
		for (int i = 0; i < count; ++i)
		{
			ret = writeULong(buffer, ref index, sourceBuffer[i]) && ret;
		}
		return ret;
	}
	// writeCount表示要写入的ulong的个数,小于0表示写入整个数组
	public static bool writeULongsBigEndian(byte[] buffer, ref int index, ulong[] sourceBuffer, int writeCount = -1)
	{
		bool ret = true;
		int count = writeCount < 0 ? sourceBuffer.Length : writeCount;
		for (int i = 0; i < count; ++i)
		{
			ret = writeULongBigEndian(buffer, ref index, sourceBuffer[i]) && ret;
		}
		return ret;
	}
	// writeCount表示要写入的浮点数的数量,小于0表示写入整个数组
	public static bool writeFloats(byte[] buffer, ref int index, float[] sourceBuffer, int writeCount = -1)
	{
		bool ret = true;
		int count = writeCount < 0 ? sourceBuffer.Length : writeCount;
		for (int i = 0; i < count; ++i)
		{
			ret = writeFloat(buffer, ref index, sourceBuffer[i]) && ret;
		}
		return ret;
	}
	// writeCount表示要写入的浮点数的数量,小于0表示写入整个数组
	public static bool writeFloatsBigEndian(byte[] buffer, ref int index, float[] sourceBuffer, int writeCount = -1)
	{
		bool ret = true;
		int count = writeCount < 0 ? sourceBuffer.Length : writeCount;
		for (int i = 0; i < count; ++i)
		{
			ret = writeFloatBigEndian(buffer, ref index, sourceBuffer[i]) && ret;
		}
		return ret;
	}
	public static void memcpy(Array dest, Array src, int destByteOffset, int srcByteOffset, int byteCount)
	{
		Buffer.BlockCopy(src, srcByteOffset, dest, destByteOffset, byteCount);
	}
	public static void memmove<T>(ref T[] data, int dest, int src, int count)
	{
		if (count <= 0)
		{
			return;
		}
		// 如果两个内存区有相交的部分,并且源地址在前面,则从后面往前拷贝字节
		if (src < dest && src + count > dest)
		{
			for (int i = 0; i < count; ++i)
			{
				data[count - i - 1 + dest] = data[count - i - 1 + src];
			}
		}
		else
		{
			for (int i = 0; i < count; ++i)
			{
				data[i + dest] = data[i + src];
			}
		}
	}
	// 将数组每个元素值设置为指定值
	public static void memset<T>(T[] p, T value, int length = -1)
	{
		if (length == -1)
		{
			length = p.Length;
		}
		for (int i = 0; i < length; ++i)
		{
			p[i] = value;
		}
	}
	public static byte[] toBytes(float value)
	{
		return BitConverter.GetBytes(value);
	}
	public static float bytesToFloat(byte[] array)
	{
		if (array == null)
		{
			return 0.0f;
		}
		return BitConverter.ToSingle(array, 0);
	}
	public static byte[] stringToBytes(string str, Encoding encoding = null)
	{
		if (str == null)
		{
			return null;
		}
		// 默认为UTF8
		if (encoding == null)
		{
			encoding = Encoding.UTF8;
		}
		return encoding.GetBytes(str);
	}
	public static string bytesToString(byte[] bytes, Encoding encoding = null)
	{
		if (bytes == null)
		{
			return null;
		}
		if (bytes.Length == 0)
		{
			return string.Empty;
		}
		// 默认为UTF8
		if (encoding == null)
		{
			encoding = Encoding.UTF8;
		}
		return removeLastZero(encoding.GetString(bytes));
	}
	public static string bytesToString(byte[] bytes, int startIndex, int count, Encoding encoding = null)
	{
		if (bytes == null || count < 0)
		{
			return null;
		}
		if (bytes.Length == 0 || count == 0 || startIndex + count > bytes.Length)
		{
			return string.Empty;
		}
		// 默认为UTF8
		if (encoding == null)
		{
			encoding = Encoding.UTF8;
		}
		return removeLastZero(encoding.GetString(bytes, startIndex, count));
	}
	// 字节数组转换为字符串时,末尾可能会带有数字0,此时在字符串比较时会出现错误,所以需要移除字符串末尾的0
	public static string removeLastZero(string str)
	{
		int strLen = str.Length;
		int newLen = strLen;
		for (int i = 0; i < strLen; ++i)
		{
			if (str[i] == 0)
			{
				newLen = i;
				break;
			}
		}
		str = str.Substring(0, newLen);
		return str;
	}
	public static short bytesToShortBigEndian(byte[] array)
	{
		if (array == null)
		{
			return 0;
		}
		return bytesToShortBigEndian(array[0], array[1]);
	}
	public static short bytesToShort(byte byte0, byte byte1) { return (short)((byte1 << (8 * 1)) | (byte0 << (8 * 0))); }
	public static short bytesToShortBigEndian(byte byte0, byte byte1) { return (short)((byte1 << (8 * 0)) | (byte0 << (8 * 1))); }
	public static ushort bytesToUShort(byte[] array)
	{
		if (array == null)
		{
			return 0;
		}
		return bytesToUShort(array[0], array[1]);
	}
	public static ushort bytesToUShortBigEndian(byte[] array)
	{
		if (array == null)
		{
			return 0;
		}
		return bytesToUShortBigEndian(array[0], array[1]);
	}
	public static ushort bytesToUShort(byte b0, byte b1) { return (ushort)((b1 << (8 * 1)) | (b0 << (8 * 0))); }
	public static ushort bytesToUShortBigEndian(byte b0, byte b1) { return (ushort)((b1 << (8 * 0)) | (b0 << (8 * 1))); }
	public static int bytesToInt(byte[] array)
	{
		if (array == null)
		{
			return 0;
		}
		return bytesToInt(array[0], array[1], array[2], array[3]);
	}
	public static int bytesToIntBigEndian(byte[] array)
	{
		if (array == null)
		{
			return 0;
		}
		return bytesToIntBigEndian(array[0], array[1], array[2], array[3]);
	}
	public static int bytesToInt(byte b0, byte b1, byte b2, byte b3) { return (b3 << (8 * 3)) | (b2 << (8 * 2)) | (b1 << (8 * 1)) | (b0 << (8 * 0)); }
	public static int bytesToIntBigEndian(byte b0, byte b1, byte b2, byte b3) { return (b3 << (8 * 0)) | (b2 << (8 * 1)) | (b1 << (8 * 2)) | (b0 << (8 * 3)); }
	public static uint bytesToUInt(byte[] array)
	{
		if (array == null)
		{
			return 0;
		}
		return bytesToUInt(array[0], array[1], array[2], array[3]);
	}
	public static uint bytesToUIntBigEndian(byte[] array)
	{
		if (array == null)
		{
			return 0;
		}
		return bytesToUIntBigEndian(array[0], array[1], array[2], array[3]);
	}
	public static uint bytesToUInt(byte b0, byte b1, byte b2, byte b3)
	{
		return (uint)((b3 << (8 * 3)) | (b2 << (8 * 2)) | (b1 << (8 * 1)) | (b0 << (8 * 0)));
	}
	public static uint bytesToUIntBigEndian(byte b0, byte b1, byte b2, byte b3)
	{
		return (uint)((b3 << (8 * 0)) | (b2 << (8 * 1)) | (b1 << (8 * 2)) | (b0 << (8 * 3)));
	}
	public static long bytesToLong(byte b0, byte b1, byte b2, byte b3, byte b4, byte b5, byte b6, byte b7)
	{
		return (long)b7 << (8 * 7) | (long)b6 << (8 * 6) | (long)b5 << (8 * 5) | (long)b4 << (8 * 4) |
				(long)b3 << (8 * 3) | (long)b2 << (8 * 2) | (long)b1 << (8 * 1) | (long)b0 << (8 * 0);
	}
	public static long bytesToLongBigEndian(byte b0, byte b1, byte b2, byte b3, byte b4, byte b5, byte b6, byte b7)
	{
		return (long)b7 << (8 * 0) | (long)b6 << (8 * 1) | (long)b5 << (8 * 2) | (long)b4 << (8 * 3) |
				(long)b3 << (8 * 4) | (long)b2 << (8 * 5) | (long)b1 << (8 * 6) | (long)b0 << (8 * 7);
	}
	public static ulong bytesToULong(byte b0, byte b1, byte b2, byte b3, byte b4, byte b5, byte b6, byte b7)
	{
		return (ulong)b7 << (8 * 7) | (ulong)b6 << (8 * 6) | (ulong)b5 << (8 * 5) | (ulong)b4 << (8 * 4) |
				(ulong)b3 << (8 * 3) | (ulong)b2 << (8 * 2) | (ulong)b1 << (8 * 1) | (ulong)b0 << (8 * 0);
	}
	public static ulong bytesToULongBigEndian(byte b0, byte b1, byte b2, byte b3, byte b4, byte b5, byte b6, byte b7)
	{
		return (ulong)b7 << (8 * 0) | (ulong)b6 << (8 * 1) | (ulong)b5 << (8 * 2) | (ulong)b4 << (8 * 3) |
				(ulong)b3 << (8 * 4) | (ulong)b2 << (8 * 5) | (ulong)b1 << (8 * 6) | (ulong)b0 << (8 * 7);
	}
}