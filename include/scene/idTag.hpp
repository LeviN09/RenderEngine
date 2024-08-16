#pragma once

#include <mutex>
#include <string>
#include <unordered_set>

class IdTag
{
    public:
        IdTag();
        IdTag(const std::string& uid);
        virtual ~IdTag();

        const std::string& GetUid();
        void SetUid(const std::string& uid);
        void GenerateNewUid();

        static bool IsUidTaken(const std::string& uid);
    
    protected:
        std::string m_uid;

    private:
        static void AddId(const std::string& uid);
        static void RemoveId(const std::string& uid);

        static std::unordered_set<std::string> s_used_ids;
        static std::mutex s_uid_lock;
};