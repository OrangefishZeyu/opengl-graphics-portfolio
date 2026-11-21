#include <GL/freeglut.h>
#include <math.h>
//using namespace std;
//#pragma comment (lib, "freeglut_static.lib")  // 链接静态库

namespace CloudNS {
    class Cloud {
    public:
        Cloud(float x, float y, float radiusX, float radiusY, float speed)
            : x(x), y(y), radiusX(radiusX), radiusY(radiusY), speed(speed) {}

        void draw() {
            glColor3f(1.0, 1.0, 1.0);  // 白色

            // 绘制第一个椭圆
            glBegin(GL_POLYGON);
            for (int i = 0; i < 360; i += 10) {
                float degInRad = i * 3.14159 / 180;
                glVertex2f(x + cos(degInRad) * radiusX, y + sin(degInRad) * radiusY);
            }
            glEnd();

            // 绘制第二个椭圆
            glBegin(GL_POLYGON);
            for (int i = 0; i < 360; i += 10) {
                float degInRad = i * 3.14159 / 180;
                glVertex2f(x + radiusX * 1.5f + cos(degInRad) * radiusX, y + sin(degInRad) * radiusY);
            }
            glEnd();

            // 绘制第三个椭圆
            glBegin(GL_POLYGON);
            for (int i = 0; i < 360; i += 10) {
                float degInRad = i * 3.14159 / 180;
                glVertex2f(x + radiusX * 0.75f + cos(degInRad) * radiusX, y + radiusY * 0.5f + sin(degInRad) * radiusY);
            }
            glEnd();
        }

        void update() {
            x += speed;
            if (x > 1.0f + radiusX * 1.5f) {
                x = -1.0f - radiusX * 1.5f;
            }
        }

    private:
        float x, y;
        float radiusX, radiusY;
        float speed;
    };
}

namespace FlameNS {
    class Flame {
    public:
        Flame(float centerX, float centerY, float height)
            : centerX(centerX), centerY(centerY), height(height), scale(1.0f), targetX(centerX + 0.1f), targetY(centerY - 0.05f), isShrinking(false) {}

        void draw() {
            if (scale <= 0.0f) return; // 如果火焰已经消失，则不绘制

            glBegin(GL_TRIANGLE_FAN);
            glColor3f(1.0f, 1.0f, 0.0f); // 黄色
            glVertex2f(centerX, centerY - height / 2 * scale); // 底部

            glColor3f(1.0f, 0.0f, 0.0f); // 红色
            glVertex2f(centerX, centerY + height / 2 * scale); // 顶部

            // 绘制第一条贝塞尔曲线
            for (int i = 0; i <= 200; i++) {
                float t = static_cast<float>(i) / 200.0f;
                float u = 1.0f - t;
                float tt = t * t;
                float uu = u * u;
                float uuu = uu * u;
                float ttt = tt * t;

                float x = uuu * centerX + 3 * uu * t * (centerX - 0.045f) + 3 * u * tt * (centerX - 0.045f) + ttt * centerX;
                float y = uuu * (centerY - height / 2) + 3 * uu * t * (centerY - height / 2 + 0.03f) + 3 * u * tt * (centerY - height / 2 + 0.07f) + ttt * (centerY + height / 2);

                // 切变缩小效果
                if (isShrinking) {
                    x = centerX + (x - centerX) * scale + (targetX - centerX) * (1 - scale);
                    y = centerY + (y - centerY) * scale + (targetY - centerY) * (1 - scale);
                }

                glVertex2f(x, y);
            }

            // 绘制第二条贝塞尔曲线
            for (int i = 0; i <= 200; i++) {
                float t = static_cast<float>(i) / 200.0f;
                float u = 1.0f - t;
                float tt = t * t;
                float uu = u * u;
                float uuu = uu * u;
                float ttt = tt * t;

                float x = uuu * centerX + 3 * uu * t * (centerX + 0.045f) + 3 * u * tt * (centerX + 0.045f) + ttt * centerX;
                float y = uuu * (centerY - height / 2) + 3 * uu * t * (centerY - height / 2 + 0.03f) + 3 * u * tt * (centerY - height / 2 + 0.07f) + ttt * (centerY + height / 2);

                // 切变缩小效果
                if (isShrinking) {
                    x = centerX + (x - centerX) * scale + (targetX - centerX) * (1 - scale);
                    y = centerY + (y - centerY) * scale + (targetY - centerY) * (1 - scale);
                }

                glVertex2f(x, y);
            }

            glEnd();
        }

