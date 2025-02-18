#pragma once

#include "Mesh.h"
#include "Material.h"
#include "Component.h"
#include <memory>

class MeshRenderer : public Component {
public:
    MeshRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
            : m_Mesh(mesh), m_Material(material) {}

    void Update() override {
        // Implement specific update logic if needed
    }

    void Render() override {
        // Use the shader program from the material
        auto shader = m_Material->GetShader();
        shader->use();

        // Set the model matrix
        glm::mat4 model = m_Owner->GetTransform()->GetModelMatrix();
        shader->setMat4("model", model);

        // Bind the material
        m_Material->Bind();

        // Bind the mesh
        m_Mesh->Bind();

        // Draw the mesh
        glDrawElements(GL_TRIANGLES, m_Mesh->GetIndexCount(), GL_UNSIGNED_INT, 0);

        // Unbind the mesh
        m_Mesh->Unbind();

        // Unbind the material
        m_Material->Unbind();

        // Unuse the shader program
        shader->unuse();
    }

    std::shared_ptr<Material> GetMaterial() const { return m_Material; }

    void SetMaterial(std::shared_ptr<Material> sharedPtr) {
        m_Material = sharedPtr;

    }

    inline void RenderShadow(const Shader& shadowShader) {
        // Here, use the shadowShader (passed as parameter) instead of the material's shader.
        // Set the model matrix using your GameObject's transform.
        glm::mat4 model = m_Owner->GetTransform()->GetModelMatrix();
        shadowShader.setMat4("model", model);

        // Bind the mesh and draw it.
        m_Mesh->Bind();
        glDrawElements(GL_TRIANGLES, m_Mesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
        m_Mesh->Unbind();
    }


private:
    std::shared_ptr<Mesh> m_Mesh;
    std::shared_ptr<Material> m_Material;
};
