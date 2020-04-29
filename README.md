# Introduction
I've made this program for testing purpose in RPG Maker MV. I wanted to create a standard alone program of small capacity instead of Python or Node.js. This program allows you to create the child process and create the package.json file if it didn't exist it.

To build this program, you must install Visual Studio and CMake and you'll need to copy my repository into a certain directory on your computer.

```cmd
cmake CMakeLists.txt
```

and Opens your Visual Studio's Developer Console or Visual Studio and Try to run below command.

```cmd
"C:\Program Files (x86)\MSBuild\14.0\Bin\msbuild.exe" Project.sln /p:Configuration=Release
```

You can execute a main.exe file after compiling source files.

```cmd
main.exe <Game_FOLDER> <NWJS_FOLDER>
main.exe E:\Games\201907\ C:\Users\U\Desktop\MV\Windows\ToolManager\lib\v0.45.4\nwjs-sdk-v0.45.4-win-x64
```