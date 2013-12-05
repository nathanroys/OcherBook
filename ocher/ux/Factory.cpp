#include "ocher/ux/Event.h"
#include "ocher/ux/Factory.h"

EventLoop* g_loop;
FrameBuffer* g_fb;
FreeType* g_ft;


clc::List& getDrivers()
{
    static clc::List drivers;
    return drivers;
}


UiFactory::UiFactory()
{
}

void UiFactory::populate()
{
    // Populate globals for ease of access
    g_fb = uiFactory->getFrameBuffer();
    g_ft = uiFactory->getFontEngine();
    g_loop = new EventLoop;
}
