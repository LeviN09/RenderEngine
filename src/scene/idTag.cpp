#include <chrono>
#include <cmath>
#include <sstream>
#include <stdexcept>

#include "scene/idTag.hpp"

std::unordered_set<std::string> IdTag::s_used_ids;

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
    ss << std::chrono::duration<double_t>(now.time_since_epoch());

    SetUid(ss.str());
}

bool IdTag::IsUidTaken(const std::string &uid)
{
    return s_used_ids.contains(uid);
}

void IdTag::AddId(const std::string& uid)
{
    s_used_ids.insert(uid);
}

void IdTag::RemoveId(const std::string& uid)
{
    s_used_ids.erase(uid);
}