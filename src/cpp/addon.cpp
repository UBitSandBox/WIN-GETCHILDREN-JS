//
// Created by ubit-gng on 15.07.2020.
//

#include <Windows.h>
#include <shlwapi.h>

#include "helper/helper.h"

#pragma comment(lib, "shlwapi.lib")

using namespace Napi;

/**
 * Converting Windows FileTime to Unix Time
 * @param ft
 * @return
 */
LONGLONG FileTimeToUnixTime(FILETIME ft) {
    LARGE_INTEGER date, adjust;
    date.HighPart = ft.dwHighDateTime;
    date.LowPart = ft.dwLowDateTime;

    adjust.QuadPart = 11644473600000 * 10000;
    date.QuadPart -= adjust.QuadPart;

    return date.QuadPart / 10000000;
}

/***
 * Creating javascript child object from WIN32_FIND_DATAW
 * @param env
 * @param currentChild
 * @return
 */
Napi::Object CreateChildObject(Napi::Env env, WIN32_FIND_DATAW *currentChild){
    Napi::Object object = Napi::Object::New(env);

    std::wstring fileNameW(currentChild->cFileName);
    std::u16string fileNameU16(fileNameW.begin(), fileNameW.end());

    object.Set(Napi::String::New(env, "fileName"),
            Napi::String::New(env, fileNameU16));
    object.Set(Napi::String::New(env, "isFile"),
            Napi::Boolean::New(env, !(bool)(currentChild->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)));
    object.Set(Napi::String::New(env, "isDirectory"),
            Napi::Boolean::New(env, (bool)(currentChild->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)));
    object.Set(Napi::String::New(env, "size"),
            Napi::Number::New(env, (static_cast<LONGLONG>(currentChild->nFileSizeHigh) << sizeof(currentChild->nFileSizeLow) *8) |currentChild->nFileSizeLow));
    object.Set(Napi::String::New(env, "lastAccessedAt"), Napi::Number::New(env, FileTimeToUnixTime(currentChild->ftLastAccessTime)));
    object.Set(Napi::String::New(env, "lastModifiedAt"), Napi::Number::New(env, FileTimeToUnixTime(currentChild->ftLastWriteTime)));
    object.Set(Napi::String::New(env, "createdAt"), Napi::Number::New(env, FileTimeToUnixTime(currentChild->ftCreationTime)));

    return object;
}

/***
 * Getting Folder Children with attributes as json object
 * @param info
 * @return
 */
Napi::Object GetChildren(const Napi::CallbackInfo &info){
    Napi::Env env = info.Env();

    int length = info.Length();
    if(length != 1 || !info[0].IsString()){
        Napi::TypeError::New(env, "1 String parameter (folderPath) is expected!").ThrowAsJavaScriptException();
    }

    std::wstring folderPath = to_wstring(info[0]);

    WIN32_FIND_DATAW wfd;
    HANDLE hFind;

    wchar_t searchPathBuffer[MAX_PATH] = L"";
    wchar_t *searchPath;
    searchPath = searchPathBuffer;

    PathCombineW(searchPath, folderPath.c_str(), L"*.*");
    hFind = FindFirstFileExW(searchPath, FindExInfoBasic, &wfd,
                             FindExSearchNameMatch, NULL, FIND_FIRST_EX_LARGE_FETCH);

    if(hFind == INVALID_HANDLE_VALUE) {
        createWindowsError(env, GetLastError(), "GetChildren").ThrowAsJavaScriptException();
    }

    Napi::Object object = Napi::Object::New(env);
    int i = 0;

    do
    {
        if(wcscmp(L".", wfd.cFileName) == 0 || wcscmp(L"..", wfd.cFileName) == 0) {
            continue;
        }

        object[i] = CreateChildObject(env, &wfd);
        i++;
    }
    while(FindNextFileW(hFind, &wfd) != 0);

    if(hFind){
        FindClose(hFind);
    }

    DWORD dwError= GetLastError();
    if(dwError != ERROR_NO_MORE_FILES){
        createWindowsError(env, dwError, "GetChildren").ThrowAsJavaScriptException();
    }

    return object;
}

/***
 * Initializing Function GetChildren
 * @param env
 * @param exports
 * @return
 */
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    return Napi::Function::New(env, GetChildren, "getChildren");
}

NODE_API_MODULE(addon, Init)

