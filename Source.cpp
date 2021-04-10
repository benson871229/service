#include<stdio.h>
#include<Windows.h>

int main(int argc, char* argv[])
{
	SC_HANDLE hSCM = OpenSCManagerA(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	if (hSCM == NULL)
	{
		printf("OpenSCManager Error. (%d)\n", GetLastError());
		return EXIT_FAILURE;
	}
	DWORD dwServiceType = SERVICE_DRIVER | SERVICE_WIN32;
	DWORD dwServiceCount = 0;
	DWORD dwSize = 0;
	LPENUM_SERVICE_STATUSA lpInfo = {0};
	BOOL bRet = EnumServicesStatusA(hSCM, dwServiceType, SERVICE_STATE_ALL, NULL, 0, &dwSize, &dwServiceCount, NULL);
	
	if (!bRet && GetLastError() == ERROR_MORE_DATA)
	{
		lpInfo = (LPENUM_SERVICE_STATUSA)malloc(sizeof(ENUM_SERVICE_STATUSA) * dwSize);
		bRet = EnumServicesStatusA(hSCM, dwServiceType, SERVICE_STATE_ALL, (LPENUM_SERVICE_STATUSA)lpInfo, dwSize, &dwSize, &dwServiceCount, NULL);
	}
	if (!bRet)
	{
		printf("EnumServicesStatusA Error. (%d)\n", GetLastError());
		CloseServiceHandle(hSCM);
		return EXIT_FAILURE;
	}
	for (DWORD i = 0; i < dwServiceCount; i++)
	{
		printf("%d:%s %s", i + 1, lpInfo[i].lpServiceName, lpInfo[i].lpDisplayName);
		switch (lpInfo[i].ServiceStatus.dwCurrentState)
		{
		case SERVICE_PAUSED:
			printf("Paused. \n");
			break;
		case SERVICE_STOPPED:
			printf("Stopped.\n");
			break;
		case SERVICE_RUNNING:
			printf("Running.\n");
			break;
		default:
			printf("¯}¸£.\n");
			break;
		}
	}
	free(lpInfo);
	system("pause");
}