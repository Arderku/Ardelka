#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>

class Texture {
public:
    // Existing constructor that loads a texture from a file
    Texture(const std::string& path);

    // New constructor that creates a Texture from an existing OpenGL texture ID
    Texture(GLuint id);

    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind(unsigned int slot = 0) const;

    inline unsigned int GetID() const { return m_ID; }

private:
    unsigned int m_ID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
};

#endif // TEXTURE_H
