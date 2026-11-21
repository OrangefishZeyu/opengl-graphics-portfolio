#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#define M_PI 3.14159265358979323846

float fltZoom = 1.0f;
// 定义当前光源索引，初始值为0
int currentLight = 0;
GLfloat rotation = 0.0f;
float carOffset = -2900.0f; // 初始位置设为-3000.0f
float carSpeed = 10.0f; // 可根据需要调整速度

// 全局变量声明
GLfloat eyeX, eyeY, eyeZ;
GLfloat centerX, centerY, centerZ;
GLfloat rotationAngle = 45.0f;
GLfloat distance;

struct image {
    GLint imgWd;
    GLint imgHt;
    GLint pixelLength;
};
image texture[8];
std::vector<GLubyte*> p;
GLuint textures[8];

// 函数声明
void my(void);
void reshapeWindow(GLint intNewWidth, GLint intNewHeight);
void drawCB();
void displayObject(void);
void keyboard(unsigned char key, int x, int y);
void updateEyePosition();

void initLight() {
    // 启用光照
    glEnable(GL_LIGHTING);

    // 启用光源0
    glEnable(GL_LIGHT0);
    // 设置光源0的位置和颜色
    GLfloat light_position0[] = { 0.0, 3000.0, 0.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    GLfloat light_color0[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_color0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_color0);

    // 启用光源1
    glEnable(GL_LIGHT1);
    // 设置光源1的位置
    GLfloat light_position1[] = { 0.0, 1000.0, 0.0, 1.0 }; // 点光源
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    // 设置光源1的颜色
    GLfloat light_color1[] = { 0.3, 0.33, 0.43, 1.0 }; // 黄色
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_color1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_color1);

    // 设置点光源的衰减参数
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.001);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0001);

    // 设置材质属性
    GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    GLfloat mat_shininess[] = { 50.0 };
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // 启用颜色材质
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // 设置深度函数
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    // 设置环境光
    GLfloat ambientLight[] = { 0.3, 0.33, 0.43, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
}


void drawCar() {
    glPushMatrix();
    glTranslatef(carOffset, 75.0, 1000.0);
    glScalef(3.0, 3.0, 3.0); // Scale the car proportionally

    // Lower part of the car
    glBegin(GL_POLYGON);
    glColor3f(0.1, 0.1, 0.8);
    glVertex3f(-40.0f, 0.0f, 15.0f);
    glVertex3f(40.0f, 0.0f, 15.0f);
    glColor3f(1, 0.5, 0.8);
    glVertex3f(40.0f, -15.0f, 15.0f);
    glVertex3f(-40.0f, -15.0f, 15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.3, 0.2, 0.5);
    glVertex3f(40.0f, 0.0f, -15.0f);
    glVertex3f(40.0f, 0.0f, 15.0f);
    glVertex3f(40.0f, -15.0f, 15.0f);
    glVertex3f(40.0f, -15.0f, -15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.3, 0.1, 0.3);
    glVertex3f(-40.0f, 0.0f, -15.0f);
    glVertex3f(-40.0f, 0.0f, 15.0f);
    glVertex3f(-40.0f, -15.0f, 15.0f);
    glVertex3f(-40.0f, -15.0f, -15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.1, 0.1, 0.8);
    glVertex3f(-40.0f, 0.0f, -15.0f);
    glVertex3f(40.0f, 0.0f, -15.0f);
    glColor3f(1, 0.5, 0.8);
    glVertex3f(40.0f, -15.0f, -15.0f);
    glVertex3f(-40.0f, -15.0f, -15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0, 0, 1);
    glVertex3f(-40.0f, 0.0f, 15.0f);
    glVertex3f(-40.0f, 0.0f, -15.0f);
    glVertex3f(40.0f, 0.0f, -15.0f);
    glVertex3f(40.0f, 0.0f, 15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.8, 0.5, 0.2);
    glVertex3f(-40.0f, -15.0f, 15.0f);
    glVertex3f(-40.0f, -15.0f, -15.0f);
    glVertex3f(40.0f, -15.0f, -15.0f);
    glVertex3f(40.0f, -15.0f, 15.0f);
    glEnd();
    // Upper part of the car
    glBegin(GL_POLYGON);
    glColor3f(0, 0, 1);
    glVertex3f(-20.0f, 0.0f, 15.0f);
    glVertex3f(-10.0f, 10.0f, 15.0f);
    glVertex3f(20.0f, 10.0f, 15.0f);
    glVertex3f(25.0f, 0.0f, 15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0, 0, 1);
    glVertex3f(-20.0f, 0.0f, -15.0f);
    glVertex3f(-10.0f, 10.0f, -15.0f);
    glVertex3f(20.0f, 10.0f, -15.0f);
    glVertex3f(25.0f, 0.0f, -15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0, 1, 1);
    glVertex3f(-10.0f, 10.0f, 15.0f);
    glVertex3f(-10.0f, 10.0f, -15.0f);
    glVertex3f(20.0f, 10.0f, -15.0f);
    glVertex3f(20.0f, 10.0f, 15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.8, 0.8);
    glVertex3f(-10.0f, 10.0f, 15.0f);
    glVertex3f(-20.0f, 0.0f, 15.0f);
    glVertex3f(-20.0f, 0.0f, -15.0f);
    glVertex3f(-10.0f, 10.0f, -15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0, 0.5, 0.5);
    glVertex3f(20.0f, 10.0f, 15.0f);
    glVertex3f(20.0f, 10.0f, -15.0f);
    glVertex3f(25.0f, 0.0f, -15.0f);
    glVertex3f(25.0f, 0.0f, 15.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0, 0, 1);
    glVertex3f(-30.0f, -15.0f, 15.0f);
    glVertex3f(-30.0f, -15.0f, -15.0f);
    glVertex3f(30.0f, -15.0f, -15.0f);
    glVertex3f(30.0f, -15.0f, 15.0f);
    glEnd();
    // Wheels
    glColor3f(1, 0, 0);
    glTranslated(-20.0f, -15.0f, 15.0f);
    glutSolidTorus(2, 5, 5, 100);
    glTranslated(0.0f, 0.0f, -30.0f);
    glutSolidTorus(2, 5, 5, 100);
    glTranslated(45.0f, 0.0f, 0.0f);
    glutSolidTorus(2, 5, 5, 100);
    glTranslated(0.0f, 0.0f, 30.0f);
    glutSolidTorus(2, 5, 5, 100);
    glPopMatrix();

}