        void update() {
            if (isShrinking) {
                if (scale > 0.0f) {
                    scale -= 0.01f;
                    if (scale < 0.0f) scale = 0.0f; // 确保缩放到0
                }
                else {
                    isShrinking = false;
                }
            }
        }

        void startShrinking() {
            isShrinking = true;
        }

        void reset() {
            isShrinking = false;
            scale = 1.0f; // 回到初始状态
        }

    private:
        float centerX, centerY;
        float height;
        float scale;
        float targetX, targetY;
        bool isShrinking;
    };
}

GLfloat w = 600;
GLfloat h = 800;

FlameNS::Flame flame1(0.0f, 0.47f, 0.1f);
FlameNS::Flame flame2(0.275f, 0.47f, 0.1f);
FlameNS::Flame flame3(-0.275f, 0.47f, 0.1f);

CloudNS::Cloud clouds[2] = {
    CloudNS::Cloud(1.5f, 0.8f, 0.2f, 0.1f, 0.008f),
    CloudNS::Cloud(0.0f, 0.7f, 0.2f, 0.1f, 0.008f),
};

bool balloonGenerated = false; // 标志变量，用于跟踪气球是否已经生成
struct Star {
    float x, y; // 中心坐标
    float R, r; // 大圆和小圆的半径
    float angle; // 旋转角度
    int rdm; // 颜色类型
    bool active; // 是否激活
};

Star stars[1000];
int starCount = 0;

void selectColor(int type) {
    switch (type) {
    case 1: glColor3f(0.0 / 255, 61.0 / 255, 153.0 / 255); break;
    case 2: glColor3f(28.0 / 255, 38.0 / 255, 82.0 / 255); break;
    case 3: glColor3f(30.0 / 255, 44.0 / 255, 128.0 / 255); break;
    case 4: glColor3f(249.0 / 255, 104.0 / 255, 70.0 / 255); break;
    case 5: glColor3f(252.0 / 255, 193.0 / 255, 88.0 / 255); break;
    case 6: glColor3f(42.0 / 255, 12.0 / 255, 88.0 / 255); break;
    case 7: glColor3f(179.0 / 255, 0.0 / 255, 124.0 / 255); break;
    case 8: glColor3f(234.0 / 255, 139.0 / 255, 89.0 / 255); break;
    case 9: glColor3f(255.0 / 255, 243.0 / 255, 127.0 / 255); break;
    case 10: glColor3f(0.0 / 255, 94.0 / 255, 255.0 / 255); break;
    case 11: glColor3f(44.0 / 255, 228.0 / 255, 214.0 / 255); break;
    default: break;
    }
}

void drawStar(float cx, float cy, float R, float r, float angle, int rdm) {
    float x0, y0, x1, y1;
    float o0 = angle;
    float o1 = angle + 0.2 * 3.14;

    for (int i = 0; i < 10; i++) {
        x0 = cx + R * cos(o0);
        y0 = cy + R * sin(o0);
        x1 = cx + r * cos(o1);
        y1 = cy + r * sin(o1);
        if (i % 2 == 0) selectColor(rdm);
        else {
            if (rdm % 2 == 0) selectColor(rdm + 1);
            else selectColor(rdm - 1);
        }
        glBegin(GL_POLYGON);
        glVertex2f(x0, y0);
        glVertex2f(x1, y1);
        glVertex2f(cx, cy);
        glEnd();
        if (i % 2 == 0) o0 = o0 + 0.4 * 3.14;
        else o1 = o1 + 0.4 * 3.14;
    }
}

void drawSineWave(float x1, float y1, float x2, float y2, float amplitude, float frequency, int segments) {
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float x = x1 + (x2 - x1) * i / segments;
        float y = y1 + amplitude * sin(frequency * x);
        glVertex2f(x, y);
        glVertex2f(x, y2);
    }
    glEnd();
}

