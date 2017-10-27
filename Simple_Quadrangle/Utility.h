#ifndef Utility_h
#define Utility_h

//-----これを書かないと作業ディレクトリの表示が出来ない-----
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else

#include <unistd.h>

#define GetCurrentDir getcwd
#endif
//----------

//GLFW内でエラーが発生するした際にメッセージを飛ばす関数
void ErrorCallback(int error, const char *description) {
    std::cerr << "Error: " << description << ".\n";
}

//当プロジェクトの作業ディレクトリを表示する関数
std::string GetCurrentWorkingDir() {
    char buff[FILENAME_MAX];
    GetCurrentDir(buff, FILENAME_MAX);
    std::string currentWorkingDir(buff);
    return currentWorkingDir;
}

#endif /* Utility_h */
