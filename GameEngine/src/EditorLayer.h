#pragma once
#include "Layer.h"
#include "Editor.h"

class EditorLayer : public Layer {
public:
    // The EditorLayer takes a reference to your existing Editor.
    EditorLayer(Editor& editor) : m_Editor(editor)
    {
        m_Name = "EditorLayer";
    }

    virtual void OnImGuiRender() override {
        m_Editor.Render();
    }

    virtual void OnDetach() override {
        m_Editor.Shutdown();
    }

private:
    Editor& m_Editor;
};