void cake() {
    // 第一个矩形
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 0.8f); // 浅黄色
    glVertex2f(-0.8, -0.5);
    glVertex2f(0.8, -0.5);
    glVertex2f(0.8, -0.25);
    glVertex2f(-0.8, -0.25);
    glEnd();

    // 绘制正弦曲线并填充
    glColor3f(1.0f, 1.0f, 1.0f); // 白色
    drawSineWave(-0.8, -0.375, 0.8, -0.25, 0.05, 10.0, 100);

    // 第二个矩形
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 0.8f); // 浅黄色
    glVertex2f(-0.6, -0.25);
    glVertex2f(0.6, -0.25);
    glVertex2f(0.6, 0);
    glVertex2f(-0.6, 0);
    glEnd();

    // 绘制正弦曲线并填充
    glColor3f(1.0f, 1.0f, 1.0f); // 白色
    drawSineWave(-0.6, -0.125, 0.6, 0, 0.05, 10.0, 100);

    // 第三个矩形
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 0.8f); // 浅黄色
    glVertex2f(-0.4, 0);
    glVertex2f(0.4, 0);
    glVertex2f(0.4, 0.25);
    glVertex2f(-0.4, 0.25);
    glEnd();

    // 绘制正弦曲线并填充
    glColor3f(1.0f, 1.0f, 1.0f); // 白色
    drawSineWave(-0.4, 0.125, 0.4, 0.25, 0.05, 15.0, 100);

    // 画蜡烛
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.0f, 0.8f);
    glVertex2f(-0.3, 0.25);
    glVertex2f(-0.25, 0.25);
    glVertex2f(-0.25, 0.4);
    glVertex2f(-0.3, 0.4);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.0f, 0.8f);
    glVertex2f(-0.025, 0.25);
    glVertex2f(0.025, 0.25);
    glVertex2f(0.025, 0.4);
    glVertex2f(-0.025, 0.4);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.0f, 0.8f);
    glVertex2f(0.3, 0.25);
    glVertex2f(0.25, 0.25);
    glVertex2f(0.25, 0.4);
    glVertex2f(0.3, 0.4);
    glEnd();
}

void sky() {
    glBegin(GL_QUADS);
    // 粉红色渐变部分
    glColor3f(1.0f, 0.0f, 0.5f); // 深粉红色
    glVertex2f(-1.0f, -0.5f);
    glVertex2f(1.0f, -0.5f);
    glColor3f(1.0f, 0.5f, 1.0f); // 浅粉红色
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();
}

void ground() {
    glBegin(GL_QUADS);
    // 绿色渐变部分
    glColor3f(0.0f, 0.5f, 0.0f); // 深绿色
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // 浅绿色
    glVertex2f(1.0f, -0.5f);
    glVertex2f(-1.0f, -0.5f);
    glEnd();
}

class Letter {
public:
    float x, y; // 信纸的位置
    float width = 1.1f, height = 1.1f; // 信纸的大小
    bool isVisible; // 是否可见
    bool isAttachedToBalloon; // 是否与气球相连
    float speed = 0.005f; // 上升速度

    Letter() {
        x = 0.0f; // 初始位置为屏幕中央
        y = -1.0f - 2.0f; // 初始位置在气球下面0.25f
        isVisible = false;
        isAttachedToBalloon = false;
    }

    void setPosition(float newX, float newY) {
        x = newX;
        y = newY;
    }

    void drawText(const char* text) {
        if (!isVisible) return; // 如果信纸不可见，则不绘制文本

        float textX = x - width / 2;
        float textY = y + height / 2 - 0.05f;

        glColor3f(0, 0, 0); // 设置文字颜色为黑色

        const char* start = text;
        const char* end = text;

        while (*end != '\0') {
            if (*end == '\n') {
                // 绘制当前行
                glRasterPos2f(textX, textY);
                for (const char* c = start; c < end; c++) {
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
                }
                textY -= 0.05f; // 根据字体大小调整换行的距离
                textX = x - width / 2;
                start = end + 1; // 跳过换行符
            }
            end++;
        }

        // 绘制最后一行
        glRasterPos2f(textX, textY);
        for (const char* c = start; c < end; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
        }
    }

