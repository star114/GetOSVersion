// GetOSVersion.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Windows.h>

void GetOSVersion_Normal()
{
	OSVERSIONINFOEX versioninfo;
	versioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if (::GetVersionExW((LPOSVERSIONINFO)&versioninfo))
	{
		printf("MajorVersion : %d , MinorVersion : %d, ProductType : %d, ServicePack : %d.%d\n", versioninfo.dwMajorVersion, versioninfo.dwMinorVersion, versioninfo.wProductType, versioninfo.wServicePackMajor, versioninfo.wServicePackMinor);
		printf("\nMeaning of product type\n");
		printf("0 - UNKNOWN\n1 - VER_NT_WORKSTATION\n2 - VER_NT_DOMAIN_CONTROLLER\n3 - VER_NT_SERVER\n");
	}
}

BOOL GetOSVersion_KnownVersion()
{
	int iVersion = 0;

	const int VERSION_N = 9;
	int VersionTable[VERSION_N] = { 50, 51, 52, 60, 61, 62, 63, 64, 100 };
	// 2K XP 2K3 Vista 7 8 8.1 10

	// osVersionInfoToCompare contains the OS version requirements to compare 
	OSVERSIONINFOEX osVersionInfoToCompare = { sizeof(OSVERSIONINFOEX) };

	// Initialize the condition mask with ULONGLONG VER_SET_CONDITION( 
	// ULONGLONG dwlConditionMask, DWORD dwTypeBitMask, BYTE dwConditionMask) 
	ULONGLONG comparisonInfo = 0;
	BYTE conditionMask = VER_EQUAL;
	VER_SET_CONDITION(comparisonInfo, VER_MAJORVERSION, conditionMask);
	VER_SET_CONDITION(comparisonInfo, VER_MINORVERSION, conditionMask);

	for (int i = 0; i < VERSION_N; i++) {

		osVersionInfoToCompare.dwMajorVersion = VersionTable[i] / 10;
		osVersionInfoToCompare.dwMinorVersion = VersionTable[i] % 10;

		if (VerifyVersionInfo(&osVersionInfoToCompare, VER_MAJORVERSION |
			VER_MINORVERSION,
			comparisonInfo))
		{
			printf("MajorVersion : %d, MinorVersion : %d\n", osVersionInfoToCompare.dwMajorVersion, osVersionInfoToCompare.dwMinorVersion);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL EqualsMajorVersion(DWORD majorVersion)
{
	OSVERSIONINFOEX osVersionInfo;
	::ZeroMemory(&osVersionInfo, sizeof(OSVERSIONINFOEX));
	osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	osVersionInfo.dwMajorVersion = majorVersion;
	ULONGLONG maskCondition = ::VerSetConditionMask(0, VER_MAJORVERSION, VER_EQUAL);
	return ::VerifyVersionInfo(&osVersionInfo, VER_MAJORVERSION, maskCondition);
}

BOOL EqualsMinorVersion(DWORD minorVersion)
{
	OSVERSIONINFOEX osVersionInfo;
	::ZeroMemory(&osVersionInfo, sizeof(OSVERSIONINFOEX));
	osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	osVersionInfo.dwMinorVersion = minorVersion;
	ULONGLONG maskCondition = ::VerSetConditionMask(0, VER_MINORVERSION, VER_EQUAL);
	return ::VerifyVersionInfo(&osVersionInfo, VER_MINORVERSION, maskCondition);
}

BOOL EqualsServicePack(DWORD servicePackMajor)
{
	OSVERSIONINFOEX osVersionInfo;
	::ZeroMemory(&osVersionInfo, sizeof(OSVERSIONINFOEX));
	osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	osVersionInfo.wServicePackMajor = (WORD)servicePackMajor;
	ULONGLONG maskCondition = ::VerSetConditionMask(0, VER_SERVICEPACKMAJOR, VER_EQUAL);
	return ::VerifyVersionInfo(&osVersionInfo, VER_SERVICEPACKMAJOR, maskCondition);
}

BOOL EqualsProductType(BYTE productType)
{
	OSVERSIONINFOEX osVersionInfo;
	::ZeroMemory(&osVersionInfo, sizeof(OSVERSIONINFOEX));
	osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	osVersionInfo.wProductType = productType;
	ULONGLONG maskCondition = ::VerSetConditionMask(0, VER_PRODUCT_TYPE, VER_EQUAL);
	return ::VerifyVersionInfo(&osVersionInfo, VER_PRODUCT_TYPE, maskCondition);
}

BOOL IsWindowsVersionOrGreater(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor)
{
	OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0, { 0 }, 0, 0 };
	DWORDLONG        const dwlConditionMask = VerSetConditionMask(
		VerSetConditionMask(
		VerSetConditionMask(
		0, VER_MAJORVERSION, VER_GREATER_EQUAL),
		VER_MINORVERSION, VER_GREATER_EQUAL),
		VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);

	osvi.dwMajorVersion = wMajorVersion;
	osvi.dwMinorVersion = wMinorVersion;
	osvi.wServicePackMajor = wServicePackMajor;

	return VerifyVersionInfoW(&osvi, VER_MAJORVERSION |
		VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask) != FALSE;
}

BYTE GetProductType()
{
	if (EqualsProductType(VER_NT_WORKSTATION))
	{
		return VER_NT_WORKSTATION;
	}
	else if (EqualsProductType(VER_NT_SERVER))
	{
		return VER_NT_SERVER;
	}
	else if (EqualsProductType(VER_NT_DOMAIN_CONTROLLER))
	{
		return VER_NT_DOMAIN_CONTROLLER;
	}
	return 0;//unkown
}

const DWORD MajorVersion_Start = 5;
const DWORD MinorVersion_Start = 0;
const WORD ServicePackVersion_Start = 0;

const DWORD MajorVersion_Max = 10;
const DWORD MinorVersion_Max = 5;
const WORD ServicePackVersion_Max = 4;

BOOL GetOSVersion_UnknownVersion()
{
	DWORD minorVersionCounterSeed = MinorVersion_Start;
	DWORD servicePackCounterSeed = ServicePackVersion_Start;
	//by design, if we can't find the right service pack we will return true;
	for (DWORD majorVersion = MajorVersion_Start; majorVersion <= MajorVersion_Max; majorVersion++)
	{
		if (EqualsMajorVersion(majorVersion))
		{
			for (DWORD minorVersion = minorVersionCounterSeed;
				minorVersion <= MinorVersion_Max; minorVersion++)
			{
				if (EqualsMinorVersion(minorVersion))
				{
					DWORD servicePack;
					for (servicePack = servicePackCounterSeed;
						servicePack <= ServicePackVersion_Max; servicePack++)
					{
						if (EqualsServicePack(servicePack))
						{
							break;
						}
					}

					printf("UNKNOWN Version * MajorVersion : %d , MinorVersion : %d, ProductType : %d, ServicePack : %d.%d\n", majorVersion, minorVersion, GetProductType(), servicePack);
					printf("\nMeaning of product type\n");
					printf("0 - UNKNOWN\n1 - VER_NT_WORKSTATION\n2 - VER_NT_DOMAIN_CONTROLLER\n3 - VER_NT_SERVER\n");
					return true;
				}
				else
				{
					//reset servicepack version counter to 0 because
					//we are going to increment our minor version.
					servicePackCounterSeed = 0;
				}
			}
		}
		else
		{
			//reset minor version to start from 0 because we are going to increment majorVersion;
			minorVersionCounterSeed = 0;
		}
	}
	return false;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 2)
	{
		printf("*************************************************\n");
		printf("*                                               *\n");
		printf("* GetOSVersion                                  *\n");
		printf("*                                               *\n");
		printf("* GetOSVersion.exe TypeNumber                   *\n");
		printf("*                                               *\n");
		printf("* TypeNumber                                    *\n");
		printf("* 1 : Using GetVersionEx                        *\n");
		printf("* 2 : Using VerifyVersionInfo Known OSVersion   *\n");
		printf("* 3 : Using VerifyVersionInfo UnKnown OSVersion *\n");
		printf("*                                               *\n");
		printf("*************************************************\n");
		return -1;
	}
	int nType = _wtoi(argv[1]);

	switch (nType)
	{
	case 1:
		GetOSVersion_Normal();
		break;
	case 2:
		GetOSVersion_KnownVersion();
		break;
	case 3:
		GetOSVersion_UnknownVersion();
		break;
	default:
		printf("type number [%d] is not defined.\n", nType);
		break;
	}
	return 0;
}

