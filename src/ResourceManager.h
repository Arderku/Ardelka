#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Texture.h"
#include <string>
#include <unordered_map>

class ResourceManager {
public:
    static Texture* loadTexture(const std::string& name, const std::string& path);
    static Texture* getTexture(const std::string& name);

    static void clear();

private:
    ResourceManager() {}
    static std::unordered_map<std::string, Texture*> m_Textures;
};

#endif // RESOURCEMANAGER_H
