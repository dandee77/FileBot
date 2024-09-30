#include "filebot.h"
#include "log.h"

int main()
{
    filebot app("src/filetypes.json");
    // app.checkEnvironmentVariables();
    app.selectDirectoryToSort();
    return 0;
}