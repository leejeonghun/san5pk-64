#include <windows.h>
#include <shlwapi.h>
#include <winnt.h>
#include <cstdint>
#include <cassert>
extern "C" {
#include "wine/winbase16.h"
}

#pragma comment(lib, "shlwapi")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    constexpr int32_t kPebOffset = 0x30;
    constexpr int32_t kAppCompatFlagsUserOffset = 0x1e0;
    constexpr int32_t kAppCompatFlagsUserNoGhost = 0x80000;
    constexpr int32_t kAppCompatFlagsUserNoPaddedBorder = 0x10000000;

    uintptr_t peb = *reinterpret_cast<DWORD*>(reinterpret_cast<BYTE*>(NtCurrentTeb()) + kPebOffset);
    PULARGE_INTEGER appCompatFlagsUser = reinterpret_cast<PULARGE_INTEGER>(peb + kAppCompatFlagsUserOffset);
    assert(appCompatFlagsUser != nullptr);
    appCompatFlagsUser->LowPart |= (kAppCompatFlagsUserNoGhost | kAppCompatFlagsUserNoPaddedBorder);

    char path[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, path, _countof(path));
    PathRemoveFileSpecA(path);
    PathCombineA(path, path, "Sam5.exe");

    if (PathFileExistsA(path) == FALSE) {
        MessageBox(NULL, L"Sam5.exe 파일이 존재하지 않습니다.", L"오류", MB_OK | MB_ICONERROR);
        return -1;
    }

    RestoreThunkLock(1);

    LoadLibrary16("gdi.exe");
    LoadLibrary16("user.exe");
    LoadLibrary16("mmsystem.dll");

    LOADPARAMS16 params = { 0 };
    params.cmdLine = MapLS(lpCmdLine);
    WORD showCmd[] = { 0, (WORD)nShowCmd };
    params.showCmd = MapLS(showCmd);

    if (LoadModule16(path, &params) == NULL) {
        return -1;
    }

    DWORD count = 0;
    ReleaseThunkLock(&count);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