    void draw() {
        if (isVisible) {
            glColor3f(1.0, 1.0, 1.0); // 设置信纸颜色为白色
            glBegin(GL_QUADS);
            glVertex2f(x - width / 2, y - height / 2);
            glVertex2f(x + width / 2, y - height / 2);
            glVertex2f(x + width / 2, y + height / 2);
            glVertex2f(x - width / 2, y + height / 2);
            glEnd();
            drawDetails();
        }
    }

    void drawDetails() {
        //TODO 绘制信纸的细节
        glColor3f(0.8, 0.8, 0.8); // 设置线条颜色为浅灰色
        for (float i = y - height / 2 + 0.05f; i < y + height / 2; i += 0.05f) {
            glBegin(GL_LINES);
            glVertex2f(x - width / 2 + 0.05f, i);
            glVertex2f(x + width / 2 - 0.05f, i);
            glEnd();
        }
    }

    void show() {
        isVisible = true;
    }

    void attachToBalloon() {
        isAttachedToBalloon = true;
    }

    void detachFromBalloon() {
        isAttachedToBalloon = false;
    }

    void update() {
        if (isVisible) {
            y += speed;
            if (y >= 0.0f) {
                y = 0.0f; // 固定在屏幕中心
            }
        }
    }
};

Letter letter;

class Balloon {
protected:
    float x, y;
    float r, g, b;
    float speed = 0.005f;
    float windTime = 0.0f;
    float controlPointOffset = 0.0f;
    bool isSpecial;
    bool isAttachedToLetter; // 是否与信相连

public:
    Balloon(bool special = false) {
        x = 0.0f;
        y = -1.4f;
        isSpecial = special;
        isAttachedToLetter = false;
        if (special) {
            r = 1.0f;
            g = 0.0f;
            b = 0.0f;
        }
        else {
            r = static_cast<float>(rand()) / RAND_MAX;
            g = static_cast<float>(rand()) / RAND_MAX;
            b = static_cast<float>(rand()) / RAND_MAX;
        }
    }

    void setY(float newY) {
        y = newY;
    }

    void setColor(float newR, float newG, float newB) {
        r = newR;
        g = newG;
        b = newB;
    }

    void setSpeed(float newSpeed) {
        speed = newSpeed;
    }

    float getY() const {
        return y;
    }

    bool holdingText() const {
        return isSpecial;
    }

    void update() {
        y += speed;
        windTime += 0.1f;
        controlPointOffset = sin(windTime) * 0.05f;
    }

    virtual void draw() {
        // 绘制弯曲的绳子
        glColor3f(0.5, 0.5, 0.5);  // 灰色
        float controlX = x + controlPointOffset;
        float controlY = y - 1.5f;  // 调整控制点的位置，使其更接近气球的底部

        glBegin(GL_LINE_STRIP);  // 使用GL_LINE_STRIP来绘制连续的线段
        glVertex2f(x, y);  // 起点
        // 使用贝塞尔曲线的公式来绘制曲线
        for (float t = 0; t <= 1; t += 0.01) {
            float pointX = (1 - t) * (1 - t) * x + 2 * (1 - t) * t * controlX + t * t * x;
            float pointY = (1 - t) * (1 - t) * y + 2 * (1 - t) * t * controlY + t * t * (y - 2.0f);
            glVertex2f(pointX, pointY);
        }
        glEnd();

        glColor3f(r, g, b);  // 设置气球颜色
        float balloonRadiusX = 0.2f;  // 气球的X轴半径
        float balloonRadiusY = 0.25f;  // 气球的Y轴半径
        glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i += 10) {
            float degInRad = i * 3.14159 / 180;
            glVertex2f(x + cos(degInRad) * balloonRadiusX, y + sin(degInRad) * balloonRadiusY);  // 椭圆形的气球
        }
        glEnd();