void myIdleFunction() {
    carOffset += carSpeed;
    if (carOffset > 3000.0f) {
        carOffset = -2950.0f; // 瞬移至-2950.0f重新开始
    }
    glutPostRedisplay(); // 重绘场景
}

void my(void) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_FLAT);
    glEnable(GL_TEXTURE_2D);

    const char* photographs[8] = {
        "texture/rock.bmp",
        "texture/grass.bmp",
        "texture/back1.bmp",
        "texture/right1.bmp",
        "texture/left1.bmp",
        "texture/front1.bmp",
        "texture/top1.bmp",
        "texture/bottom1.bmp"
    };
    for (int i = 0; i < 8; i++) {
        GLubyte* pixeldata;
        FILE* pfile;
        pfile = fopen(photographs[i], "rb");
        if (!pfile) {
            perror(photographs[i]);
            exit(EXIT_FAILURE);
        }
        fseek(pfile, 0x0012, SEEK_SET);
        fread(&texture[i].imgWd, sizeof(texture[i].imgWd), 1, pfile);
        fread(&texture[i].imgHt, sizeof(texture[i].imgHt), 1, pfile);
        texture[i].pixelLength = texture[i].imgWd * 3;
        while (texture[i].pixelLength % 4 != 0)
            texture[i].pixelLength++;
        texture[i].pixelLength *= texture[i].imgHt;
        pixeldata = (GLubyte*)malloc(texture[i].pixelLength);
        if (pixeldata == 0)
            exit(0);
        fseek(pfile, 54, SEEK_SET);
        fread(pixeldata, texture[i].pixelLength, 1, pfile);
        p.push_back(pixeldata);
        fclose(pfile);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &textures[i]);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, texture[i].imgWd, texture[i].imgHt, 0, 0x80E0 /* GL_BGR */, GL_UNSIGNED_BYTE, p[i]);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

    // 初始化视角
    eyeX = 2000.0f * fltZoom;
    eyeY = 1000.0f * fltZoom;
    eyeZ = 2000.0f * fltZoom;
    centerX = 0.0f * fltZoom;
    centerY = 800.0f * fltZoom;
    centerZ = 0.0f * fltZoom;

    GLfloat offsetX = eyeX - centerX;
    GLfloat offsetZ = eyeZ - centerZ;
    distance = sqrt(offsetX * offsetX + offsetZ * offsetZ);

    initLight();
}

