#pragma once
#include "Body.h"

class PhysicsScene
{
public:
    void Update();
    Body& AddBody(const Body& body);
    void RemoveBody(u32 handle);
    
protected:
    std::vector<Body> m_Bodies;
};
