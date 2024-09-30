#include <iostream>
#include <array>

using namespace std;

enum class logLevel
{
    INFO = 0,
    WARNING,
    ERROR
};

class log
{
public:
    static void print(logLevel level, const string &message)
    {
        string color = stringColors[static_cast<int>(level)];
        cout << color << message << "\033[0m" << endl;
    }

private:
    static constexpr array<string, 3> stringColors = {
        "\033[1;32m", // Green
        "\033[1;33m", // Yellow
        "\033[1;31m"  // Red
    };
};