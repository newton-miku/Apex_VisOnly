/*******************************
 ������
	1.mac:162387C5B7F7����̫�����ء�����16:23:87:C5:B7:F7#34:23:87:C5:B7:F7#E0:DB:55:B5:9C:16
	2.harddiskӲ�����кţ�3W0PKVLE
	3.harddisk�� mac�ϲ�Ϊmachine_code��3W0PKVLE16:23:87:C5:B7:F7
	4.machine_codeȥð�ţ�machine_code��3W0PKVLE162387C5B7F7
	5.��ȡmachine_code����λ��oddnumber��30KL328CBF
	6.����λ����: Key:996837194
	7.ע�����
	8.��¼����
  ��������main()
********************************/
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <winsock2.h>
#include <Iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "Iphlpapi.lib")
#include <locale.h>
#include <tchar.h>
#include <windows.h>
#include "QR_Verify.hpp"

std::string Path;

// 1.1 MAC��������̫�������ء�����
void byte2Hex(unsigned char bData, unsigned char hex[])
{
	int high = bData / 16, low = bData % 16;
	hex[0] = (high < 10) ? ('0' + high) : ('A' + high - 10);
	hex[1] = (low < 10) ? ('0' + low) : ('A' + low - 10);
}
// 1.2 ��ȡ����MAC��ַ 
int getLocalMac(unsigned char* mac)
{
	ULONG ulSize = 0;
	PIP_ADAPTER_INFO pInfo = NULL;
	int temp = 0;
	temp = GetAdaptersInfo(pInfo, &ulSize);//��һ�ε��ã���ȡ��������С
	pInfo = (PIP_ADAPTER_INFO)malloc(ulSize);
	temp = GetAdaptersInfo(pInfo, &ulSize);

	int iCount = 0;
	while (pInfo)//����ÿһ������
	{
		//  pInfo->Address ��MAC��ַ
		for (int i = 0; i < (int)pInfo->AddressLength; i++)
		{
			byte2Hex(pInfo->Address[i], &mac[iCount]);
			iCount += 2;
			if (i < (int)pInfo->AddressLength - 1)
			{
				mac[iCount++] = ':';
			}
			else
			{
				mac[iCount++] = '#';
			}
		}
		pInfo = pInfo->Next;
	}
	if (iCount > 0)
	{
		mac[--iCount] = '\0';
		return iCount;
	}
	else return -1;
}

