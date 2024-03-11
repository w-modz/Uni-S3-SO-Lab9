// SO IS1 221A LAB09
// Wiktor Modzelewski
// mw53766@zut.edu.pl
//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <ctype.h>
#include <sys/types.h>
#include <stdbool.h>
#include <windows.h>
#include <processthreadsapi.h>
#include <string.h>

// funkcja pomocnicza do weryfikowania argumentow
bool is_number(const char* str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (isdigit(str[i]) == 0) return false;
    }
    return true;
}


int main(int argc, char* argv[])
{
    // weryfikowanie poprawnej ilosci argumentow
    if (argc != 2)
    {
        fprintf(stderr, "Invalid number of arguments\n");
    }
    // weryfikowanie poprawnosci argumentu
    else if (strlen(argv[1]) > 25 || !is_number(argv[1]))
    {
        fprintf(stderr, "Invalid argument\n");
    }
    // przypadek argumentu w postaci jednej cyfry
    else if (strlen(argv[1]) == 1)
    {
        return atoi(argv[1]);
    }
    else
    {
        const char* executable_path = "C:\\Users\\vitom\\OneDrive\\Dokumenty\\GitHub\\Uni-S3-SO-Lab9\\SO_Lab9\\x64\\Debug\\SO_Lab9.exe ";

        // rodzielanie argumentu na dwie "polowy"
        char* str = argv[1];
        int length = strlen(str);
        int length1 = floor(length / 2);
        int length2 = length - length1;
        char str1[13] = {};
        char str2[13] = {};
        for (int i = 0; i<length;i++)
        {
            if (i<length1)
            {
                str1[i] = str[i];
            }
            else
            {
                str2[i-length1] = str[i];
            }
        }
        // przygotowanie argumentow dla nowych procesow
        char arg1[100];
        char arg2[100];
        strcpy_s(arg1,100, executable_path);
        strcpy_s(arg2, 100, executable_path);
        strcat_s(arg1, 100, str1);
        strcat_s(arg2, 100, str2);
        
        // tworzenie nowych procesow
        PROCESS_INFORMATION process_info1 = { 0 };
        STARTUPINFOA startup_info1 = { 0 };
        startup_info1.cb = sizeof(LPSTARTUPINFO);
        PROCESS_INFORMATION process_info2 = { 0 };
        STARTUPINFOA startup_info2 = { 0 };
        startup_info2.cb = sizeof(LPSTARTUPINFO);
        if (!CreateProcessA(executable_path, arg1, NULL, NULL, TRUE, 0, NULL, NULL, &startup_info1, &process_info1))
        {
            fprintf(stderr, "Error creating process\n");
            return 1;
        }
        if (!CreateProcessA(executable_path, arg2, NULL, NULL, TRUE, 0, NULL, NULL, &startup_info2, &process_info2))
        {
            fprintf(stderr, "Error creating process\n");
            return 1;
        }

        WaitForSingleObject(process_info1.hProcess, INFINITE);
        WaitForSingleObject(process_info2.hProcess, INFINITE);
        
        int exit_code1, exit_code2;
        DWORD new_exitcode;
        GetExitCodeProcess(process_info1.hProcess, &new_exitcode);
        exit_code1 = new_exitcode;
        GetExitCodeProcess(process_info2.hProcess, &new_exitcode);
        exit_code2 = new_exitcode;

        printf("%d  %d  %s %d\n", GetCurrentProcessId(), process_info1.dwProcessId, str1, exit_code1);
        printf("%d  %d  %s %d\n", GetCurrentProcessId(), process_info2.dwProcessId, str2, exit_code2);

        CloseHandle(process_info1.hProcess);
        CloseHandle(process_info2.hProcess);
        CloseHandle(process_info1.hThread);
        CloseHandle(process_info2.hThread);
        return exit_code1 + exit_code2;
    }
}