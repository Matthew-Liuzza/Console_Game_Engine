#include "WindowsGameEngine_abs.h"


class gameExample : public WindowsGameEngine {

public:

    float pi, g, theta_0, v_0, x_0, y_0, x, y, t, v_x, v_y;

    float theta_0_a, v_0_a, x_0_a, y_0_a, x_a, y_a, v_x_a, v_y_a;

    gameExample() {

        pi = 3.1415f;
        g = 9.81f;
        t = 0;

        theta_0 = pi / 3.0f;
        v_0 = 100;
        x_0 = 0;
        y_0 = 0;
        x = 0;
        y = 0;

        theta_0_a = pi / 2.2f;
        v_0_a = 120;
        x_0_a = 100;
        y_0_a = 0;
        x_a = 0;
        y_a = 0;

        v_x = 0;
        v_y = 0;
        v_x_a = 0;
        v_y_a = 0;


    }

    virtual bool userCreate() {

        return true;
    }

    virtual bool userUpdateBuffer(float f_elapsed_t) {

        while (!m_keyStates[VK_SPACE].bHeld) {

            if (m_keyStates[VK_LEFT].bPressed) {
                t = 0;
                x = 0;
                y = 0;
                x_a = 0;
                y_a = 0;
            }
            FillScreen();

            x = v_0 * cos(theta_0) * t + x_0;
            y = (v_0 * sin(theta_0) * t) - (0.5f * g * pow(t, 2)) + y_0;

            x_a = v_0_a * cos(theta_0_a) * t + x_0_a;
            y_a = (v_0_a * sin(theta_0_a) * t) - (0.5 * g * pow(t, 2)) + y_0_a;
            t += 0.25f;

            v_x = v_0 * cos(theta_0);
            v_y = v_0 * sin(theta_0) - (g * t);

            v_x_a = v_0_a * cos(theta_0_a);
            v_y_a = v_0_a * sin(theta_0_a) - (g * t);

            FillCircle(x, y, 10, false, FG_CYAN);
            DrawLine(x, y, x, y - 50, FG_RED);
            DrawLine(x, y, x + v_x, y + v_y, FG_GREEN);

            FillCircle(x_a, y_a, 20, false, FG_YELLOW);
            DrawLine(x_a, y_a, x_a, y_a - 50, FG_RED);
            DrawLine(x_a, y_a, x_a + v_x_a, y_a + v_y_a, FG_GREEN);

            return true;

        }
    }
};

int main() {

    // random number generator
    std::random_device rd; // obtains seed for random number generator
    std::mt19937 gen(rd()); // uses standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(1, 32); // use 'distrib(gen)' to transform the random unsigned int generated by gen into an int from [1,32]

    gameExample game;
    game.constructConsole(800, 600);
    game.gameStart();

}