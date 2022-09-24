#include "pch.h"
#include "Camera.h"
#include "utils.h"
#include "Game.h"

Camera::Camera(float width, float height)
{
	m_Width = width;
	m_Height = height;
	m_LevelBoundaries = Rectf{ 0, 0, m_Width, m_Height };
}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

void Camera::Transform(const Rectf& target, const float verticalDistance)
{
	Point2f bottomLeftPos{ Track(target) };
	bottomLeftPos.y += verticalDistance;
	Clamp(bottomLeftPos);
	glTranslatef(-bottomLeftPos.x, -bottomLeftPos.y, 0);
}


Point2f Camera::Track(const Rectf& target)
{
	return Point2f((target.left + target.width / 2) - m_Width / 2, (target.bottom + target.height / 2) - m_Height / 2);
}

void Camera::Clamp(Point2f& bottomLeftPos)
{
	if (m_LevelBoundaries.left > bottomLeftPos.x)
	{
		bottomLeftPos.x = m_LevelBoundaries.left;
	}

	if (m_LevelBoundaries.width < bottomLeftPos.x + m_Width)
	{
		bottomLeftPos.x = m_LevelBoundaries.width - m_Width;
	}

	if (m_LevelBoundaries.bottom > bottomLeftPos.y)
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom;
	}

	if (m_LevelBoundaries.height < bottomLeftPos.y + m_Height)
	{
		bottomLeftPos.y = m_LevelBoundaries.height - m_Height;
	}
}

