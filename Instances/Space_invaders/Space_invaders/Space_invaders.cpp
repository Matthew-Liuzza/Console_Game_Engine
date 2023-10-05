#include "WindowsGameEngine_abs.h"

const short n_balls = 36;

class ElasticCollision : public WindowsGameEngine {

public:

	struct m_ball {
		float px, py, vx, vy, ax, ay, r;
		short col;
	};

	float a = 200.0f;

	m_ball m_Balls[n_balls];

	ElasticCollision() {

		memset(m_Balls, 0, n_balls * sizeof(m_ball));

	}

	virtual bool userCreate() override {


		for (int i = 0; i < n_balls; ++i) {

			m_Balls[i].px = rand() % m_nScreenWidth;
			m_Balls[i].py = rand() % m_nScreenHeight;
			m_Balls[i].r = 16;
			m_Balls[i].vx = 0;
			m_Balls[i].vy = 0;
			m_Balls[i].ax = 0;
			m_Balls[i].ay = 0;

			m_Balls[i].col = FG_BLUE;
		}

		m_Balls[0].px = 100;
		m_Balls[0].py = 200;
		m_Balls[0].col = FG_DARK_RED;

		return true;
	}

	virtual bool userUpdateBuffer(float f_t) override
	{

		FillScreen();



			if (m_keyStates[VK_RIGHT].bHeld)
				m_Balls[0].ax += a;

			if (m_keyStates[VK_LEFT].bHeld)
				m_Balls[0].ax -= a;

			if (m_keyStates[VK_UP].bHeld)
				m_Balls[0].ay += a;

			if (m_keyStates[VK_DOWN].bHeld)
				m_Balls[0].ay -= a;

			for (int i = 0; i < n_balls; ++i) {

				for (int j = 0; j < n_balls; ++j) {

					if (j == i);

					else {

						float d_x = m_Balls[i].px - m_Balls[j].px;
						float d_y = m_Balls[i].py - m_Balls[j].py;

						float d = fabs(d_x * d_x + d_y * d_y);

						if (d < (m_Balls[i].r + m_Balls[j].r) * (m_Balls[i].r + m_Balls[j].r)) {


							m_Balls[i].ax = a * 0.9f * d_x;
							m_Balls[i].ay = a * 0.9f * d_y;

							m_Balls[j].ax = -a * 0.9f * d_x;
							m_Balls[j].ay = -a * 0.9f * d_y;

						}
					}
				}

				if (m_Balls[i].px > m_nScreenWidth) {
					m_Balls[i].px = m_nScreenWidth - m_Balls[i].r;
					m_Balls[i].ax *= -0.8;
				}

				if (m_Balls[i].px < 0) {
					m_Balls[i].px = m_Balls[i].r;
					m_Balls[i].ax *= -0.8;
				}

				if (m_Balls[i].py > m_nScreenHeight) {
					m_Balls[i].py = m_nScreenHeight - m_Balls[i].r;
					m_Balls[i].ay *= -0.8;
				}

				if (m_Balls[i].py < 0) {
					m_Balls[i].py = m_Balls[i].r;
					m_Balls[i].ay *= -0.8;
				}

				m_Balls[i].ax *= 0.99f;
				m_Balls[i].ay *= 0.99f;

				m_Balls[i].px += m_Balls[i].ax * f_t * f_t;
				m_Balls[i].py += m_Balls[i].ay * f_t * f_t;

				if (abs(m_Balls[0].ax) < 0.5f)
					m_Balls[0].ax = 0.0f;

				if (abs(m_Balls[0].ay) < 0.5f)
					m_Balls[0].ay = 0.0f;

				FillCircle(m_Balls[i].px, m_Balls[i].py, m_Balls[i].r, false, m_Balls[i].col);
			}


			//FillCircle(m_Balls[0].px, m_Balls[0].py, 4);

		return true;
	}
};

int main() {

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, 16);


	ElasticCollision game;
	game.constructConsole(800, 400);
	game.userCreate();
	game.gameStart();
}