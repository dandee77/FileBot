#include "filebot.h"
#include "log.h"

int main()
{
    filebot app("src/filetypes.json");
    // app.checkEnvironmentVariables();
    app.run();
    return 0;
}