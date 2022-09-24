#include "pch.h"
#include "Level.h"
#include "utils.h"
#include "Game.h"
#include "SVGParser.h"
#include <iostream>

Level::Level()
    : m_CanGrabledgeRight{ false }
    , m_IsLookingRight{ false }
    , m_Flexibility{10}
{
    m_pBackgroundTexture = new Texture("Resources\\Images\\background.png");
    SVGParser::GetVerticesFromSvgFile("Resources\\Images\\background.svg", m_VectorOfVertices);
    m_Boundaries = Rectf{ 0, 0, m_pBackgroundTexture->GetWidth() , m_pBackgroundTexture->GetHeight() };
}

Level::~Level()
{
    delete m_pBackgroundTexture;
    m_pBackgroundTexture = nullptr;
}

void Level::DrawBackground() const
{
    Rectf rect{};
    rect.left = 0.0f;
    rect.bottom = m_pBackgroundTexture->GetHeight();
    rect.width = m_pBackgroundTexture->GetWidth();
    rect.height = m_pBackgroundTexture->GetHeight();

    m_pBackgroundTexture->Draw(m_Boundaries, rect);
}

bool Level::HandleCollision(Rectf& actorshape, Vector2f& actorVelocity, const bool& isPlayer) const
{
    HandleCollisionFloorRoof(actorshape, actorVelocity);  
    HandleCollisionWalls(actorshape, actorVelocity);
    return HandleCanGrabAledge(actorshape, actorVelocity, isPlayer);

}

void Level::HandleCollisionFloorRoof(Rectf& actorShape, Vector2f& actorVelocity) const
{
    utils::HitInfo hitInfo{};
    for (unsigned int i = 0; i < m_VectorOfVertices.size(); i++)
    {
        if (utils::Raycast(m_VectorOfVertices[i], Point2f{ actorShape.left + (actorShape.width - m_Flexibility), (actorShape.bottom + actorShape.height) }
            , Point2f{ actorShape.left + (actorShape.width - m_Flexibility), actorShape.bottom }, hitInfo) || utils::Raycast(m_VectorOfVertices[i], Point2f{ actorShape.left + m_Flexibility, (actorShape.bottom + actorShape.height) }
                , Point2f{ actorShape.left + m_Flexibility, actorShape.bottom }, hitInfo))
        {
            if (actorVelocity.y < 0)
                actorShape.bottom = hitInfo.intersectPoint.y;

            else
                actorShape.bottom = hitInfo.intersectPoint.y - actorShape.height;

                actorVelocity.y = 0;
        }
    }
}

void Level::HandleCollisionWalls(Rectf& actorshape, Vector2f& actorVelocity) const
{
    utils::HitInfo hitInfo{};

    for (unsigned int i = 0; i < m_VectorOfVertices.size(); i++)
    {
        if (utils::Raycast(m_VectorOfVertices[i], Point2f{ actorshape.left, actorshape.bottom + actorshape.height }
            , Point2f{ actorshape.left + actorshape.width, actorshape.bottom + actorshape.height }, hitInfo) || utils::Raycast(m_VectorOfVertices[i], Point2f{ actorshape.left, actorshape.bottom + m_Flexibility / 2 }
            , Point2f{ actorshape.left + actorshape.width, actorshape.bottom + m_Flexibility / 2 }, hitInfo))
        {
            if (actorshape.left + (actorshape.width / 2) > hitInfo.intersectPoint.x)
                actorshape.left = hitInfo.intersectPoint.x;

            if (actorshape.left + (actorshape.width / 2) < hitInfo.intersectPoint.x)
                actorshape.left = hitInfo.intersectPoint.x - actorshape.width;

            actorVelocity.x = 0; 
        }
    }
}

bool Level::IsOnGround(const Rectf& actorShape, Vector2f& actorVelocity) const
{
     utils::HitInfo hitInfo{};
     for (unsigned int i = 0; i < m_VectorOfVertices.size(); i++)
     {
         if (utils::Raycast(m_VectorOfVertices[i], Point2f{ actorShape.left + (actorShape.width - m_Flexibility), (actorShape.bottom + actorShape.height) }
             , Point2f{ actorShape.left + (actorShape.width - m_Flexibility), actorShape.bottom - 1 }, hitInfo) || utils::Raycast(m_VectorOfVertices[i], Point2f{ actorShape.left + m_Flexibility, (actorShape.bottom + actorShape.height) }
             , Point2f{ actorShape.left + m_Flexibility, actorShape.bottom - 1 }, hitInfo))
         {
             return true;
         }
     }
     return false;
}

