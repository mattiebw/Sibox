#include "siboxpch.h"
#include "Physics/PhysicsScene.h"

#include "Core/Application.h"

void PhysicsScene::Update()
{
    f64 delta = Application::Get()->GetDeltaTime();
}

void PhysicsScene::AddBody(Body* body)
{
    m_Bodies.push_back(body);
}

void PhysicsScene::RemoveBody(Body* body)
{
    for (s32 i = static_cast<s32>(m_Bodies.size()) - 1; i >= 0; i--)
    {
        if (m_Bodies[i] == body)
        {
            m_Bodies.erase(m_Bodies.begin() + i);
            break;
        }
    }
}
