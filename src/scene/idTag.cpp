#include <chrono>
#include <iostream>
#include <mutex>
#include <sstream>
#include <stdexcept>

#include "scene/idTag.hpp"

std::unordered_set<std::string> IdTag::s_used_ids;
std::mutex IdTag::s_uid_lock;

IdTag::IdTag()
{
    GenerateNewUid();
}

IdTag::IdTag(const std::string& uid)
{
    SetUid(uid);
}

IdTag::~IdTag()
{
    IdTag::RemoveId(m_uid);
}

const std::string& IdTag::GetUid()
{
    return m_uid;
}

void IdTag::SetUid(const std::string& uid)
{
    if (!IdTag::IsUidTaken(uid))
    {
        m_uid = uid;
        IdTag::AddId(m_uid);
    }
    else
    {
        throw std::invalid_argument("Unique ID [" + uid + "] already taken");
    }
}

void IdTag::GenerateNewUid()
{
    auto now = std::chrono::system_clock::now();
    std::stringstream ss{"object_"};
    ss << now.time_since_epoch().count();

    SetUid(ss.str());
}

bool IdTag::IsUidTaken(const std::string &uid)
{
    s_uid_lock.lock();
    bool contains = s_used_ids.contains(uid);
    s_uid_lock.unlock();
    return contains;
}

void IdTag::AddId(const std::string& uid)
{
    s_uid_lock.lock();
    s_used_ids.insert(uid);
    s_uid_lock.unlock();
}

void IdTag::RemoveId(const std::string& uid)
{
    s_uid_lock.lock();
    s_used_ids.erase(uid);
    s_uid_lock.unlock();
}