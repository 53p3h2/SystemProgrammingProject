#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <fstream>
#include <string>
#include <commdlg.h>

std::string ConvertToRegularString(const std::wstring& wideString) {
	//passing null as lpMultiByteStr and 0 as buffer size it calculate the needed buffer size
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), -1, NULL, 0, NULL, NULL);
	std::string regularString(bufferSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), -1, &regularString[0], bufferSize, NULL, NULL);
	return regularString;
}

std::wstring GetSaveFilePath() {
	wchar_t fileName[MAX_PATH] = L"";

	OPENFILENAMEW ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = L"CSV Files (*.csv)\0*.csv\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt = L"csv";
	GetSaveFileNameW(&ofn);
	return std::wstring(fileName);
}

int main() {
	std::wstring fileName = GetSaveFilePath();
	std::ofstream csvFile(fileName);
	csvFile << "PID,Process Name\n";
	HANDLE processesSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	while (Process32Next(processesSnap, &pe)) {
		std::wcout << L"Process Name: " << pe.szExeFile << L", PID: " << pe.th32ProcessID << std::endl;
		csvFile << pe.th32ProcessID << "," << ConvertToRegularString(pe.szExeFile) << "\n";
	}
	CloseHandle(processesSnap);
	csvFile.close();
	return 0;
}
