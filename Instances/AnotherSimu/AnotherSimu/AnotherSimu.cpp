#include "WindowsGameEngine_abs_1.h"

const int N = 10;
const float G = 9.8f;

class ProjectileMotion : public WindowsGameEngine {

public:

	struct m_ball {
		float px, py, vx, vy, ax, ay, r, m, v0, theta;
	};


	m_ball Ball;
	m_ball* B_ptr = nullptr;
	bool ball_flg = false;
	bool timer = false;
	float t = 0.0f;

	ProjectileMotion() {

		Ball.px = 0.0f;
		Ball.py = 0.0f;
		Ball.v0 = 0.0f;
		Ball.vx = 0.0f;
		Ball.vy = 0.0f;
		Ball.ax = 0.0f;
		Ball.ay = 0.0f;
		Ball.r = 7.0f;
		Ball.m = 25.0f;
		Ball.theta = 0.0f;

	}

	virtual bool userCreate() override {

		return true;
	}

	virtual bool userUpdateBuffer(float f_t) {

		FillScreen();

		auto IsInCircle = [](float mx, float my, float px, float py, float r) {

			return ((mx - px) * (mx - px) + (my - py) * (my - py) < r * r);
		};

		auto CalcDist = [](float x1, float y1, float x2, float y2) {

			return sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
		};

			
			if (m_mouseStates[1].bPressed || m_mouseStates[1].bHeld)
				if (IsInCircle(m_mouseX, m_mouseY, Ball.px, Ball.py, Ball.r))
					ball_flg = true;

			if (m_mouseStates[1].bReleased) {

				if (ball_flg) {

					float mX = m_mouseX;
					float mY = m_mouseY;

					Ball.v0 = CalcDist(mX, mY, Ball.px, Ball.py) / 3.0f;

					float x = CalcDist(mX, mY, Ball.px, mY);
					float y = CalcDist(Ball.px, mY, Ball.px, Ball.py);
					Ball.theta = atanf(y / x);
					timer = true;

				}

				ball_flg = false;
			}

			Ball.px = 75.0f + (Ball.v0 * cos(Ball.theta) * t);
			Ball.py = 75.0f + (Ball.v0 * sin(Ball.theta) * t - (0.5f * G * t * t));

			if (Ball.py < 75.0f)
				Ball.py = 75.0f;

			if (timer)
				t += 0.15;

			DrawCircle(Ball.px, Ball.py, Ball.r, FG_GREEN);
			DrawLine(0, 75 - Ball.r - 1, m_nScreenWidth, 75 - Ball.r - 1, FG_WHITE);
			DrawLine(750, 0, 750, m_nScreenHeight, FG_WHITE);


			if (ball_flg) {
				DrawLine(m_mouseX, m_mouseY, Ball.px, Ball.py, FG_BLUE);
				DrawLine(Ball.px, m_mouseY, Ball.px, Ball.py, FG_RED);
				DrawLine(m_mouseX, m_mouseY, Ball.px, m_mouseY, FG_CYAN);

			}
		
		return true;
	}




};


int main() {

	ProjectileMotion simulation;
	simulation.constructConsole(800, 400);
	simulation.userCreate();
	simulation.gameStart();



	
}