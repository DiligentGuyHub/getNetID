#include <iostream>
#include <winsock2.h>
#include <math.h>
#include <Windows.h>
#pragma warning(disable : 4996)
using namespace std;

#define ulong unsigned long

bool CheckAddress(char* IP)
{
	int points = 0, numbers = 0; 
	char* buffer; // буфер для одного октета
	buffer = new char[3];

	for (int i = 0; IP[i] != '\0'; i++)
	{ // для строки IP-адреса
		if (IP[i] <= '9' && IP[i] >= '0') // если цифра
		{
			if (numbers > 3) return false;
			//если больше трех чисел в октете – ошибка
			buffer[numbers++] = IP[i];
			//скопировать в буфер
		}
		else
		{
			if (IP[i] == '.') // если точка
			{
				if (atoi(buffer) > 255)
					// проверить диапазон октета
					return false;
				if (numbers == 0)
					//если числа нет - ошибка
					return false;
				numbers = 0;
				points++;
				delete[]buffer;
				buffer = new char[3];
			}
			else return false;
		}	
	}
	if (points != 3) // если количество точек в IP-адресе не 3 - ошибка
		return false;
	if (numbers == 0 || numbers > 3)
		return false;
	return true;
}

ulong CharToLong(char* IP)
{
	ulong out = 0; //число для IP-адреса
	char* buffer;
	buffer = new char[3];
	//буфер для хранения одного октета
	for (int i = 0, j = 0, k = 0; IP[i] != '\0'; i++, j++)
	{
		if (IP[i] != '.') //если не точка
			buffer[j] = IP[i]; // записать символ в буфер
		if (IP[i] == '.' || IP[i + 1] == '\0')
		{
			// если следующий октет или последний
			out <<= 8; //сдвинуть число на 8 бит
			if (atoi(buffer) > 255)
				return NULL;
			// еcли октет больше 255 – ошибка
			out += (ulong)atoi(buffer);
			//преобразовать и добавить
			//к числу IP-адреса
			k++;
			j = -1;
			delete[]buffer;
			buffer = new char[3];
		}
	}
	return out;
}

void LongToChar(ulong IP)
{
	char Result[16] = {};
	char* Buffer = new char[3];
	for (int i = 3; i >= 0; i--)
	{
		itoa(IP / int(pow(256, i)), Buffer, 10);
		IP = IP % int(pow(256, i));
		strcat_s(Result, Buffer);
		if(i != 0)
			strcat_s(Result, ".");
	}
	cout << Result;
}
//2886976051

bool CheckMask(char* mask)
{
	int Octets[4] = {};
	int Points = 0, Numbers = 0;
	char* buffer = new char[3]{};
	for (int i = 0, k = 0; mask[i] != '\0'; i++)
	{
		if (mask[i] <= '9' && mask[i] >= '0')
		{
			if (Numbers > 3) return false;
			buffer[Numbers++] = mask[i];
		}
		else 
		{
			if (mask[i] == '.')
			{
				if (Numbers = 0)
				{
					return false;
				}
				Octets[k] += atoi(buffer);
				k++;
				Numbers = 0;
				Points++;
				delete[] buffer;
				buffer = new char[3]{};
			}
			else
			{
				return false;
			}
		}
	}
	Octets[3] = atoi(buffer);
	for (int i = 1; i < 4; i++)
	{
		if (Octets[i] > Octets[i - 1]) return false;
		if (Octets[0] != 255 && Octets[i] != 0) return false;
	}
	if (Points != 3) // если количество точек в mask-адресе не 3 - ошибка
		return false;
	if (Numbers == 0 || Numbers > 3)
		return false;
}
//bool CheckMask(char* mask)
//{
//	int points = 0, numbers = 0;
//	char* buffer = new char[3]{};
//	char previous[4]{};
//
//	for (int i = 0; mask[i] != '\0'; i++)
//	{
//		if (mask[i] <= '9' && mask[i] >= '0')
//		{
//			if (numbers > 3) return false;
//			buffer[numbers++] = mask[i];
//		}
//		else
//		{
//			if (mask[i] == '.') 
//			{
//				if (atoi(buffer) > atoi(previous) && i > 4) return false;
//				strncpy_s(previous, buffer, 3);
//				if (atoi(buffer) > 255)
//					return false;
//				if (numbers == 0)
//					//если числа нет - ошибка
//					return false;
//				numbers = 0;
//				points++;
//				delete[]buffer;
//				buffer = new char[3];
//			}
//			else return false;
//		}
//		if (atoi(buffer) > atoi(previous) && i > 4) return false;
//	}
//	if (points != 3) // если количество точек в mask-адресе не 3 - ошибка
//		return false;
//	if (numbers == 0 || numbers > 3)
//		return false;
//	return true;
//}

int main(int argc, char* argv[])
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	setlocale(LC_ALL, "RUS");
	ulong ip, mask, host, subnet, broadcast;
	char* ip_, * mask_;
	bool flag = true;
	ip_ = new char[16];
	mask_ = new char[16];
	do
	{
		if (!flag) cout << "Неверно введён адрес!" << endl;
		cout << "IP:\t\t";
		cin >> ip_;
	} while (!(flag = CheckAddress(ip_)));
	ip = CharToLong(ip_);
	flag = true;
	do
	{
		if (!flag) cout << "Неправильная маска!" << endl;
		flag = true;
		do
		{
			cout << "Маска:\t\t";
			cin >> mask_;
			flag = CheckMask(mask_);
			if (!flag) cout << "Неверно введена маска!" << endl;
		} while (!(flag = CheckAddress(mask_)));
		mask = CharToLong(mask_);
	} while (!(flag = CheckMask(mask_)));
	subnet = ip & mask;
	cout << "ID подсети:\t";
 	LongToChar(subnet);
	host = ip & ~mask;
	cout << "\nID хоста:\t";
	LongToChar(host);
	cout << "\nBroadcast:\t";
	broadcast = ip & mask | ~mask;
	LongToChar(broadcast);
	

}