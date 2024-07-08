#ifndef IDTAG_CLASS_HPP
#define IDTAG_CLASS_HPP

#include <string>
#include <unordered_set>

class IdTag
{
    public:
        IdTag();
        IdTag(const std::string& uid);
        ~IdTag();

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
};

#endif