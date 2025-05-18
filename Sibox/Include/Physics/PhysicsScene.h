#pragma once

#include "Body.h"

class PhysicsScene
{
public:
    void Update();
    void AddBody(Body* body);
    void RemoveBody(Body* body);

    NODISCARD FORCEINLINE u64 GetBodyCount() const { return m_Bodies.size(); }
    
protected:
    std::vector<Body*> m_Bodies;
};
