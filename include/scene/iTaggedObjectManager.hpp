#ifndef ITAGGEDOBJECTMANAGER_CLASS_HPP
#define ITAGGEDOBJECTMANAGER_CLASS_HPP

#include <optional>

#include "scene/idTag.hpp"

class ITaggedObjectManager
{
    public:
        virtual std::optional<std::reference_wrapper<IdTag>> SearchObject(const std::string& uid) const = 0;
};

#endif