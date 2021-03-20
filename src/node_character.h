#pragma once
#include "mesh.h"
#include "modules/rendering_mode.h"
#include "node.h"
#include "node_model.h"
#include "resources.h"
#include "resources_alphabet.h"
#include "shader.h"
#include "texture.h"
#include "world.h"

/**
 * Member of the scene graph, renders cubes that compose a character.
 * 
 * Note that not all characters are supported.
 * 
 * @see ResourcesAlphabet
 * @see SceneGraph
 */
class NodeCharacter : public Node
{
public:

    NodeCharacter(char character, Shader& shader, float scale = 1.0f)
    {
        this->scale = scale;
        this->shader = &shader;
        auto res = ResourcesAlphabet::getCubes(character);
        if (res != nullptr)
            cubes = res;
    }

    virtual void render(World& world, RenderPass pass, const glm::mat4& matrixStack) override
    {
        if (shader != nullptr)
        {
            world.renderingMode->SetupPolygonMode(renderMode);

            if (pass == RenderPass::Color)
            {
                shader->activate();
                shader->setColor(color);
                if (texture == nullptr)
                    Resources::whiteTexture.useTexture();
                else
                    texture->useTexture();
            }
            else if (pass == RenderPass::Shadow)
            {
                Resources::shadowCastShader.activate();
            }

            // Render the transformed unit cubes that compose the current character
            for (auto transform : *cubes)
            {
                glm::mat4 cubeMatrix = glm::scale(matrixStack, glm::vec3(scale)) * transform;
                if (pass == RenderPass::Color)
                    shader->setModelMatrix(cubeMatrix);
                else if (pass == RenderPass::Shadow)
                    Resources::shadowCastShader.setModelMatrix(cubeMatrix);
                Resources::unitCube.draw();
            }
        }
    }

    const std::vector<glm::mat4>* cubes = nullptr;
    Shader* shader = nullptr;
    Texture* texture = nullptr;
    glm::vec3 color = Resources::colorWhite;
    float scale = 1.0f;
    RenderMode renderMode = RenderMode::Triangle;
};
