#pragma once

#include "Body.h"

class PhysicsScene
{
public:
    void Update();
    void AddBody(Body* body);
    void RemoveBody(Body* body);

    void ClearBodies();
    void ClearDynamicBodies();

    NODISCARD FORCEINLINE u64 GetBodyCount() const { return m_Bodies.size(); }
    
protected:
    static void ResolveContact(const BodyContact& contact);
    
    std::vector<Body*> m_Bodies;
};
