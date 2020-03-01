#pragma once

class IRenderObject;

class RenderMachine{
public:
    RenderMachine();
    ~RenderMachine();

    void Render();

    void AddRenderObject( IRenderObject* ro );
    void RemoveRenderObject( IRenderObject* ro );

private:
    std::vector<IRenderObject*> roArray;
};

