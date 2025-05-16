#include "siboxpch.h"
#include "Physics/PhysicsScene.h"

#include "Core/Application.h"
#include "Core/Random.h"

void PhysicsScene::Update()
{
    f32 delta = Application::Get()->GetDeltaTime();
}

Body& PhysicsScene::AddBody(const Body& body)
{
    m_Bodies.push_back(body);
    Body& newBody = m_Bodies.back();
    newBody.Handle = static_cast<u32>(Random::Int());
    return newBody;
}

void PhysicsScene::RemoveBody(u32 handle)
{
    for (s32 i = static_cast<s32>(m_Bodies.size()) - 1; i >= 0; i--)
    {
        if (m_Bodies[i].Handle == handle)
        {
            m_Bodies.erase(m_Bodies.begin() + i);
            break;
        }
    }
}
