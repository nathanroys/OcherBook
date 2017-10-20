#include "controller.h"
#include "logger.h"
#include "options.h"

void Controller::initLog()
{
    static LogAppenderCFile appender(stderr);
    Logger* l = Log::get("");

    l->setAppender(&appender);

    if (m_options->verbose < 0)
        l->setLevel(Log::Fatal);
    else if (m_options->verbose == 0)
        l->setLevel(Log::Error);
    else if (m_options->verbose == 1)
        l->setLevel(Log::Warn);
    else if (m_options->verbose == 2)
        l->setLevel(Log::Info);
    else if (m_options->verbose == 3)
        l->setLevel(Log::Debug);
    else
        l->setLevel(Log::Trace);
    Log::info("ocher", "Log initialized");
}

Controller::Controller(Options* options)
    : m_options(options)
{
    initLog();
}

Controller::~Controller()
{
}
