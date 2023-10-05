#include "WindowsGameEngine_abs_1.h"

const int N = 30;

class MouseDraw : public WindowsGameEngine {

public:
	struct Ball {

		float m, r;
		float px, py;
		float vx, vy;
		float ax, ay;
		short col;
		int id;
		float px_0, py_0;

	};

	float a_mult = 75.0f;

	Ball m_balls[N];
	Ball* pBallSelect = nullptr;

	int x_0, y_0;

	MouseDraw() {

		x_0 = m_mouseX;
		y_0 = m_mouseY;

		std::memset(m_balls, 0, sizeof(Ball) * N);
	}

	virtual bool userCreate() override {

		int i = 0;
		for (auto& ball : m_balls) {

			ball.px = rand() % m_nScreenWidth;
			ball.py = rand() % m_nScreenHeight;
			ball.r = 25;
			ball.m = ball.r;
			ball.vx = 0;
			ball.vy = 0;
			ball.ax = 0;
			ball.vy = 0;
			ball.col = 1 + rand() % 15;
			ball.id = i;

			ball.px_0 = ball.px;
			ball.py_0 = ball.py;

			++i;

		}
		return true;

	}

	virtual bool userUpdateBuffer(float f_t) override {

		FillScreen();

		std::vector < std::pair <Ball*,Ball*>> vec_balls;

		auto CalcDistance = [](float x1, float y1, float x2, float y2) {

			return sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
		};

		auto IsOverlap = [](float x1, float y1, float x2, float y2, float r1, float r2) {

			return fabs((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) <= (r1 + r2) * (r1 + r2));
		};

		auto IsInCircle = [](float mx, float my, float bx, float by, float r) {

			return ((mx - bx) * (mx - bx) + (my - by) * (my - by) <= r * r);
		};

		if (m_mouseStates[0].bPressed || m_mouseStates[1].bPressed) {
			pBallSelect = nullptr;
		
			for (auto &ball : m_balls) {

				if (IsInCircle(m_mouseX,m_mouseY,ball.px,ball.py,ball.r)) {
					pBallSelect = &ball;
					break;

				}				
			}
		}

		if (m_mouseStates[0].bHeld) {
			if (pBallSelect != nullptr) {
				pBallSelect->px = m_mouseX;
				pBallSelect->py = m_mouseY;
			}
		}
	
		if (m_mouseStates[0].bReleased)
			pBallSelect = nullptr;

		if (m_mouseStates[1].bReleased) {

			if (pBallSelect != nullptr) {
				float traj_d = CalcDistance(m_mouseX, m_mouseY, pBallSelect->px, pBallSelect->py);

				pBallSelect->ax = 30.0f * log(traj_d) * ((pBallSelect->px) - (float)m_mouseX);
				pBallSelect->ay = 30.0f * log(traj_d) * ((pBallSelect->py) - (float)m_mouseY);
			}

			pBallSelect = nullptr;
		}

		for (auto& ball_i : m_balls) {

			for (auto& ball_j : m_balls) {

				if (ball_i.id != ball_j.id) {

					if (IsOverlap(ball_i.px, ball_i.py, ball_j.px, ball_j.py, ball_i.r, ball_j.r)) {

						Ball* ball_i_pt = &ball_i;
						Ball* ball_j_pt = &ball_j;

						vec_balls.push_back(std::make_pair(ball_i_pt, ball_j_pt));

						float f_distance = CalcDistance(ball_i.px, ball_i.py, ball_j.px, ball_j.py);
						float f_overlap = 0.5f * (f_distance - ball_i.r - ball_j.r);

						ball_i.px -= f_overlap * (ball_i.px - ball_j.px) / f_distance;
						ball_i.py -= f_overlap * (ball_i.py - ball_j.py) / f_distance;

						ball_j.px += f_overlap * (ball_i.px - ball_j.px) / f_distance;
						ball_j.py += f_overlap * (ball_i.py - ball_j.py) / f_distance;

					}
				}
			}
		}

		for (auto& ball : m_balls) {

			ball.ax *= 0.95f;
			ball.ay *= 0.95f;

			//ball.vx = ball.ax * f_t * 1000 - (ball.ax * f_t * 1000 * 0.05f);
			//ball.vy = ball.ay * f_t * 1000 - (ball.ay * f_t * 1000 * 0.05f);

			//ball.px += ball.vx * f_t * 1000;
			//ball.py += ball.vy * f_t * 1000;

			ball.px += ball.ax * f_t * 1000;
			ball.py += ball.ay * f_t * 1000;

			if (ball.px + ball.r > m_nScreenWidth || ball.px - ball.r < 0) {
				if (ball.px - ball.r < 0)
					ball.px = ball.r;
				else
					ball.px = ball.px - ball.r;

				ball.ax *= -0.8;
			}

			if (ball.py + ball.r > m_nScreenHeight || ball.py - ball.r < 0) {
				if (ball.py - ball.r < 0)
					ball.py = ball.r;
				else
					ball.py = ball.py - ball.r;

				ball.ay *= -0.8;
			}

			if (ball.ax * ball.ax + ball.ay * ball.ay < 0.01f) {
				ball.ax = 0;
				ball.ay = 0;
			}
		}

		for (auto& balls : vec_balls) {

			float ball_i_moment = 2.0f * balls.second->m / (balls.first->m + balls.second->m);
			float ball_j_moment = 2.0f * balls.first->m / (balls.first->m + balls.second->m);

			float ball_i_dot_x = (balls.first->ax - balls.second->ax) * (balls.first->px - balls.second->px);
			float ball_i_dot_y = (balls.first->ay - balls.second->ay) * (balls.first->py - balls.second->py);
			float ball_i_dot = ball_i_dot_x + ball_i_dot_y;

			float ball_j_dot_x = (balls.second->ax - balls.first->ax) * (balls.second->px - balls.first->px);
			float ball_j_dot_y = (balls.second->ay - balls.first->ay) * (balls.second->py - balls.first->py);
			float ball_j_dot = ball_j_dot_x + ball_j_dot_y;

			float ball_norm = CalcDistance(balls.first->px, balls.first->py, balls.second->px, balls.second->py);
			ball_norm *= ball_norm * ball_norm;

			float ball_i_x = balls.first->px - balls.second->px;
			float ball_i_y = balls.first->py - balls.second->px;

			float ball_j_x = -ball_i_x;
			float ball_j_y = -ball_i_y;

			balls.first->ax = balls.first->ax - (ball_i_moment * ball_i_dot / ball_norm) * ball_i_x;
			balls.first->ay = balls.first->ay - (ball_i_moment * ball_i_dot / ball_norm) * ball_i_y;

			balls.second->ax = balls.second->ax - (ball_j_moment * ball_j_dot / ball_norm) * ball_j_x;
			balls.second->ay = balls.second->ay - (ball_j_moment * ball_j_dot / ball_norm) * ball_j_y;
		}


		for (auto &ball : m_balls) {
			FillCircle(ball.px, ball.py, ball.r, false, ball.col);
			
			if (pBallSelect != nullptr)
				DrawLine(pBallSelect->px, pBallSelect->py, m_mouseX, m_mouseY, FG_BLUE);
		}

		return true;
	}
};


int main() {

	MouseDraw game;
	game.constructConsole(800, 400);
	game.userCreate();
	game.gameStart();


}