void reshapeWindow(GLint intNewWidth, GLint intNewHeight) {
    glViewport(0, 0, intNewWidth, intNewHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, 1, 0.1, 10000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void drawSky() {

    //Top Face
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, textures[6]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3000.1f, 5000.1f, -3000.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-3000.1f, 5000.1f, 3000.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(3000.1f, 5000.1f, 3000.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(3000.1f, 5000.1f, -3000.1f);
    glEnd();

    // Bottom Face
    glBindTexture(GL_TEXTURE_2D, textures[7]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3000.1f, -1000.1f, -3000.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-3000.1f, -1000.1f, -3000.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(3000.1f, -1000.1f, 3000.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(3000.1f, -1000.1f, 3000.1f);
    glEnd();

    // front face
    glBindTexture(GL_TEXTURE_2D, textures[5]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3000.1f, -1000.1f, -3000.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-3000.1f, 5000.1f, -3000.10f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(3000.1f, 5000.1f, -3000.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(3000.1f, -1000.1f, -3000.1f);
    glEnd();

    //back Face
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3000.1f, -1000.1f, 3000.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-3000.1f, 5000.1f, 3000.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(3000.1f, 5000.1f, 3000.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(3000.1f, -1000.1f, 3000.1f);
    glEnd();

    // Right face
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(3000.0f, -1000.0f, -3000.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(3000.0f, 5000.0f, -3000.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(3000.0f, 5000.0f, 3000.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(3000.0f, -1000.0f, 3000.0f);
    glEnd();

    // Left Face
    glBindTexture(GL_TEXTURE_2D, textures[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3000.1f, -1000.1f, 3000.1f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-3000.1f, 5000.1f, 3000.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-3000.1f, 5000.1f, -3000.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-3000.1f, -1000.1f, -3000.1f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawRoad() {
    glPushMatrix();
    glColor3f(0.3, 0.3, 0.3);
    glTranslatef(0.01, 10.01, 1000.01);
    glScalef(6000.99, 10.99, 300.99);
    glutSolidCube(1);
    glColor3f(1, 1, 1);
    glutWireCube(1);
    glPopMatrix();
}

void christmasTree() {
    glPushMatrix();
    glTranslatef(1300, 300, 0);
    glScalef(200, 200, 200);

    // 绘制树冠（锥体）
    glColor3f(0, 1, 0); // 绿色
    glPushMatrix();
    glTranslatef(0, -0.4, 0); // 移动到树冠下方
    glScalef(2, 1.5, 2); // 高一点的锥体
    glRotatef(270, 1, 0, 0); // 绕x轴旋转90度
    glutSolidCone(0.5, 0.8, 20, 20);
    glPopMatrix();

    glColor3f(0, 1, 0); // 绿色
    glPushMatrix();
    glTranslatef(0, 0.5, 0);
    glScalef(2, 1.5, 2); // 高一点的锥体
    glRotatef(270, 1, 0, 0); // 绕x轴旋转90度
    glutSolidCone(0.3, 0.6, 20, 20);
    glPopMatrix();

    glColor3f(0, 1, 0); // 绿色
    glPushMatrix();
    glTranslatef(0, 1.2, 0);
    glScalef(2, 1.5, 2); // 高一点的锥体
    glRotatef(270, 1, 0, 0); // 绕x轴旋转90度
    glutSolidCone(0.2, 0.6, 20, 20);
    glPopMatrix();

    // 绘制树干（圆柱体）
    glColor3f(1, 0.5, 0); // 棕色
    glPushMatrix();
    glTranslatef(0, -2.5, 0); // 移动到树冠下方
    glScalef(0.2, 0.2, 0.2); // 细长的圆柱
    glRotatef(270, 1, 0, 0); // 绕x轴旋转90度
    glutSolidCylinder(1, 10, 20, 20);
    glPopMatrix();
    glPopMatrix();
}

void star(GLfloat angle, GLfloat r, GLfloat x, GLfloat y, GLfloat z)
{
    // 保存当前矩阵状态
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, 1, 0, 0);
    glColor3f(205.0 / 255.0, 195.0 / 255.0, 0);
    glScalef(8 * r / sqrt(3), 8 * r, 8 * r);
    glutSolidTetrahedron();
    glColor3f(1, 1.0, 1);
    glutWireTetrahedron();
    glRotatef(180, -1, 0, 0);
    glRotatef(180, 0, -1, 0);
    glColor3f(1, 245.0 / 255.0, 0);
    glutSolidTetrahedron();
    glColor3f(1, 1, 1);
    glutWireTetrahedron();
    glPopMatrix();
}

void drawCB() {
    //Draw the grass and its texture
    glColor3f(0.52, 0.32, 0.63);
    glPushMatrix();
    glScalef(1399.01, 29.01, 1399.01);
    glPushMatrix();
    glColor3f(0.81, 0.81, 0.81);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(2999.99, 11, -2999.99);
    glTexCoord2f(0, 100); glVertex3f(2999.99, 11, 2999.99);
    glTexCoord2f(100, 100); glVertex3f(-2999.99, 11, 2999.99);
    glTexCoord2f(100, 0); glVertex3f(-2999.99, 11, -2999.99);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //Draw the ground and its texture of the cb ground
    glColor3f(1, 0, 0);
    glLineWidth(1);
    glPushMatrix();
    glScalef(5999.01, 10.1, 5999.01);
    glPushMatrix();
    glColor3f(0.53, 0.53, 0.53);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(799.99, 18, -799.99);
    glTexCoord2f(0, 6.5); glVertex3f(799.99, 18, 799.99);
    glTexCoord2f(6.5, 6.5); glVertex3f(-799.99, 18, 799.99);
    glTexCoord2f(6.5, 0); glVertex3f(-799.99, 18, -799.99);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //Blue railing on the second floor
    glPushMatrix();
    glColor3f(0.27, 0.509, 0.705);
    glTranslatef(0, 240.01, 0);
    glScalef(780.01, 30.01, 780.01);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();

    //The big frame of cb
    glPushMatrix();
    glColor3f(0.61, 0.61, 0.61);
    glScalef(751.01, 1201.01, 751.01);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();

    //Top floor connection
    glPushMatrix();
    glColor3f(0.6, 0.6, 0.6);
    glTranslatef(10.01, 975.01, 0);
    glScalef(130.01, 150.01, 750.01);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.27, 0.509, 0.705);
    glTranslatef(-232.5, 585, 0);
    glScalef(300.01, 30.01, 780.01);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.6, 0.6, 0.6);
    glTranslatef(-210, 900, 0);
    glScalef(300, 300, 750);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();

    //Blue railing
    glPushMatrix();
    glColor3f(0.27, 0.509, 0.705);
    glRotatef(-25, 0, 0, 1);
    glTranslatef(-75.01, 492.01, 0);
    glScalef(504.01, 30.01, 780.01);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.27, 0.509, 0.705);
    glTranslatef(225.01, 825.01, 0);
    glScalef(300.01, 450.01, 750.01);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.27, 0.509, 0.705);
    glTranslatef(-210.01, 750.01, 0);
    glScalef(300.01, 30.01, 780.01);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();


    glPushMatrix();
    glColor3f(0.27, 0.509, 0.705);
    glTranslatef(-60.01, 900.01, 2.01);
    glScalef(15.01, 300.01, 785.01);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.27, 0.509, 0.705);
    glTranslatef(0, 585.01, -210.01);
    glScalef(770.01, 30.01, 360.01);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.27, 0.509, 0.705);
    glRotatef(28, 1, 0, 0);
    glTranslatef(240.01, 498.01, -75.01);
    glScalef(300.01, 30.01, 450.01);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();

    //The red sidebar outside cb
    glPushMatrix();
    glColor3f(0.4f, 0.0f, 0.2f);
    glRotatef(70, 1, 0, 0);
    glTranslatef(386.01, 400.01, -730.01);
    glScalef(18.01, 25.01, 470.01);
    glutSolidCube(1);
    glColor3f(0.2f, 0.0f, 0.0f);
    glutWireCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.4f, 0.0f, 0.2f);
    glRotatef(100, 1, 0, 0);
    glTranslatef(386.01, -250.01, -795.01);
    glScalef(18.01, 25.01, 450.01);
    glutSolidCube(1);
    glColor3f(0.2f, 0.0f, 0.0f);
    glutWireCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.4f, 0.0f, 0.2f);
    glRotatef(85, 1, 0, 0);
    glTranslatef(-386.01, 0.51, -420.01);
    glScalef(18.01, 25.01, 310.01);
    glutSolidCube(1);
    glColor3f(0.2f, 0.0f, 0.0f);
    glutWireCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.4f, 0.0f, 0.2f);
    glRotatef(100, 1, 0, 0);
    glTranslatef(-386.01, 0.51, -420.01);
    glScalef(18.01, 25.01, 310.01);
    glutSolidCube(1);
    glColor3f(0.2f, 0.0f, 0.0f);
    glutWireCube(1);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90.0, 1.01, 0, 0);
    glColor3f(0.41, 0.41, 0.41);
    glTranslatef(-270.01, 150.01, -750.01);
    glScalef(1, 1, 3);
    glutSolidCylinder(61.01, 201.01, 101.01, 11.01);
    glPopMatrix();

    //The sidebar at the top level of cb
    glPushMatrix();
    glColor3f(0.27, 0.509, 0.705);
    glTranslatef(-345.01, 900.01, 15.01);
    glScalef(15.01, 300.01, 750.01);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();

    // 红色正方体，x=500
    //glPushMatrix();
    //glTranslatef(1000.0f, 0.0f, 0.0f);
    //glColor3f(1.0f, 0.0f, 0.0f); // 红色
    //glutSolidCube(500.0f); // 绘制边长为1的正方体
    //glPopMatrix();

    //// 绿色正方体，y=500
    //glPushMatrix();
    //glTranslatef(0.0f, 1000.0f, 0.0f);
    //glColor3f(0.0f, 1.0f, 0.0f); // 绿色
    //glutSolidCube(500.0f);
    //glPopMatrix();

    //// 蓝色正方体，z=500
    //glPushMatrix();
    //glTranslatef(0.0f, 0.0f, 1000.0f);
    //glColor3f(0.0f, 0.0f, 1.0f); // 蓝色
    //glutSolidCube(500.0f);
    //glPopMatrix();

    // Draw the steps along the X-axis
    glColor3f(0.7, 0.7, 0.7); // Step color
    for (int i = 0; i < 9; i++) {
        float zPos = 380.0f + i * 25.0f;
        float height = 220.0f - i * 22.0f;
        glPushMatrix();
        glTranslatef(0.0f, height / 2.0f, zPos);
        glScalef(500.0f, height, 25.0f);

        // Draw solid steps
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f);
        glColor3f(0.7, 0.7, 0.7);
        glutSolidCube(1);
        glDisable(GL_POLYGON_OFFSET_FILL);

        // Drawing the border
        glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);
        glEnable(GL_LINE_STIPPLE);
        glLineWidth(1.0);
        glColor3f(0, 0, 0);
        glutWireCube(1);
        glDisable(GL_LINE_STIPPLE);
        glPopAttrib();

        glPopMatrix();
    }

    // Draw the steps along the X-axis
    glColor3f(0.7, 0.7, 0.7); // Step color
    for (int i = 0; i < 9; i++) {
        float xPos = 380.0f + i * 25.0f;
        float height = 220.0f - i * 22.0f;
        glPushMatrix();
        glTranslatef(xPos, height / 2.0f, 0.0f);
        glScalef(25.0f, height, 500.0f);

        // Draw solid steps
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f);
        glColor3f(0.7, 0.7, 0.7);
        glutSolidCube(1);
        glDisable(GL_POLYGON_OFFSET_FILL);

        // Drawing the border
        glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);
        glEnable(GL_LINE_STIPPLE);
        glLineWidth(1.0);
        glColor3f(0, 0, 0);
        glutWireCube(1);
        glDisable(GL_LINE_STIPPLE);
        glPopAttrib();

        glPopMatrix();
    }


}

