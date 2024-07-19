#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Component.h"
#include <memory>
#include <iostream>

class MeshRenderer : public Component {
public:
    MeshRenderer(Mesh* mesh, Material* material)
            : m_Mesh(mesh), m_Material(material) {}

    void Render() override {


        if (!m_Mesh || !m_Material) {
            std::cerr << "MeshRenderer: Mesh or Material is null!" << std::endl;
            return;
        }

        m_Material->bind();

        //check mowner and get transform
        if (!m_Owner) {
            std::cerr << "MeshRenderer: Owner is null!" << std::endl;
            return;
        }
        glm::mat4 model =  m_Owner->GetTransform()->GetGlobalModelMatrix();

        //check shader is valid
        if (!m_Material->GetShader()->IsValid()) {
            std::cerr << "MeshRenderer: Shader is not valid!" << std::endl;
            return;
        }
        m_Material->GetShader()->setMat4("model", model);


        m_Mesh->Bind();

        glDrawElements(GL_TRIANGLES, m_Mesh->GetIndexCount(), GL_UNSIGNED_INT, 0);

        m_Mesh->Unbind();
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "OpenGL Error after glDrawElements: " << error << std::endl;
        }

        m_Material->GetShader()->unuse();
    }

    void Update() override {

    }

private:
    Mesh* m_Mesh;
    Material* m_Material;
};
