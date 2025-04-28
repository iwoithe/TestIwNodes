#pragma once

#include <chrono>

namespace iw
{
	class DeltaTime
	{
	public:
		DeltaTime()
		{
			m_deltaTime = 0.0;
			m_elapsedTime = 0.0;
			m_fps = 30.0;
			m_curTime = std::chrono::steady_clock::now();
			m_prevTime = std::chrono::steady_clock::now();
		}

		double elapsedTime()
		{
			return m_elapsedTime;
		}

		void setElapsedTime(double elapsedTime)
		{
			m_elapsedTime = elapsedTime;
		}

		double fps() const
		{
			return m_fps;
		}

		void setFps(double fps)
		{
			m_fps = fps;
		}

		double get() const
		{
			return std::max(m_deltaTime, 1 / m_fps);
		}

		double getUnlimited() const
		{
			return m_deltaTime;
		}

		void tick()
		{
			m_curTime = std::chrono::steady_clock::now();
			m_deltaTime = std::chrono::duration<double>(m_curTime - m_prevTime).count();
			m_elapsedTime += m_deltaTime;
			m_prevTime = m_curTime;
		}

	private:
		std::chrono::time_point<std::chrono::steady_clock> m_prevTime;
		std::chrono::time_point<std::chrono::steady_clock> m_curTime;
		double m_deltaTime;
		double m_fps;
		double m_elapsedTime;
	};
}