void streetlight() {
    // 第一组灯：y 和 z 坐标不变，x 从 -2900 到 2900，每隔 1160
    const float y1 = 200.0f;
    const float z1 = 1155.0f;
    for (float x = -2900.0f; x <= 2900.0f; x += 1160.0f) {
        glPushMatrix();
        glTranslatef(x, y1, z1);

        // 灯杆
        glPushMatrix();
        glScalef(20.0f, 400.0f, 20.0f); // 缩放为细长的圆柱体
        glColor3f(0.5f, 0.5f, 0.5f); // 灰色
        glutSolidCube(1.0f); // 使用立方体近似圆柱体
        glPopMatrix();

        // 移动到灯杆顶部
        glPushMatrix();
        glTranslatef(0.0f, 400.0f / 2, 0.0f); // 灯杆高度为400.0f，立方体中心在底部，所以向上移动200.0f

        // 绘制黄色球体
        glColor3f(1.0f, 1.0f, 0.0f); // 黄色
        glutSolidSphere(30.0f, 20, 20); // 半径30.0f，细分20

        // 绘制黑色圆台
        glPushMatrix();
        glTranslatef(0.0f, 30.0f, 0.0f); // 移动到球体顶部
        glRotatef(270.0f, 1.0f, 0.0f, 0.0f); // 绕X轴旋转90度，使圆台沿Y轴
        glColor3f(0.0f, 0.55f, 0.55f); // 黑色
        GLUquadricObj* quad = gluNewQuadric();
        gluCylinder(quad, 60.0f, 40.0f, 30.0f, 20, 20); // baseRadius=60, topRadius=40, height=30
        gluDeleteQuadric(quad);
        glPopMatrix();

        glPopMatrix();
        glPopMatrix();
    }

    // 第二组灯：z = 850，x 从 -2320 到 2320，每隔 1160
    const float z2 = 850.0f;
    for (float x = -2320.0f; x <= 2320.0f; x += 1160.0f) {
        glPushMatrix();
        glTranslatef(x, y1, z2);

        // 灯杆
        glPushMatrix();
        glScalef(20.0f, 400.0f, 20.0f); // 缩放为细长的圆柱体
        glColor3f(0.5f, 0.5f, 0.5f); // 灰色
        glutSolidCube(1.0f); // 使用立方体近似圆柱体
        glPopMatrix();

        // 移动到灯杆顶部
        glPushMatrix();
        glTranslatef(0.0f, 400.0f / 2, 0.0f); // 灯杆高度为400.0f，立方体中心在底部，所以向上移动200.0f

        // 绘制黄色球体
        glColor3f(1.0f, 1.0f, 0.0f); // 黄色
        glutSolidSphere(30.0f, 20, 20); // 半径30.0f，细分20

        // 绘制黑色圆台
        glPushMatrix();
        glTranslatef(0.0f, 30.0f, 0.0f); // 移动到球体顶部
        glRotatef(270.0f, 1.0f, 0.0f, 0.0f); // 绕X轴旋转90度，使圆台沿Y轴
        glColor3f(0.0f, 0.55f, 0.55f); // 黑色
        GLUquadricObj* quad = gluNewQuadric();
        gluCylinder(quad, 60.0f, 40.0f, 30.0f, 20, 20); // baseRadius=60, topRadius=40, height=30
        gluDeleteQuadric(quad);
        glPopMatrix();

        glPopMatrix();
        glPopMatrix();
    }
}



