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
    static FirstContact CompareContacts(const BodyContact& a, const BodyContact& b);
    FORCEINLINE static s32 CompareContactsForQSort(const void* a, const void* b)
    {
        return static_cast<s32>(CompareContacts(*static_cast<const BodyContact*>(a), *static_cast<const BodyContact*>(b)));
    }
    static void ResolveContact(const BodyContact& contact);
    
    std::vector<Body*> m_Bodies;
};
