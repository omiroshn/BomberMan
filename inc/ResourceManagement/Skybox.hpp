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
    Skybox(Skybox const &);
    Skybox &operator=(Skybox const &);

    /*draw*/
    void generate(std::vector<unsigned char *> aFacesData, std::vector<std::pair<int, int>> textureSizes);
    void draw(std::shared_ptr<Shader> aShader);
    unsigned int mCubeMap;
private:
    //unsigned int loadCubemap(std::vector<std::string> faces);
    unsigned int mVAO, mVBO;

};

#endif