// 2.��ȡharddisk
// 2.1 harddisk����
char* flipAndCodeBytes(const char* str, int pos, int flip, char* buf)
{
	int i;
	int j = 0;
	int k = 0;

	buf[0] = '\0';
	if (pos <= 0)
		return buf;

	if (!j)
	{
		char p = 0;
		// First try to gather all characters representing hex digits only.
		j = 1;
		k = 0;
		buf[k] = 0;
		for (i = pos; j && str[i] != '\0'; ++i)
		{
			char c = tolower(str[i]);

			if (isspace(c))
				c = '0';
			++p;
			buf[k] <<= 4;

			if (c >= '0' && c <= '9')
				buf[k] |= (unsigned char)(c - '0');
			else if (c >= 'a' && c <= 'f')
				buf[k] |= (unsigned char)(c - 'a' + 10);
			else
			{
				j = 0;
				break;
			}
			if (p == 2)
			{
				if (buf[k] != '\0' && !isprint(buf[k]))
				{
					j = 0;
					break;
				}
				++k;
				p = 0;
				buf[k] = 0;
			}
		}
	}

	if (!j)
	{
		// There are non-digit characters, gather them as is.
		j = 1;
		k = 0;
		for (i = pos; j && str[i] != '\0'; ++i)
		{
			char c = str[i];

			if (!isprint(c))
			{
				j = 0;
				break;
			}
			buf[k++] = c;
		}
	}

	if (!j)
	{
		// The characters are not there or are not printable.
		k = 0;
	}

	buf[k] = '\0';

	if (flip)
		// Flip adjacent characters
		for (j = 0; j < k; j += 2)
		{
			char t = buf[j];
			buf[j] = buf[j + 1];
			buf[j + 1] = t;
		}

	// Trim any beginning and end space
	i = j = -1;
	for (k = 0; buf[k] != '\0'; ++k)
	{
		if (!isspace(buf[k]))
		{
			if (i < 0)
				i = k;
			j = k;
		}
	}

	if ((i >= 0) && (j >= 0))
	{
		for (k = i; (k <= j) && (buf[k] != '\0'); ++k)
			buf[k - i] = buf[k];
		buf[k - i] = '\0';
	}
	return buf;
}
// 2.2 ��ȡharddisk
ULONG GetHDSerial(PCHAR pszIDBuff, int nBuffLen, int nDriveID)
{
	HANDLE hPhysicalDrive = INVALID_HANDLE_VALUE;
	ULONG ulSerialLen = 0;
	__try
	{
		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		TCHAR szDriveName[32];
		wsprintf(szDriveName, TEXT("\\\\.\\PhysicalDrive%d"), nDriveID);

		//  Windows NT, Windows 2000, Windows XP - admin rights not required
		hPhysicalDrive = CreateFile(szDriveName, 0,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);
		if (hPhysicalDrive == INVALID_HANDLE_VALUE)
		{
			__leave;
		}
		STORAGE_PROPERTY_QUERY query;
		DWORD cbBytesReturned = 0;
		static char local_buffer[10000];

		memset((void*)&query, 0, sizeof(query));
		query.PropertyId = StorageDeviceProperty;
		query.QueryType = PropertyStandardQuery;

		memset(local_buffer, 0, sizeof(local_buffer));

		if (DeviceIoControl(hPhysicalDrive, IOCTL_STORAGE_QUERY_PROPERTY,
			&query,
			sizeof(query),
			&local_buffer[0],
			sizeof(local_buffer),
			&cbBytesReturned, NULL))
		{
			STORAGE_DEVICE_DESCRIPTOR* descrip = (STORAGE_DEVICE_DESCRIPTOR*)&local_buffer;
			char serialNumber[1000];

			flipAndCodeBytes(local_buffer,
				descrip->SerialNumberOffset,
				1, serialNumber);

			if (isalnum(serialNumber[0]))
			{
				ULONG ulSerialLenTemp = strnlen(serialNumber, nBuffLen - 1);
				memcpy(pszIDBuff, serialNumber, ulSerialLenTemp);
				pszIDBuff[ulSerialLenTemp] = NULL;
				ulSerialLen = ulSerialLenTemp;
				__leave;
			}

		}
	}
	__finally
	{
		if (hPhysicalDrive != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hPhysicalDrive);
		}
		return ulSerialLen;
	}
}

char  machine_code[100];  // ������ ���ӵ�Ӳ�����кź�mac

// 3. ��ȡmac��harddisk ������
void GetAllHDSerial(void)
{

	// harddisk���к����
	const int MAX_IDE_DRIVES = 16;
	static char szBuff[0x100];
	for (int nDriveNum = 0; nDriveNum < MAX_IDE_DRIVES; nDriveNum++)
	{
		ULONG ulLen = GetHDSerial(szBuff, sizeof(szBuff), nDriveNum);

		/****************
		if (ulLen > 0)
		{
			_tprintf(TEXT("��%d��Ӳ�̵����к�Ϊ��%hs\n"), nDriveNum + 1, szBuff);
		}
		*****************/


	}

	// mac���
	unsigned char address[1024];
	/*****************
	if (getLocalMac(address)>0)
	{
		printf("MAC��%s\n", address);
	}
	******************/

	if (getLocalMac(address) <= 0)
	{
		printf("invoke getMAC error!\n");
	}


	// �����ַ���
	static char szBuff1[64];
	strncat(szBuff1, szBuff, 8);    // ȡszBuffǰ8�ִ��ӵ�szBuff1ĩβ
	//printf("%s\n", szBuff1);

	strncat(szBuff1, (const char*)address, 17);  // ȡmacǰ17λ���ӵ�szBuff1ĩβ
	//printf(szBuff);
	strcpy(machine_code, szBuff1);  // szBuff1��ֵ��machine_code������
	//printf(machine_code);

}

// 4. ȥ��ð��
void del_chr(char* s, char ch)
{
	char* t = s; //Ŀ��ָ����ָ��ԭ��ͷ
	while (*s != '\0') //�����ַ���s
	{
		if (*s != ch) //�����ǰ�ַ�����Ҫɾ���ģ��򱣴浽Ŀ�괮��
			*t++ = *s;
		s++; //�����һ���ַ�
	}
	*t = '\0'; //��Ŀ�괮��������
}

