#pragma once

#include "Body.h"

struct CollisionPair
{
    s32 BodyAID = -1;
    s32 BodyBID = -1;

    bool operator==(const CollisionPair& other) const
    {
        return (BodyAID == other.BodyAID && BodyBID == other.BodyBID)
        || (BodyAID == other.BodyBID && BodyBID == other.BodyAID);
    }

    bool operator!=(const CollisionPair& other) const
    {
        return !(*this == other);
    }
};

class PhysicsScene
{
public:
    ~PhysicsScene();
    
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

    static s32 CompareSAP(const void* a, const void* b); // MW @todo: What is "SAP"?

    static void SortBodiesBounds(const Body** bodies, const s32 bodyCount, PseudoBody* sortedArray, const f32 delta);
    static void BuildPairs(std::vector<CollisionPair>& pairs, const PseudoBody* sortedBodies, const s32 bodyCount);
    void SweepAndPrune1D(const Body** bodies, const s32 bodyCount, std::vector<CollisionPair>& outputPairs, const f32 delta);
    void BroadPhase(const Body** bodies, const s32 bodyCount, std::vector<CollisionPair>& outputPairs, const f32 delta);
    
    std::vector<Body*> m_Bodies;

    PseudoBody* m_SortedBodyArray = nullptr;
    s32 m_SortedBodyArraySize = 0;
};
