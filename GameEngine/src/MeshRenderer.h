#pragma once

#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "Component.h"

class MeshRenderer : public Component {
public:
    MeshRenderer(Mesh* mesh, Material* material, Shader* shader)
            : m_Mesh(mesh), m_Material(material), m_Shader(shader) {}

    void Update() override {
        // Implement specific update logic if needed
    }

    void Render() override {
        // Use the shader program
        m_Shader->use();

        // Set the model matrix
        glm::mat4 model = m_Owner->GetTransform()->GetModelMatrix();
        m_Shader->setMat4("model", model);

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
        m_Shader->unuse();
    }

    Shader* GetShader() const { return m_Shader; }
    void SetShader(Shader* shader) { m_Shader = shader; }

    Material* GetMaterial() const { return m_Material; }

private:
    Mesh* m_Mesh;
    Material* m_Material;
    Shader* m_Shader;
};
