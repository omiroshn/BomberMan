#ifndef Skybox_H
# define Skybox_H

#include <memory>
#include <string>
#include <vector>
#include "Shader.hpp"

class Skybox
{
public:
    ~Skybox();
    Skybox();
    Skybox(Skybox const &) = delete;
    Skybox &operator=(Skybox const &) = delete;

	void generate(std::vector<unsigned char*> aFacesData, std::vector<std::pair<int, int>> textureSizes, GLenum format);
    void draw(std::shared_ptr<Shader> aShader);
    unsigned int mCubeMap;
private:
    unsigned int mVAO, mVBO;

};

#endif