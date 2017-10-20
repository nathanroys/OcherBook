#include <memory>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "logger.h"

using namespace std;

Loggers Log::loggers;

Loggers::Loggers()
{
    m_init = true;
    setRoot();
}

Loggers::~Loggers()
{
    clear();
    m_init = false;
}

void Loggers::clearAppenderUnchecked(LogAppender* logAppender)
{
    for (auto& logger : m_loggers) {
        logger.second->clearAppender(logAppender);
    }
}

void Loggers::clear()
{
    if (m_init) {
        lock_guard<mutex> lock(m_lock);

        // Break ties to all LogAppenders, so that destruction order of statics does not matter.
        clearAppenderUnchecked(0);
        m_loggers.clear();
    }
}

void Loggers::clearAppender(LogAppender* logAppender)
{
    if (m_init) {
        lock_guard<mutex> lock(m_lock);

        clearAppenderUnchecked(logAppender);
    }
}

void Loggers::setRoot()
{
    // Calling setRoot from another static is bad because I can't guarantee ordering.  Asserting
    // because I can't honor the contract.  Just don't do it.
    assert(m_init);
    if (m_init) {
        Logger* root = new Logger(this, nullptr, "");
        root->setLevel(Log::Warn);

        {
            lock_guard<mutex> lock(m_lock);
            m_loggers[root->getName()] = root;
        }
    }
}

Logger* Loggers::get(const char* name)
{
    if (!m_init)
        return (Logger*)0;

    m_lock.lock();
    Logger* logger = m_loggers[name];
    m_lock.unlock();

    if (!logger) {
        unsigned int nameLen = strlen(name);
        const char* end;
        unsigned int searchOffset = 0;
        // Root logger should have been created in setRoot.
        assert(nameLen > 0);

        lock_guard<mutex> lock(m_lock);
        Logger* parent = m_loggers[""];
        assert(parent);
        do {
            unsigned int subnameLen;
            end = strchr(name + searchOffset, '.');
            if (end) {
                subnameLen = end - name;
                searchOffset = subnameLen + 1;
            } else {
                subnameLen = nameLen;
            }
            std::string subname(name, subnameLen);
            logger = m_loggers[subname];
            if (!logger) {
                logger = new Logger(this, parent, subname);
                m_loggers[subname] = logger;
            }
            parent = logger;
        } while (end);
    }
    return logger;
}

std::string getLogDirectory()
{
    // TODO
    return "/tmp";
}

void Log::reset()
{
    loggers.clear();
    loggers.setRoot();
}

Logger* Log::get(const char* name)
{
    return loggers.get(name);
}

void Log::log(const char* name, Log::Level level, const char* fmt, va_list args)
{
    get(name)->log(level, fmt, args);
}

void Log::log(const char* name, Log::Level level, const char* fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    get(name)->log(level, fmt, ap);
    va_end(ap);
}

Logger::Logger(Loggers* loggers, Logger* parent, const std::string& name)
    : m_loggers(loggers)
    , m_parent(parent)
    , m_name(name)
    , m_level(Log::Unset)
{
}

void Logger::setLevel(Log::Level level)
{
    if (m_parent || level != Log::Unset) // Can't unset root logger
        m_level = level;
}

Log::Level Logger::getLevel() const
{
    if (m_level != Log::Unset)
        return m_level;
    return m_parent->getLevel();
}

Logger* Logger::getParent()
{
    return m_parent;
}

void Logger::setAppender(LogAppender* a)
{
    assert(a);

    lock_guard<mutex> lock(m_loggers->m_lock);
    m_appenders.insert(a);
    a->setLoggers(m_loggers);
}

void Logger::clearAppender(LogAppender* logAppender)
{
    if (logAppender) {
        auto iter = m_appenders.find(logAppender);
        if (iter != m_appenders.end())
            m_appenders.erase(iter);
    } else {
        for (auto a : m_appenders) {
            a->setLoggers(0);
        }
        m_appenders.clear();
    }
}

void Logger::append(Log::Level level, std::string const& s)
{
    {
        lock_guard<mutex> lock(m_loggers->m_lock);
        for (auto a : m_appenders) {
            a->append(s);
        }
    }

    if (m_parent)
        m_parent->append(level, s);
}

static const char levelChar[] = { 'T', 'D', 'I', 'W', 'E', 'F' };

static std::string formatList(const char* fmt, va_list argList)
{
    std::string s;
    char* buf;
    int len = vasprintf(&buf, fmt, argList);
    if (len >= 0) {
        s.assign(buf, len);
        free(buf);
    }
    return s;
}

static std::string format(const char* fmt, ...)
{
    va_list argList;
    va_start(argList, fmt);
    std::string s = formatList(fmt, argList);
    va_end(argList);
    return s;
}

void Logger::log(Log::Level level, const char* fmt, va_list ap)
{
    try {
        if (getLevel() <= level) {
            std::string msg(formatList(fmt, ap));
            std::string s(
                format("%c %-10s %s\n", levelChar[level - 1], m_name.c_str(), msg.c_str()));
            append(level, s);
        }
    } catch (...) {
        // Logging is best effort.
    }
}

void Logger::log(Log::Level level, const char* fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    log(level, fmt, ap);
    va_end(ap);
}
