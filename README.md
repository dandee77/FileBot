# FileBot - Backend

A simple automatic file sorter built using C++ and Raylib. This program organizes files into different categories like images, audio, video, and documents, based on their extensions.

## Features
- Quicly sort files with just one click!
- wala pa
- wala parin
- wala den tngina hsahha

## Installation 

### Prerequisites
- C++ compiler (GCC, Clang, or MSVC)
- Raylib library
- Visual Studio Code (VSCode) with the following extensions:
  - C++ Tools
  - Makefile Tools

### Steps

#### 1. Clone the repository:
   ```bash
   git clone https://github.com/dandee77/FileBot.git
   ```

#### 2. Install Raylib:
   - On Linux, you can install it using:
   ```bash
   sudo apt-get install libraylib-dev
   ```
   - On Windows, download Raylib from [here](https://www.raylib.com) and follow the setup instructions for Visual Studio Code.

#### 3. Install required C++ build tools (if not installed):
   - For Linux:
   ```bash
   sudo apt-get install build-essential
   ```
   - For Windows:
     - Install MSVC or MinGW from [here](https://visualstudio.microsoft.com/visual-cpp-build-tools).

#### 4. Build the project:
   - Open the project folder in VSCode by running the `main.code-workspace`.
   - Configure and build the project using the provided `Makefile` or directly from VSCode.
   - In VSCODE, Press `Ctrl+Shift+B` or `F5`
  
## Windows Specific Instructions
  - Ensure Raylib DLLs (`libgcc_s_dw2-1.dll` and `libstdc++-6.dll`) are correctly placed in the `lib` folder and available during runtime.

## Usage
  - Move the `FileBot.exe` to the directory that you want to be sorted.
  - Run the executable
  - Done!

## Contributing
Feel free to fork this repository, make your changes, and submit a pull request.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
