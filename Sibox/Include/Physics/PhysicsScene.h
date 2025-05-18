#pragma once

#include "Body.h"

class PhysicsScene
{
public:
    void Update();
    void AddBody(Body* body);
    void RemoveBody(Body* body);
    
protected:
    std::vector<Body*> m_Bodies;
};