        if (isSpecial) {
            // 绘制高光
            float highlightWidth = 0.025f;  // 高光的宽度
            float highlightHeight = 0.05f;  // 高光的高度
            float highlightX = x + 0.1f;  // 高光X
            float highlightY = y + 0.1f;  // 高光Y

            glBegin(GL_TRIANGLE_FAN);
            glColor4f(0.9f, 0.9f, 0.9f, 0.4f);  // 高光颜色
            glVertex2f(highlightX, highlightY);  // 高光中心点
            for (int i = 0; i <= 360; i += 10) {  // 高光的边缘
                glColor4f(0.9f, 0.9f, 0.9f, 0.0f);  // 高光的边缘颜色
                float degInRad = i * 3.14159 / 180;
                glVertex2f(highlightX + cos(degInRad) * highlightWidth, highlightY + sin(degInRad) * highlightHeight);
            }
            glEnd();
        }
    }

    void attachToLetter() {
        isAttachedToLetter = true;
    }

    void detachFromLetter() {
        isAttachedToLetter = false;
    }
};

const int MAX_BALLOONS = 10; // 设置气球的最大数量
Balloon balloons[MAX_BALLOONS];
int balloonCount = 0;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    sky();
    ground();

    cake();
    // cloud
    for (int i = 0; i < 2; ++i) {
        clouds[i].draw();
    }
    // flame
    flame1.draw();
    flame2.draw();
    flame3.draw();

    // stars
    for (int i = 0; i < starCount; ++i) {
        if (stars[i].active) {
            drawStar(stars[i].x, stars[i].y, stars[i].R, stars[i].r, stars[i].angle, stars[i].rdm);
        }
    }

    // balloons
    for (int i = 0; i < balloonCount; ++i) {
        balloons[i].draw();
    }

    // letter
    letter.draw();
    letter.drawText("                                             Birthday Card\n"
        "\nDear friend,\n"
        "\nHappy Birthday To You!\n"
        "\n"
        "As you celebrate another year of life, I want to take a moment to wish \n you all the best. May your day be filled with joy, laughter, and all the \n things that make you smile. You deserve nothing but the best, and I \n hope this year brings you endless happiness, success, and beautiful \n memories.\n"
        "\nWith all my love and warmest wishes,\n"
        "\nZeyu Cheng");

    glFlush();
}

void update(int value) {
    for (int i = 0; i < 2; ++i) {
        clouds[i].update();
    }
    flame1.update();
    flame2.update();
    flame3.update();

    for (int i = 0; i < balloonCount; ++i) {
        balloons[i].update();
    }

    for (int i = 0; i < starCount; ++i) {
        if (stars[i].active) {
            stars[i].R -= 2.0f / w;
            if (stars[i].r > 0) stars[i].r -= 1.0f / w;
            else stars[i].r = 0;
            stars[i].angle += 0.1; // 增加旋转角度
            if (stars[i].R <= 0) {
                stars[i].active = false;
            }
        }
    }

    letter.update();

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // 60 FPS
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'e':
        flame1.startShrinking();
        flame2.startShrinking();
        flame3.startShrinking();
        break;
    case 'b':
        flame1.reset();
        flame2.reset();
        flame3.reset();
        break;
    case 'f':
        if (!balloonGenerated && balloonCount < MAX_BALLOONS) { // 检查气球是否已经生成
            balloons[balloonCount++] = Balloon(true);  // 添加特殊气球
            letter.show();
            balloonGenerated = true; // 设置标志变量
        }
        break;
    }
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        float mouseX = (float)x / (w / 2) - 1.0f;
        float mouseY = 1.0f - (float)y / (h / 2);

        if (starCount < 1000) {
            Star newStar;
            newStar.rdm = rand() % 10 + 2;
            newStar.x = mouseX;
            newStar.y = mouseY;
            newStar.angle = 0;
            newStar.active = true;

            if (button == GLUT_LEFT_BUTTON) {
                newStar.R = 60.0f / w;
                newStar.r = 20.0f / w;
            }
            else if (button == GLUT_RIGHT_BUTTON) {
                newStar.R = 100.0f / w;
                newStar.r = 40.0f / w;
            }

            stars[starCount++] = newStar;
        }
    }
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 设置背景颜色为黑色
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // 设置视口范围
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(w, h);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Birthday Card");
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0); // 启动定时器
    glutKeyboardFunc(keyboard); // 注册键盘回调函数
    glutMouseFunc(mouse); // 注册鼠标回调函数
    init();
    glutMainLoop();
    return 0;
}