bool Level::HandleCanGrabAledge(Rectf& actorShape, Vector2f& actorVelocity, const bool& isPlayer) const
{
    utils::HitInfo hitInfo{};

    if (!isPlayer)
        return false;

    //looks what the last direction was
    if (actorVelocity.x < 0)
    {
        m_CanGrabledgeRight = false;
    }
    else if (actorVelocity.x > 0)
    {
        m_CanGrabledgeRight = true;
    }
    if (actorVelocity.y < 0)
    {
        for (unsigned int i = 0; i < m_VectorOfVertices.size(); i++)
        {
            if ((!utils::Raycast(m_VectorOfVertices[i], Point2f{ actorShape.left + actorShape.width + (actorShape.width / 4)
                    , (actorShape.bottom + actorShape.height + 4) }, Point2f{ actorShape.left - (actorShape.width / 4)
                    , (actorShape.bottom + actorShape.height + 4) }, hitInfo)) && (!utils::Raycast(m_VectorOfVertices[i], Point2f{ actorShape.left + (actorShape.width / 2)
                    , (actorShape.bottom + actorShape.height + 10) }, Point2f{ actorShape.left + (actorShape.width / 2)
                    , (actorShape.bottom + actorShape.height) }, hitInfo)))
            {
                if (!m_CanGrabledgeRight)
                {
                    if (utils::Raycast(m_VectorOfVertices[i], Point2f{ actorShape.left - (actorShape.width / 4), (actorShape.bottom + actorShape.height) }
                        , Point2f{ actorShape.left + (actorShape.width / 4), (actorShape.bottom + actorShape.height) }, hitInfo))
                    {
                        actorShape.bottom = hitInfo.intersectPoint.y - (actorShape.height - 1);
                        actorVelocity.y = 0;
                        return true;
                    }
                }
                else
                {
                    if (utils::Raycast(m_VectorOfVertices[i], Point2f{ actorShape.left + (actorShape.width / 4), (actorShape.bottom + actorShape.height) }
                        , Point2f{ actorShape.left + actorShape.width + (actorShape.width / 4), (actorShape.bottom + actorShape.height) }, hitInfo))
                    {
                        actorShape.bottom = hitInfo.intersectPoint.y - (actorShape.height - 1);
                        actorVelocity.y = 0;
                        return true;
                    }   
                }

               
            }
        }
    }
    return false;
}

bool Level::IsOnledge(const Rectf& actorShape, Vector2f& actorVelocity) const
{
    utils::HitInfo hitInfo{};
    for (unsigned int i = 0; i < m_VectorOfVertices.size(); i++)
    {  
        if (IsOnGround(actorShape, actorVelocity))
        {
            if (!(utils::Raycast(m_VectorOfVertices[i], Point2f{ actorShape.left, actorShape.bottom - 1 }
                , Point2f{ actorShape.left, actorShape.bottom + 2 }, hitInfo)) && (utils::Raycast(m_VectorOfVertices[i], Point2f{ actorShape.left + actorShape.width, actorShape.bottom - 1 }
                , Point2f{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height / 2 }, hitInfo)))
            {
                return true;
            }
            else if ((utils::Raycast(m_VectorOfVertices[i], Point2f{ actorShape.left, actorShape.bottom - 1 }
                    , Point2f{ actorShape.left, actorShape.bottom + 2 }, hitInfo)) && !(utils::Raycast(m_VectorOfVertices[i], Point2f{ actorShape.left + actorShape.width, actorShape.bottom - 1 }
                    , Point2f{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height / 2 }, hitInfo)))
            {
                return true;
            }
        }
    }
    return false;
}

Rectf Level::GetBoundaries()
{
    return m_Boundaries;
}

bool Level::HasReachedDoor(const Rectf& actorShape)
{
    return utils::IsOverlapping(Point2f{ 2315, 240 }, Point2f{ 2315, 280 }, actorShape);
}


