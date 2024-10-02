#include "filebot.h"
#include "window.h"

int main()
{
    // filebot app("src/filetypes.json");
    //  app.checkEnvironmentVariables();
    // app.run();
    Window app = Window(800, 450, "Filebot");
    app.run();
    return 0;
}