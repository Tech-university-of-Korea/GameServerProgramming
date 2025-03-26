#pragma once

#include <bit>
#include <iostream>
#include <format>
#include <source_location>

inline constexpr auto PROTOCOL_ENDIAN = std::endian::big;
inline constexpr auto NATIVE_ENDIAN = std::endian::native;

template <typename T>
inline constexpr T ZERO = 0;

inline void PrintErrorMessage(int errorCode = ::WSAGetLastError(), const std::source_location& sl = std::source_location::current())
{
	LPVOID msg;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<char*>(&msg),
		0,
		NULL
	);

	auto str = std::format("Error Occurred!\n\nFILE: {}\n\nFUNCTION: {}\n\nLINE: {}\n\nError Code: {}\n\nError: {}",
		sl.file_name(), sl.function_name(), sl.line(), errorCode, reinterpret_cast<char*>(msg));

	std::cout << str << std::endl;
	LocalFree(msg);
}

inline void PopupErrorMessage(int errorCode = ::WSAGetLastError(), const std::source_location& sl = std::source_location::current())
{
	LPVOID msg;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<char*>(&msg),
		0,
		NULL
	);
	
	auto str = std::format("Error Occurred!\n\nFILE: {}\n\nFUNCTION: {}\n\nLINE: {}\n\nError Code: {}\n\nError: {}",
		sl.file_name(), sl.function_name(), sl.line(), errorCode, reinterpret_cast<char*>(msg));

	MessageBoxA(nullptr, str.c_str(), "네트워크 오류", MB_OK | MB_ICONERROR);
	LocalFree(msg);
}

// 03-26: 엔디안 맞추기.
template <typename From> requires std::is_arithmetic_v<From>
static void Serialize(From* value)
{
	// 1바이트 자료는 엔디안 신경쓸 필요 X, 엔디안이 같다면 그냥 써도 상관 X
	if constexpr (NATIVE_ENDIAN == PROTOCOL_ENDIAN or sizeof(From) == sizeof(char)) {
		return;
	}
	else {
		if constexpr (std::is_integral_v<From>) {
			From data{ };

			// From 타입에 맞는 바이트 정렬
			if constexpr (sizeof(From) == sizeof(short)) {
				data = ::htons(*value);
			}
			else if constexpr (sizeof(From) == sizeof(long)) {
				data = ::htonl(*value);
			}
			else if constexpr (sizeof(From) == sizeof(long long)) {
				data = ::htonll(*value);
			}

			::memcpy(value, &data, sizeof(From));
		}
		else if constexpr (std::is_floating_point_v<From>) {
			if constexpr (sizeof(From) == sizeof(float)) {
				unsigned int data = ::htonf(*value);
				::memcpy(value, &data, sizeof(From));
			}
			else if constexpr (sizeof(From) == sizeof(double)) {
				unsigned long long data = ::htond(*value);
				::memcpy(value, &data, sizeof(From));
			}
		}
	}
}

// 03-26 : 역직렬화 함수는 산술 타입의 주소를 인자로 받는다.
//			해당 산술타입의 값을 호스트의 바이트 정렬 방식으로 바꾸어
//			해당 값의 주소에 복사한다.
template <typename To> requires std::is_arithmetic_v<To>
static void Deserialize(To* data)
{
	if constexpr (NATIVE_ENDIAN == PROTOCOL_ENDIAN or sizeof(To) == sizeof(char)) {
		return;
	}
	else {
		if constexpr (std::is_integral_v<To>) {
			To value{ };
			if constexpr (sizeof(To) == sizeof(short)) {
				value = ::ntohs(*data);
			}
			else if constexpr (sizeof(To) == sizeof(long)) {
				value = ::ntohl(*data);
			}
			else if constexpr (sizeof(To) == sizeof(long long)) {
				value = ::ntohll(*data);
			}

			::memcpy(data, &value, sizeof(To));
		}
		else if constexpr (std::is_floating_point_v<To>) {
			To value{ };
			if constexpr (sizeof(To) == sizeof(float)) {
				unsigned int integralData{ };
				::memcpy(&integralData, data, sizeof(To));

				value = ::ntohf(integralData);
			}
			else if constexpr (sizeof(To) == sizeof(double)) {
				unsigned long long integralData{ };
				::memcpy(&integralData, data, sizeof(To));

				value = ::ntohd(integralData);
			}

			::memcpy(data, &value, sizeof(To));
		}
	}
}