void displayObject(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0, 1.0, 0);
    glEnable(GL_DEPTH_TEST);

    drawCB();
    streetlight();
    drawRoad();
    drawCar();
    star(rotation, 15.0, 1300, 780, 0);
    // 增加旋转角度
    rotation += 1.0f;
    christmasTree();
    drawSky();

    glutSwapBuffers();
    glLoadIdentity();
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
        rotationAngle += 5.0f;
        rotationAngle = fmod(rotationAngle, 360.0f);
        updateEyePosition();
        glutPostRedisplay();
        break;
    case 'd':
        rotationAngle -= 5.0f;
        rotationAngle = fmod(rotationAngle, 360.0f);
        updateEyePosition();
        glutPostRedisplay();
        break;
    case 'w':
        fltZoom -= 0.1f; // 缩小视角
        if (fltZoom < 0.2f) fltZoom = 0.2f;
        updateEyePosition();
        glutPostRedisplay();
        break;
    case 's':
        fltZoom += 0.1f; // 放大视角
        if (fltZoom > 2.0f) fltZoom = 2.0f;
        updateEyePosition();
        glutPostRedisplay();
        break;



        // 在按键处理函数中
    case 'l':
        // 切换当前光源索引
        currentLight = (currentLight + 1) % 2;

        // 禁用当前启用的光源
        if (currentLight == 0) {
            glDisable(GL_LIGHT1);  // 禁用GL_LIGHT1
            glEnable(GL_LIGHT0);   // 启用GL_LIGHT0
        }
        else {
            glDisable(GL_LIGHT0);  // 禁用GL_LIGHT0
            glEnable(GL_LIGHT1);   // 启用GL_LIGHT1
        }

        // 确保光照功能启用
        glEnable(GL_LIGHTING);

        // 刷新显示
        glutPostRedisplay();
        break;
    }
}


void updateEyePosition() {
    GLfloat radians = rotationAngle * (M_PI / 180.0f);
    eyeX = centerX + distance * cos(radians) * fltZoom;
    eyeZ = centerZ + distance * sin(radians) * fltZoom;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(150, 125);
    glutInitWindowSize(700, 500);
    glutCreateWindow("XJTLU");
    my();
    glutReshapeFunc(reshapeWindow);
    glutDisplayFunc(displayObject);
    glutIdleFunc(myIdleFunction);
    glutKeyboardFunc(keyboard); // 注册键盘事件处理函数
    glutMainLoop();
}