// 5. ��ȡ�ַ�������λ
char oddnumber[30];
void odd(char a[30])
{
	int i, j = 0;

	for (i = 0; i < strlen(a); i += 2)
		oddnumber[j++] = a[i];
	oddnumber[j] = '\0';
	//printf("oddnumber:%s\n", oddnumber);
}
// 6.1 ����λ����
long StrToInt(char* s)  // ��һ���ַ���ת��Ϊ����
{
	long number = 0;
	while (*s != '\0')
	{
		number = number * 10 + *s - '0';
		s++;
	}
	return number;
}
// 6.2 ����λ����
char Key[30];
void encrypt(char a[20])
{
	int num_Key = 0;
	num_Key = StrToInt(a);
	// ȡ��
	if (num_Key < 0)
	{
		num_Key = -num_Key;
	}

	_itoa(num_Key, Key, 10);  //������num_Keyת���ַ���str_Key
	//printf("Key: %s\n", Key);

}

// 7.ע�����
void regist(void)
{
	// ��ʾ����ע����
	char input_Key[30];
	char incout[] = "������ע����input_Key��";
	//printf("%s", incout);
	//scanf("%s", input_Key);
	//printf("please input code:%s\n", code);

	// ��ȡ��������ܳ�ע����
	//char encode[] = "asd";
	//printf("got encode:%s\n", encode);

	// �Ƚ������ַ����Ƿ�һ��
	//if (strcmp(Key, input_Key) == 0)  // ��һ�£��½���data.txt ��д��ע���룬���г���
	if( QR_Verify(Path) == 0 )
	{
		FILE* fp = fopen("data.txt", "w");
		fprintf(fp, "%s ", Key);
		fclose(fp); //�ر��ļ�

		char succe[] = "��ӭ����";
		printf("%s\n", succe);

	}
	else  //����һ�£���ʾ��һ�£������µ���ע�����
	{
		char fail[] = "��֤ʧ�ܣ���������֤";
		//printf("%s\n", fail);
		regist();
	}
}

// 8.��¼����
int checkAuthored()
{
	// 2.1 ��ȡ��������ܳ�ע����
	//char encode[] = "asd";
	//printf("got encode:%s\n", encode);

	FILE* fp = fopen("data.txt", "r");
	char  filekey[30];
	// 2.2 ����ע���ļ�������ע�����
	if ((fp = fopen("data.txt", "r")) == NULL)
	{
		printf("%s\n", "��δע��");
		char fail[] = "����ϵ����Ա������ɨ�����ע�ᣡ";
		printf("%s\n ", fail);
		printf("��֤��Ϊ��%s\n", oddnumber);
		regist();
		return checkAuthored();
	}
	// 2.3 ���ļ�Ϊ�գ�����ע�����
	if (fgets(filekey, 30, fp) == NULL)
	{
		printf("%s", "file empty");
		char fail[] = "ע���ļ�ȱʧ������������ע���룡";
		printf("%s\n", fail);
		printf("%s\n", oddnumber);
		regist();
		return checkAuthored();
	}
	// 2.4 ���ļ��ǿգ�ȡ��ע�������»�õ�ע����Ƚϣ�һ�£�success����һ�£�����ע�����
	else
	{
		filekey[strlen(filekey) - 1] = '\0';   // ȥ��ȡ��ע�����ĩβ�ַ�
	//	int n, m;
	//	n = strlen(filekey);
	//	m = strlen(Key);
	//	printf("%d,%d\n", n, m);
	//	printf("%s\n", filekey); //���ע���ļ�����
		if (strcmp(Key, filekey) == 0)
		{
			char succe[] = "��ӭ����";
			printf("%s\n", succe);
			return 1;
		}
		else
		{
			printf("%s", "file exist");
			char fail[] = "ע���ļ�������ע���벻һ�£�������ɨ��ע��!";
			printf("%s\n", fail);
			printf("%s\n ", oddnumber);
			regist();
		}
	}


	fclose(fp); //�ر��ļ�
}


// ������
int verify(std::string path)
{
	Path = path;
	setlocale(LC_ALL, "chs");
	GetAllHDSerial();  // ��ȡmac��harddisk��ƴ�ӳɻ�����
	del_chr(machine_code, ':');  // ɾ���ַ����ڵ�ð��
	//printf("machine code:%s\n", machine_code); // ����õĻ�����
	odd(machine_code); // oddnumber  ����������λ
	//scanf("%s", oddnumber);
	encrypt(oddnumber);// ���ܺ��ע���� key

	if (checkAuthored() == 1)// ��¼����
		return 1;
	else return -1;
}