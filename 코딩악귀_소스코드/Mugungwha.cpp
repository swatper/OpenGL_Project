#include <glut.h>
#include <glu.h>
#include <gl.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <windows.h>//소리재생 헤더

#define GL_PI 3.1415f

using namespace std;
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

static GLfloat keyXRot = 0.0f;
static GLfloat KeyYRot = 0.0f;

GLfloat recordTime = 0.0f;
GLfloat trunSpeed = 10.0f;
GLfloat waitTime = 0.0f;
GLfloat walkDis = 0.0f;
GLfloat OW = 800, OH = 600;
GLfloat NW, NH;
GLfloat legAngle = 0.0f, angle = 15.0f;
GLfloat wave = 0.05f;
GLfloat headSpin = 0.0f;
GLfloat fireRange = 0.0f; //불꽃 폭발 범위
GLfloat fireX, fireY, fireAngle;
GLfloat fireColor[7][3] = {
							{1.0,0.0,0.0},//빨
							{1.0,0.2,0.0},//주
							{1.0,1.0,0.0},//노
							{0.0,1.0,0.0},//초
							{0.0,0.0,1.0},//파
							{0.0,0.1,1.0},//남
							{0.5,0.0,1.0} //보
};
GLfloat fireXsize = 5.0f;
GLfloat fireYsize = 5.0f;
GLfloat face[3] = { 1, 0.9, 0.7 };//모카신색
GLfloat sullaePos[3] = { 0.0f, -2.0f, -50.0f };
GLfloat playerPos[3] = { 0.0f, 0.8f, 0.0f };
GLfloat cameraPos[3] = { 0.0f, 0.8f, 0.0f };
GLfloat cameraRotY = 0.0f;

//화면크기 모드 상태 값
int displaymode = 0;

GLint rateTime[3][3] = { {20,30,40},{40,50,60},{50,65,80} };
GLint isEnding = 0;
GLint fireMoveX = 0;
GLint fireCount = 0;
GLint fireColorIndex = 0;
GLint wordIndex = 0;
GLint word[10] = { 0 };		//"무궁화 꽃이 피었습니다"
GLint diff = 0;				// 0쉬움 , 1보통 , 2어려움
GLint currentCamera = 1;	// 1인칭, 2인칭, 3인칭
GLint endingAnimation = 0;	//1~2 : dead Scene, 3~5 : ending Scene


GLboolean isTurn = false;
GLboolean isChance = false;
GLboolean isGameOver = false;
GLboolean isStart = false;
GLboolean isTouch = false;
GLboolean canRetry = false;

//일반 함수
void GameManual_1();
void GameManual_2();
void GameStart();
void PlayerWalk(int Vec);
void Consolescreen();
void Retry();

//일반 그래픽 함수
void MenuProc(int entryID);
void MoveCamera();
void DrawUI();
void SetRC();
void DrawSullae(GLUquadricObj* sullae);
void DrawPlayer(GLUquadricObj* player);
void DrawFloor();
void DrawVec();
void DrawTree(GLfloat x, GLfloat y, GLfloat z);
void DrawAgent(GLfloat x, GLfloat y, GLfloat z, GLint num);
void Firework();



//콜백함수
void RenderScene();
void ChangeSize(int w, int h);
void SpecialKeys(int Key, int x, int y);
void MouseClick(GLint Button, GLint State, GLint X, GLint Y);
void Keyboard(unsigned char Key, int x, int y);
void MenuFunc();
void Timer(int value);

int main(int argv, char** argc) {
	glutInit(&argv, argc);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(OW, OH);
	glutCreateWindow("MuGungFlower");
	glClearColor(0.2, 0.4, 0.8, 1.0);

	srand((unsigned)time(NULL));
	MenuFunc();
	Consolescreen();
	SetRC();
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseClick);
	glutDisplayFunc(RenderScene);
	
	glutMainLoop();
	return 0;
}

void PlayerWalk(int vec) {
	//걷는 소리 넣기
	walkDis += (0.18 * vec);
	if (legAngle > 80.0f || legAngle < -80.0f)
		angle *= -1;
	if (playerPos[1] > 0.9f || playerPos[1] < 0.7f)
		wave *= -1;
	legAngle += angle;
	playerPos[1] += wave;
}

void DrawFloor() {
	glPushMatrix();
	//stipple 구현
	GLubyte grass[] = {	0x00,	0x00,	0x00,	0x00,
						0x10,	0x00,	0x00,	0x00,
						0x01,	0x00,	0x00,	0x00,
						0x09,	0x80,	0x00,	0x00,
						0xC9,	0x80,	0x00,	0x11,
						0x8D,	0xC0,	0x00,	0x0E,
						0x0F,	0x80,	0x00,	0x00,
						0x07,	0x00,	0x00,	0x00,
						0x02,	0x00,	0x00,	0x00,
						0x00,	0x00,	0x01,	0x00,
						0x00,	0x00,	0x09,	0x80,
						0x00,	0x08,	0x09,	0x80,
						0x00,	0x08,	0x14,	0xC0,
						0x00,	0x08,	0x0F,	0x80,
						0x00,	0x38,	0x07,	0x00,
						0xC0,	0x00,	0x20,	0x11,
						0x80,	0x00,	0x20,	0x11,
						0x01,	0x00,	0x20,	0x0E,
						0x01,	0x00,	0xE0,	0x00,
						0x03,	0x00,	0x00,	0x00,
						0x03,	0x00,	0x00,	0x00,
						0x03,	0x20,	0x00,	0x00,
						0x03,	0x31,	0x00,	0x00,
						0x03,	0x31,	0x02,	0x00,
						0x01,	0xF1,	0x03,	0x00,
						0x01,	0xF4,	0x01,	0x09,
						0x00,	0xE7,	0x01,	0x8D,
						0x80,	0x00,	0x00,	0x8F,
						0xF0,	0x00,	0x00,	0xF9,
						0x70,	0x00,	0x00,	0x70,
						0x00,	0x00,	0x00,	0x00,
						0x00,	0x00,	0x00,	0x00


	};
	GLubyte flower[] = {	0x00,	0x0F,	0x80,	0x00,
							0x00,	0x50,	0x48,	0x00,
							0x00,	0x20,	0x60,	0x00,
							0x00,	0x60,	0x30,	0x00,
							0x00,	0xC0,	0x18,	0x00,
							0x00,	0xC0,	0x18,	0x00,
							0x00,	0x80,	0x18,	0x00,
							0x3F,	0x80,	0x18,	0x00,
							0x7F,	0x80,	0x08,	0x00,
							0x40,	0x3F,	0xEF,	0xFF,
							0xC0,	0x7F,	0xE7,	0xFE,
							0xC0,	0x7F,	0xF0,	0x1F,
							0xC0,	0x7F,	0xF0,	0x0F,
							0xC0,	0x7F,	0xF0,	0x07,
							0xC0,	0x7F,	0xF0,	0x07,
							0xC0,	0x7F,	0xF0,	0x03,
							0xC0,	0x7F,	0xF0,	0x03,
							0xC0,	0x7F,	0xF0,	0x02,
							0xC0,	0x7F,	0xF0,	0x06,
							0x40,	0x7F,	0xF0,	0x06,
							0x60,	0x7F,	0xF0,	0x0C,
							0x30,	0x7F,	0xF0,	0x0C,
							0x1F,	0xBF,	0xE0,	0x18,
							0x0F,	0x80,	0x1F,	0xF8,
							0x00,	0xC0,	0x1F,	0xE0,
							0x00,	0xC0,	0x30,	0x00,
							0x00,	0xC0,	0x20,	0x00,
							0x00,	0xC0,	0x60,	0x00,
							0x00,	0xC0,	0x60,	0x00,
							0x00,	0x40,	0xC0,	0x00,
							0x00,	0x7F,	0x80,	0x00,
							0x00,	0x3F,	0x80,	0x00

	};

	glEnable(GL_POLYGON_STIPPLE);
	
	//바닥, stipple 그리기
	glPolygonStipple(grass);
	glBegin(GL_POLYGON);
	glColor3f((GLfloat)71 / 255, (GLfloat)200 / 255, (GLfloat)12 / 255);
	glVertex3f(-1000.0f, -5.0f, -3000.0f);
	glVertex3f(-1000.0f, -5.0f, 3000.0f);
	glVertex3f(1000.0f, -5.0f, 3000.0f);
	glVertex3f(1000.0f, -5.0f, -3000.0f);
	glEnd();
	
	//꽃
	glPolygonStipple(flower);
	glBegin(GL_POLYGON);
	glColor3f((GLfloat)255 / 255, (GLfloat)178 / 255, (GLfloat)245 / 255);
	glVertex3f(-3 + 10, -4.0f, -1000);
	glVertex3f(-3 + 10, -4.0f, 1000);
	glVertex3f(500 + 10, -4.0f, 1000);
	glVertex3f(500 + 10, -4.0f, -1000);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f((GLfloat)250 / 255, (GLfloat)237 / 255, (GLfloat)125 / 255);
	glVertex3f(-500 - 10 , -4.0f, -1000);
	glVertex3f(-500 - 10 , -4.0f, 1000);
	glVertex3f(3 - 10 , -4.0f, 1000);
	glVertex3f(3 - 10, -4.0f, -1000);
	glEnd();

	glDisable(GL_POLYGON_STIPPLE);

	
	//바닥의 밑면
	glColor3f((GLfloat)71/255, (GLfloat)102/255, (GLfloat)12/255);
	glBegin(GL_POLYGON);
	glVertex3f(-1000.0f, -5.0f, -3000.0f);
	glVertex3f(-1000.0f, -5.0f, 3000.0f);
	glVertex3f(1000.0f, -5.0f, 3000.0f);
	glVertex3f(1000.0f, -5.0f, -3000.0f);
	glEnd();
	for (int i = -500; i < 500; i += 7) {
		int temp = i % 2 == 0 ? 30 : 0;
		DrawTree(i, -2, -180+temp);
	}
	for (int i = -130; i < 250; i += 7) {
		int temp = i % 2 == 0 ? 30 : 0;
		DrawTree(-150+temp, -2, i);
	}
	for (int i = -130; i < 250; i += 7) {
		int temp = i % 2 == 0 ? 30 : 0;
		DrawTree(150+temp, -2, i);
	}
	for (int i = -500; i < 500; i += 7) {
		int temp = i % 2 == 0 ? 300 : 0;
		DrawTree(i, -2, 150 + temp);
	}
	
	if (!isEnding) {
		if (diff > 1)
			DrawTree(13, -2, -100);

		if (diff > 0)
			DrawTree(-15, -2, -60);

		DrawTree(12, -2, -25);
	}
	glColor3f(1,0,0);
	glBegin(GL_POLYGON);
	glVertex3f(-100.0f, -3.9f, sullaePos[2]-20);
	glVertex3f(-100.0f, -3.9f, sullaePos[2]+5);
	glVertex3f(100.0f, -3.9f, sullaePos[2] + 5);
	glVertex3f(100.0f, -3.9f, sullaePos[2] - 20);
	glEnd();

	DrawTree(sullaePos[0], sullaePos[1], sullaePos[2]);

	glPopMatrix();
}

void DrawPlayer(GLUquadricObj* player){
	if (endingAnimation!=0&&(endingAnimation < 3))
		return;
	glPushMatrix();
	glTranslatef(0.0f, -2.0f, -walkDis);
	glRotatef(180, 0, 1, 0);
	glRotatef(keyXRot, 1.0, 0.0, 0.0);
	glRotatef(KeyYRot, 0.0, 1.0, 0.0);

	//왼 다리
	glPushMatrix();
	glColor3f(0.2f, 0.2f, 0.6f);
	glTranslatef(-0.5, 0, -0.25);
	glRotatef(90 - legAngle, 1, 0, 0);
	gluCylinder(player, 0.2, 0.2, 2.0, 26, 13);
	//왼발
	glPushMatrix();
	glTranslatef(0.0, -0.2, 1.8);
	glRotated(-90.0f, 1, 0, 0);
	glTranslatef(0, 0, 0.2);
	glColor3f(0.0, 0.0, 0.0);
	glutSolidCube(0.5);
	glTranslatef(0, 0, 0.3);
	glutSolidCube(0.5);
	glPopMatrix();
	glPopMatrix();

	//오른다리
	glPushMatrix();
	glColor3f(0.2f, 0.2f, 0.6f);
	glTranslatef(0.5, 0, -0.25);
	glRotatef(90 + legAngle, 1, 0, 0);
	gluCylinder(player, 0.2, 0.2, 2.0, 26, 13);
	//오른발
	glPushMatrix();
	glTranslatef(0.0, -0.2, 1.8);
	glRotated(-90.0f, 1, 0, 0);
	glTranslatef(0, 0, 0.2);
	glColor3f(0.0, 0.0, 0.0);
	glutSolidCube(0.5);
	glTranslatef(0, 0, 0.3);
	glutSolidCube(0.5);
	glPopMatrix();
	glPopMatrix();

	//몸통
	glPushMatrix();
		glDisable(GL_CULL_FACE);
		glRotatef(90, 1, 0, 0);
		glTranslatef(0, -0.25, 0);
		glColor3f(0.0f,0.9f,1.0f);
		glFrontFace(GL_CW);                                        //반시계방향이 앞면이다. *CW = 시계방향이 앞면
		gluDisk(player, 0, 0.75, 26, 13);
		glRotatef(-90, 1, 0, 0);
		glTranslatef(0, 1.8, 0);
		glRotatef(90, 1, 0, 0);
		glFrontFace(GL_CCW);                                        //반시계방향이 앞면이다. *CW = 시계방향이 앞면
		gluDisk(player, 0, 0.75, 26, 13);
		glEnable(GL_CULL_FACE);
		gluCylinder(player, 0.75, 0.75, 2.0, 26, 13);
	glPopMatrix();

	//왼팔
	glPushMatrix();
	glColor3f(0.0f, 0.9f, 1.0f);
	
	//왼쪽 상박
	glTranslatef(0.75, 1.5, 0.0);
	glRotatef(legAngle, -1, 0, 0);
	//ending
	glRotatef(-isEnding, 1, 0.5, 0);
	glutSolidSphere(0.2, 10, 10);//어깨
	glRotatef(90.0f, 1, 0.2, 0.3);
	glDisable(GL_CULL_FACE);
	gluCylinder(player, 0.15, 0.15, 0.8, 26,13);

	//왼쪽 하박
	glTranslatef(0.03, 0.0, 0.7);
	//ending
	glRotatef(isEnding, 1, 0, 0);
	glutSolidSphere(0.15, 10, 10);//팔꿈치(?)
	glRotatef(-105, 1, 0, 0);
	gluCylinder(player, 0.15, 0.15, 1.2, 26, 13);
	//왼손
	glColor3f(1.0, 0.9, 0.7);
	glTranslatef(0.01, 0.0, 1.3);
	glutSolidSphere(0.2, 5, 10);
	glPopMatrix();

	//오른팔
	glPushMatrix();
	glColor3f(0.0f, 0.9f, 1.0f);
	//오른쪽 상박
	glTranslatef(-0.75, 1.5, 0.0);
	glRotatef(legAngle, 1, 0, 0);
	glutSolidSphere(0.2, 10, 10);//어깨(?)
	glRotatef(90.0f, 1, -0.2, -0.3);
	glDisable(GL_CULL_FACE);
	gluCylinder(player, 0.15, 0.15, 0.8, 26, 13);

	//오른쪽 하박
	glTranslatef(0.03, 0.0, 0.7);
	glutSolidSphere(0.15, 10, 10);//팔꿈치(?)
	glRotatef(-105, 1, 0, 0);
	gluCylinder(player, 0.15, 0.15, 1.2, 26, 13);

	//오른손
	glColor3f(1.0, 0.9, 0.7);
	glTranslatef(0.01, 0.0, 1.3);
	glutSolidSphere(0.2, 5, 10);
	glPopMatrix();

	//머리
	glPushMatrix();
	//머리통
	glColor3f(1, 0.9, 0.7);
	glTranslatef(0, 2.45, -0.25);
	glRotatef(0, 0.0, 1.0, 0.0);
	gluSphere(player, 0.7f, 26, 13);

	//눈
	glColor3f(0, 0, 0);
	glTranslatef(0.2f, 0.25f, 0.7f);
	gluSphere(player, 0.05f, 26, 13);//왼쪽 눈
	glColor3f(0, 0, 0);
	glTranslatef(-0.4f, 0, 0);
	gluSphere(player, 0.05f, 26, 13);//오른쪽 눈

	//입
	glColor3f(1.0, 0, 0);
	glTranslatef(0.2f, -0.5f, -0.25f);
	gluSphere(player, 0.2f, 26, 13);
	glPopMatrix();
	glPopMatrix();
}

void DrawVec() {
	//R = x축, G = y축, B = Z축
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(1000, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1000, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1000);
	glEnd();
}

void DrawSullae(GLUquadricObj* sullae) {
	glPushMatrix();
	glTranslatef(sullaePos[0], sullaePos[1], sullaePos[2]);
	glRotatef(180, 0, 1, 0);
	glRotatef(keyXRot, 1.0, 0.0, 0.0);
	glRotatef(KeyYRot, 0.0, 1.0, 0.0);

	//왼 다리
	glPushMatrix();
	glTranslatef(-0.4, 0, -0.25);
	glRotatef(90, 1, 0, 0);
	glColor3f(1.0f, 1.0f, 1.0f);
	gluCylinder(sullae, 0.23, 0.18, 2.0, 26, 13);
	//왼발
	glPushMatrix();
	glTranslatef(0.0, -0.2, 1.8);
	glRotated(-90.0f, 1, 0, 0);
	//신발
	glTranslatef(0, 0, 0.2);
	glColor3f(0.0, 0.0, 0.0);
	glutSolidCube(0.5);
	glTranslatef(0, 0, 0.3);
	glutSolidCube(0.5);
	glPopMatrix();
	glPopMatrix();

	//오른다리
	glPushMatrix();
	glTranslatef(0.4, 0, -0.25);
	glRotatef(90, 1, 0, 0);
	glColor3f(1.0f, 1.0f, 1.0f);
	gluCylinder(sullae, 0.23, 0.18, 2.0, 26, 13);
	//오른발
	glPushMatrix();
	glTranslatef(0.0, -0.2, 1.8);
	glRotated(-90.0f, 1, 0, 0);
	glTranslatef(0, 0, 0.2);
	glColor3f(0.0, 0.0, 0.0);
	glutSolidCube(0.5);
	glTranslatef(0, 0, 0.3);
	glutSolidCube(0.5);
	glPopMatrix();
	glPopMatrix();

	//몸통
	glPushMatrix();
	glDisable(GL_CULL_FACE);
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, -0.25, 0);
	glColor3f(1.0f, 0.5f, 0.0f);
	glFrontFace(GL_CW);                                        //반시계방향이 앞면이다. *CW = 시계방향이 앞면
	gluDisk(sullae, 0, 0.75, 26, 13);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 1.8, 0);
	glRotatef(90, 1, 0, 0);
	glFrontFace(GL_CCW);                                        //반시계방향이 앞면이다. *CW = 시계방향이 앞면
	gluDisk(sullae, 0, 0.75, 26, 13);
	glEnable(GL_CULL_FACE);
	gluCylinder(sullae, 0.75, 0.7, 2.0, 26, 13);
	//치마(?)
	gluCylinder(sullae, 0.2, 1.0, 2.3, 26, 13);
	glPopMatrix();

	//왼팔
	glPushMatrix();

	//왼쪽 상박
	glTranslatef(0.75, 1.5, 0.0);
	glutSolidSphere(0.3, 10, 10);//어깨(?)
	glRotatef(-68.0f, 1, -1, 1);
	glDisable(GL_CULL_FACE);
	glColor3f(1.0, 0.9, 0.7);
	gluCylinder(sullae, 0.12, 0.12, 0.8, 26, 13);

	//왼쪽 하박
	glTranslatef(0.03, 0.0, 0.7);
	glutSolidSphere(0.15, 10, 10);//팔꿈치(?)
	glRotatef(-85.0f, -1, 15, 1);
	gluCylinder(sullae, 0.12, 0.12, 1.2, 26, 13);

	//왼손
	glTranslatef(0.01, 0.0, 1.3);
	glutSolidSphere(0.2, 5, 10);
	glPopMatrix();

	//오른팔
	glPushMatrix();
	glColor3f(1.0f, 0.5f, 0.0f);
	//오른쪽 상박
	glTranslatef(-0.75, 1.5, 0.0);
	glutSolidSphere(0.3, 10, 10);//어깨(?)
	glRotatef(-70.0f, 0.2, 1, 0);
	glDisable(GL_CULL_FACE);
	glColor3f(1.0, 0.9, 0.7);
	gluCylinder(sullae, 0.12, 0.12, 0.8, 26, 13);

	//오른쪽 하박
	glTranslatef(0.03, 0.0, 0.7);
	glutSolidSphere(0.15, 10, 10);//팔꿈치(?)
	glRotatef(80.0f, 0, 1, 0);
	glRotatef(-60.0f, 1, -0.45, 0);
	gluCylinder(sullae, 0.12, 0.12, 1.2, 26, 13);

	//오른손
	glTranslatef(0.01, 0.0, 1.3);
	glutSolidSphere(0.2, 5, 10);
	glPopMatrix();

	//머리
	glPushMatrix();
	//머리통
	glColor3f(face[0], face[1], face[2]);
	glTranslatef(0, 2.45, -0.4);
	glRotatef(yRot, 0.0, 1.0, 0.0);//머리회전
	gluSphere(sullae, 0.7f, 26, 13);
	//머리카락
	glPushMatrix();
		//뒷머리
		glPushMatrix();
			glTranslatef(0, 0.09, -0.1);
			glColor3f(0.125, 0.125, 0.125);
			gluSphere(sullae, 0.72f, 26, 13);
		glPopMatrix();
		//앞머리
		glPushMatrix();
			glTranslatef(0, 0.15, -0.09);
			gluSphere(sullae, 0.72f, 26, 13);
		glPopMatrix();
		//갈래 (수정중...)
		glPushMatrix();//왼쪽
			glTranslated(-0.3, -0.5, -0.4);
			gluSphere(sullae, 0.15f, 26, 13);
			glTranslated(-0.08f, -0.1, -0.1f);
			gluSphere(sullae, 0.1f, 26, 13);
			glTranslated(-0.03f, -0.05, -0.05f);
			gluSphere(sullae, 0.07f, 26, 13);
		glPopMatrix();
		glPushMatrix();//오른쪽
			glTranslated(0.3, -0.5, -0.4);
			gluSphere(sullae, 0.15f, 26, 13);
			glTranslated(0.08f, -0.1, -0.1f);
			gluSphere(sullae, 0.1f, 26, 13);
			glTranslated(0.03f, -0.05, -0.05f);
			gluSphere(sullae, 0.07f, 26, 13);
		glPopMatrix();
	glPopMatrix();

	//눈
	glColor3f(0, 0, 0);
	glTranslatef(0.2f, 0.21f, 0.65f);
	gluSphere(sullae, 0.05f, 26, 13);//왼쪽 동공
	glColor3f(0, 0, 0);
	glTranslatef(-0.4f, -0.0055, 0);
	gluSphere(sullae, 0.05f, 26, 13);//오른쪽 동공

	//입
	glColor3f(1.0, 0, 0);
	glTranslatef(0.2f, -0.35f, 0.005f);

	if (isGameOver){
		glTranslatef(0,-0.1, 0.05f);
		glRotatef(15, 1, 0, 0);
		gluPartialDisk(sullae, 0.15, 0.2, 26, 13, 270, 180);
	}
	else if (isTouch) {
		glTranslatef(0, -0.1, 0.03f);
		glRotatef(15, 1, 0, 0);
		gluDisk(sullae, 0.15, 0.2, 26, 13);
	}
	else
		gluPartialDisk(sullae, 0.15, 0.2, 26, 13, 90, 180);
	glPopMatrix();
	glPopMatrix();
}

void Retry() {
	if (!canRetry)
		return;
	system("cls");
	Consolescreen();
	xRot = 0.0f;
	yRot = 0.0f;

	keyXRot = 0.0f;
	KeyYRot = 0.0f;

	recordTime = 0.0f;
	trunSpeed = 10.0f;
	waitTime = 0.0f;
	walkDis = 0.0f;
	legAngle = 0.0f;
	headSpin = 0.0f;
	face[0] = 1;
	face[1] = 0.9;
	face[2] = 0.7;
	fireRange = 0.0f;
	fireXsize = 5.0f;
	fireYsize = 5.0f;
	

	playerPos[1] = 0.8f;
	currentCamera = 1;
	endingAnimation = 0;
	isEnding = 0;
	fireMoveX = 0;
	fireCount = 0;
	fireColorIndex = 0;
	wordIndex = 0;
	for (int i = 0; i < 10; i++)
		word[i] = 0;

	isTouch = false;
	isTurn = false;
	isChance = false;
	isGameOver = false;
	isStart = false;
	canRetry = false;
	
}

//폭죽 
void Firework() {
	//나선모양 점찍기 참고
	if (endingAnimation == 5) {
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glPushMatrix();
		glColor3f(fireColor[fireColorIndex][0], fireColor[fireColorIndex][1], fireColor[fireColorIndex][2]);
		for (fireAngle = 0.0f; fireAngle <= (2.0f * GL_PI); fireAngle += 0.3f) {
			glPushMatrix();
			fireX = fireXsize * sin(fireAngle) * fireRange;//원 크기
			fireY = fireYsize * cos(fireAngle) * fireRange;//원 크기
			glTranslatef(fireX, fireY, 0);
			glutSolidSphere(2.0, 20, 20);
			glPopMatrix();
			glutPostRedisplay();
		}
		glPopMatrix();
		glEnable(GL_LIGHTING);
		glPopMatrix();
	}
}

void Timer(int value) {
	if (value == 7) {
		if (!(isGameOver || isTouch)) {
			recordTime += 0.1;
			glutTimerFunc(100, Timer, 7);
		}
	}
	if (value == 6) {
		if (endingAnimation == 0) {
			
			endingAnimation = 3;
			glutTimerFunc(3000, Timer, 6);
		}
		else if (endingAnimation == 3) {
			endingAnimation = 4;
			glutTimerFunc(3000, Timer, 6);
		}
		else if (endingAnimation == 4) {
			endingAnimation = 5;
			glutTimerFunc(100, Timer, 6);
		}
		else if (endingAnimation == 5) {
			fireRange+=0.5;

			if (fireRange > 3.0f) {
				if (fireMoveX == 0)
					fireMoveX = 20;
				else if (fireMoveX == 20)
					fireMoveX = -20;
				else
					fireMoveX = 0;

				fireCount++;
				fireRange = 0.0f;
			}

			if (fireCount > 10){
				endingAnimation = 6;
				glutTimerFunc(1000, Timer, 6);
			}
			else {
				fireColorIndex = (fireColorIndex + 1) % 7;
				glutTimerFunc(80, Timer, 6);
			}
				

			
		}
		else {
			canRetry = true;
			Retry();
		}
	}
	else if (value == 5) {
		if (endingAnimation == 0) {
			yRot = 180.0f;
			endingAnimation = 1;
			glutTimerFunc(3000, Timer, 5);
		}
		else if (endingAnimation == 1) {
			endingAnimation = 2;
			glutTimerFunc(3000, Timer, 5);
		}
		else if (endingAnimation == 2) {
			yRot += headSpin;
			if (headSpin>360) {
				glutTimerFunc(40, Timer, 5);
				endingAnimation = 99;
			}
			else {
				headSpin += 10.0f;
				yRot += headSpin;
				glutTimerFunc(10, Timer, 5);
				glutPostRedisplay();
			}
		}
		else {
			canRetry = true;
			Retry();
		}
	}
	else if (isGameOver || isTouch) {
		return;
	}
	else {
		GLfloat diffPenalty = diff  > 1 ? 10.0 : 0.0;
		if (value == 1) {
			yRot += 10;
			if (yRot >= 180) {
				isTurn = true;
				waitTime = ((rand() % 50 + 5)) * 100;		
				//printf("%.2lf초 뒤에 뒤를 돕니다!\n", waitTime / 1000);
				glutTimerFunc(waitTime, Timer, 2);
			}
			else
				glutTimerFunc(trunSpeed, Timer, 1);
		}
		if (value == 2) {
			yRot -= 10;
			if (yRot <= 0) {
				for (int i = 0; i < 10; i++)
					word[i] = 0;
				isTurn = false;
				wordIndex = 0;
				waitTime = ((rand() % 60) + 5) * 100;
				waitTime -= diffPenalty * 100;
				//waitTime = 20000; // 삭제라인
				if (waitTime < 0)
					waitTime = 500;
				//printf("%.2lf초 뒤에 플레이어를 봅니다!\n", waitTime / 1000);
				glutTimerFunc(waitTime / 10, Timer, 3);
				
				glutTimerFunc(waitTime, Timer, 1);
			}

			else
				glutTimerFunc(trunSpeed, Timer, 2);
		}

		if (value == 3) {
			if (wordIndex >= 10)
				return;
			word[wordIndex] = 1;
			wordIndex++;
			glutTimerFunc(waitTime / 10, Timer, 3);
		}
	}
	glutPostRedisplay();
}
//게임 설명
void GameManual_1() {
	cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
	cout << "┃                                게임 플레이 방법                              ┃\n";
	cout << "┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n";
	cout << "┃- 메뉴를 통해 난이도 조절이 가능합니다. (이지, 노말, 하드)                    ┃\n";
	cout << "┃- 이지모드에서 좌측하단의 바를 통해 술래가 돌아보는 시간을 확인할 수 있습니다.┃\n";
	cout << "┃- 좌측 상단의 별은 시간이 지날수록 하나식 사라집니다.                         ┃\n";
	cout << "┃- 우측 하단의 사각형은 술래의 상태를 의미합니다.                              ┃\n";
	cout << "┃- 술래가 나무를 바라보면 초록색, 플레이어를 바라보면 붉은색으로 표시됩니다.   ┃\n";
	cout << "┃- 술래가 뒤돌아보는 시간은 랜덤이니 술래의 상태를 잘 확인하세요.              ┃\n";
	cout << "┃- 술래가 나무를 보고있는 동안 빨간선이 그어져있는 곳으로 가야합니다.          ┃\n";
	cout << "┃- 빨간선 도착 후 마우스 왼쪽 버튼를 눌러 술래를 잡으면 게임에서 승리합니다.   ┃\n";
	cout << "┃- 만약, 술래가 뒤돌아보고 있는 상태에서 움직이면 게임에서 패배합니다.         ┃\n";
	cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
	cout << "\n";
}
//게임 조작 방법
void GameManual_2() {
	cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
	cout << "┃                                조작 방법                                     ┃\n";
	cout << "┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n";
	cout << "┃- W S 키를 누르면 앞 뒤로 이동할 수 있습니다.                                 ┃\n";
	cout << "┃- 마우스 왼쪽 버튼을 눌러 술래를 잡을 수 있습니다.                            ┃\n";
	cout << "┃- 게임 시작 후 키보드 1을 누르면 1인칭, 2를 누르면 2인칭, 3을 누르면 3인칭으로┃\n";
	cout << "┃  시점 변경이 가능합니다.                                                     ┃\n";
	cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
	cout << "\n";
}

void GameStart() {
	if (isGameOver || isStart)
		return;
	system("cls");
	
	isStart = true;
	waitTime = 3000;
	printf("\n게임이 시작됩니다!\n");
	glutTimerFunc(waitTime, Timer, 1);
	glutTimerFunc(waitTime/10, Timer, 3);
	glutTimerFunc(100, Timer, 7);
	
}

//첫 콜솔 창 (내용 추가 예정)
void Consolescreen() {
	cout << "***************************************************************************" << endl;
	cout << "*                                                                         *" << endl;
	cout << "*                                                                         *" << endl;
	cout << "*                          무궁화 꽃이 피었습니다!                        *" << endl;
	cout << "*                                                                         *" << endl;
	cout << "*                                                                         *" << endl;
	cout << "***************************************************************************" << endl;
	cout << endl;
	cout << "윈도우창에 우클릭을 하여 나오는 메뉴를 통해 게임매뉴얼을 확인하세요. " << endl;
	cout << endl;
}
//메뉴판 
void MenuProc(int entryID) {
	
	if (entryID == 1) {
		GameStart();
	}
	if (entryID == 21) {
		GameManual_1();
	}
	if (entryID == 22) {
		GameManual_2();
	}
	if (entryID == 31) {
		glutReshapeWindow(800, 600);
		displaymode = 1;
	}
	if (entryID == 32) {
		glutReshapeWindow(1024, 768);
		displaymode = 2;
	}
	if (entryID == 33) {
		glutReshapeWindow(1280, 720);
		displaymode = 3;
	}
	if (entryID > 40) {
		if (!isStart) {
			diff = (entryID % 40) - 1;

			if (diff == 0)
				sullaePos[2] = -50.0f;
			if (diff == 1)
				sullaePos[2] = -100.0f;
			if (diff == 2)
				sullaePos[2] = -120.0f;
		}
		else
			printf("게임 종료 후 이용해 주세요\n");

	}

	if (entryID == 9) {
		cout << "\n게임을 종료합니다. " << endl;
		exit(0);
	}
	glutPostRedisplay();
}
//메뉴판 구현 
void MenuFunc() {
	//설명서 메뉴판
	GLint MenualID = glutCreateMenu(MenuProc);
	glutAddMenuEntry("How to play", 21);
	glutAddMenuEntry("How to control", 22);
	//화면 크기 조절 메뉴판
	GLint settingID = glutCreateMenu(MenuProc);
	glutAddMenuEntry("800 X 600 (4:3)", 31);
	glutAddMenuEntry("1024 X 768 (4:3)", 32);
	glutAddMenuEntry("1280 X 720 (16:9)", 33);

	//난이도 메뉴판
	GLint diffID = glutCreateMenu(MenuProc);
	glutAddMenuEntry("Easy", 41);
	glutAddMenuEntry("Normal", 42);
	glutAddMenuEntry("Hard", 43);

	//메인 메뉴판
	GLint MainMenuID = glutCreateMenu(MenuProc);
	glutAddMenuEntry("-Game Start", 1);
	glutAddSubMenu("-Game Meual", MenualID);
	glutAddSubMenu("-Settings", settingID);
	glutAddSubMenu("-Diffcult", diffID);
	glutAddMenuEntry("-Exit", 9); //종료 메뉴

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void MoveCamera() {
	if (endingAnimation == 1)
		gluLookAt(sullaePos[0] + 5, sullaePos[1] + 3, sullaePos[2] + 4, sullaePos[0], sullaePos[1] + 2, sullaePos[2], 0, 1, 0);
	else if (endingAnimation == 2)
		gluLookAt(sullaePos[0], 1, sullaePos[2] + 3, sullaePos[0], sullaePos[1] + 2, sullaePos[2], 0, 1, 0);
	else if (endingAnimation == 3)
		gluLookAt(sullaePos[0] + 7, 1, sullaePos[2] + 3, sullaePos[0] - 3, sullaePos[1] + 2, sullaePos[2], 0, 1, 0);
	else if (endingAnimation == 4)
		gluLookAt(sullaePos[0] - 7, 1, sullaePos[2] + 3, sullaePos[0] - 2, sullaePos[1] + 2, sullaePos[2], 0, 1, 0);
	else if (endingAnimation == 5 || endingAnimation == 6)
		gluLookAt(sullaePos[0],sullaePos[1],sullaePos[2]+3, 0, 10, 0, 0, 1, 0);
	else if (currentCamera == 1)
		gluLookAt(playerPos[0], playerPos[1], playerPos[2] - walkDis, 0.0f, 0.0f, sullaePos[2] - walkDis, 0.0f, 1.0f, 0.0f);
	else if (currentCamera == 2)
		gluLookAt(sullaePos[0] + 10.0f, sullaePos[1] + 5.0f, sullaePos[2] - 10.0f, 0.0f, 0.8f, sullaePos[2]+10, 0.0f, 1.0f, 0.0f);
	else if (currentCamera == 3)
		gluLookAt(0.0f, 5.0f, -walkDis+10.0, 0,1,-walkDis, 0.0f, 0.0f, -1.0f);


}

void DrawUI() {
	if (endingAnimation != 0 || !isStart)
		return;
	//별
	glViewport(NW/10*6, NH/ 10*8, NW/10*4, NH/10*2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 4, -1, 1, -1.0, 1.0);
	glColor3f(1, 1, 0);
	int stars = 1;
	if (recordTime < rateTime[diff][0])
		stars = 3;
	else if (recordTime < rateTime[diff][1])
		stars = 2;
	else
		stars = 1;
	//이지모드에서 20초에 별 2개 30초에 별 1개
	//노말모드에서 40초에 별 2개 50초에 별 1개
	//하드모드에서 60초에 별 2개 70초에 별 1개
	for (int i = 0; i < stars; i++) {
		glBegin(GL_POLYGON);
		glVertex3f(0.0f + i*1.5, -0.5f, 0.0f);
		glVertex3f(-0.5f + i * 1.5, -0.75f, 0.0f);
		glVertex3f(-0.33f + i * 1.5, -0.25f, 0.0f);
		glVertex3f(-0.75f + i * 1.5, 0.0f, 0.0f);
		glVertex3f(-0.25f + i * 1.5, 0.0f, 0.0f);
		glVertex3f(0.0f + i * 1.5, 0.5f, 0.0f);
		glVertex3f(0.25f + i * 1.5, 0.0f, 0.0f);
		glVertex3f(0.75f + i * 1.5, 0.0f, 0.0f);
		glVertex3f(0.33f + i * 1.5, -0.25f, 0.0f);
		glVertex3f(0.5f + i * 1.5, -0.75f, 0.0f);
		glEnd();
	}



	if (diff == 0) {
		glViewport(0, 0, NW / 2, NH / 2);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, 25, 0.0, 20, -1.0, 1.0);
		for (int i = 0; i < 10; i++) {
			if (word[i] == 1)
				glColor3f(1, 0, 0);
			else
				glColor3f(0, 0, 0);
			glBegin(GL_POLYGON);
			glVertex3f(1 + i * 1.5, 1,1);
			glVertex3f(3 + i * 1.5, 1,1);
			glVertex3f(3 + i * 1.5, 3,1);
			glVertex3f(1 + i * 1.5, 3,1);
			glEnd();
		}
	}

	//알림
	glViewport(NW / 2, 0, NW/2, NH/3);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1,1);
	if (isChance)
		glColor3f(1, 1, 0);
	else if (word[9] == 1)
		glColor3f(1, 0, 0);
	else if (word[5] == 1)
		glColor3f(0.5, 0.5, 0);
	else
		glColor3f(0, 1, 0);

	
	glBegin(GL_POLYGON);
	glVertex3f(0.25 , 0.25,1);
	glVertex3f(0.75 , 0.25,1);
	glVertex3f(0.75 , -0.75,1);
	glVertex3f(0.25 , -0.75,1);
	glEnd();
	
	// Viewport를 원래대로 복원
	glViewport(0, 0, NW, NH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0f, NW/NH, 1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void RenderScene() {
	GLUquadricObj* sullae;
	GLUquadricObj* player;
	sullae = gluNewQuadric();
	player = gluNewQuadric();

	gluQuadricNormals(sullae, GLU_SMOOTH);
	gluQuadricNormals(player, GLU_SMOOTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawUI();	
	glPushMatrix();
	MoveCamera();
	glPushMatrix();
	glTranslatef(fireMoveX, 15, 10);
	Firework();
	glPopMatrix();
	
	DrawFloor();		//바닥
	
	DrawPlayer(player);	//플레이어
	
	DrawSullae(sullae);	//술래

	//요원
	DrawAgent(-7.0, -2.0f, sullaePos[2], 1); //바라보는 기준 왼쪽
	DrawAgent(7.0f, -2.0, sullaePos[2], 2); //바라보는 기준 오른쪽

	glPopMatrix();
	glutSwapBuffers();
}

void ChangeSize(int w, int h) {
	float nRa = 100.0f;
	GLfloat fAspect;

	if (h == 0)
		h = 1;
	NW = w;
	NH = h;

	if (displaymode == 1) {
		NW = 800;
		NH = 600;

	}
	else if (displaymode == 2) {
		NW = 1024;
		NH = 768;
	}
	else if (displaymode == 3) {
		NW = 1280;
		NH = 720;
	}

	glViewport(0, 0, NW, NH);
	fAspect = (GLfloat)NW / NH;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0f, fAspect, 1, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1.0, 0.0, 0.0);
}

void SpecialKeys(int Key, int x, int y) {
	if (Key == GLUT_KEY_UP)
		keyXRot += 5.0f;
	if (Key == GLUT_KEY_DOWN)
		keyXRot -= 5.0f;
	if (Key == GLUT_KEY_RIGHT)
		KeyYRot += 5.0f;
	if (Key == GLUT_KEY_LEFT)
		KeyYRot -= 5.0f;

	glutPostRedisplay();
}

void MouseClick(GLint Button, GLint State, GLint X, GLint Y) {

	if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN && isChance && !isTouch) {
		face[0] = 1;
		face[1] = 0.5;
		face[2] = 0.5;
		legAngle = 0;
		isEnding = 90;
		yRot = 180.0f;
		isTouch = true;

		printf("\n게임에서 %.1lf초만에 승리하셨습니다!\n", recordTime);

		//기록 평가 멘트
		if (recordTime < rateTime[diff][0])
			printf("Excellent\n");
		else if (recordTime < rateTime[diff][1])
			printf("Good\n");
		else
			printf("Try Again^^\n");

		printf("\n");
		

		glutTimerFunc(1000, Timer, 6);
	}
}

void Keyboard(unsigned char Key, int x, int y) {
	isChance = (sullaePos[2] + walkDis >= -3.0f) ? true : false;
	if (Key == 'p' || Key == 'P')
		walkDis = -sullaePos[2] - 3;
	if (isGameOver || isTouch)
		return;
	if (Key == '1' || Key == '2' || Key == '3') {
		switch (Key)
		{
		case '1': 
			currentCamera = 1;
			break;
		case '2':
			currentCamera = 2;
			break;
		case '3': 
			currentCamera = 3;
			break;
		}
	}
	else if(isStart){
		if (!isTurn) {
			if (!isChance) {
				if (Key == 'w' || Key == 'W')
					PlayerWalk(1);
			}
			if (Key == 's' || Key == 'S')
				PlayerWalk(-1);
		}
		else {
			isGameOver = true;
			printf("\n%.1lf초만에 술래에게 걸렸습니다!!\n잘 좀 하지^^\n\n",recordTime);
			
			face[0] = 1;
			face[1] = 0.5;
			face[2] = 0.5;
			glutTimerFunc(2000, Timer, 5);
		}
	}
	glutPostRedisplay();
}

void SetRC() {
	//눈사람 예제 참조
	GLfloat whiteLight[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	GLfloat sourceLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat listPos[] = { 0.0f, 50.0f, -20.0f, 1.0f };

	glEnable(GL_DEPTH_TEST);                                    //깊이를 킴, 깊이가 생겼다.
	glFrontFace(GL_CCW);                                        //반시계방향이 앞면이다. *CW = 시계방향이 앞면
	glEnable(GL_CULL_FACE);                                     //
	glEnable(GL_LIGHTING);                                      //전역 조명을 켜라.
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);         //모델마다 ambient 주변광을 줘라
	glLightfv(GL_LIGHT0, GL_AMBIENT, sourceLight);              //지역조명0, 주변광 줘라
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);              //지역조명0, diffuse 확산광 반사광 줘라
	glLightfv(GL_LIGHT0, GL_POSITION, listPos);                 //지역조명0의 위치를 지정
	glEnable(GL_LIGHT0);                                        //지역조명 켜기

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void DrawTree(GLfloat x, GLfloat y, GLfloat z) {
	glPushMatrix();
	GLUquadricObj* tree;
	tree = gluNewQuadric();
	gluQuadricNormals(tree, GLU_SMOOTH);

	glTranslatef(x, y, z - 3.5f);

	glPushMatrix();
	glDisable(GL_CULL_FACE);
	glRotatef(90, 1, 0, 0);

	//나뭇잎
	glColor3f(0.2, 0.5, 0.2);
	
	glTranslatef(0, 0, -28);
	gluCylinder(tree, 0, 6, 20, 26, 13);

	glTranslatef(0, 0, 19.888);
	gluDisk(tree, 0, 6, 26, 13);
	glTranslatef(0, 0, 8.112);
	
	//몸통
	glColor3f(0.5f, 0.3f, 0.3f);
	glRotatef(-90, 1, 0, 0);
	
	glTranslatef(0, 10, 0);
	glRotatef(90, 1, 0, 0);
	gluCylinder(tree, 2.5, 2.5, 10, 26, 13);

	//뿌리
	glTranslatef(0, 0, 10);
	gluCylinder(tree, 2.5, 4, 2.3, 13, 13);
	glPopMatrix();
	glPopMatrix();
}

void DrawAgent(GLfloat x, GLfloat y, GLfloat z, GLint num) {
	GLUquadricObj* Agent;
	Agent = gluNewQuadric();
	gluQuadricNormals(Agent, GLU_SMOOTH);

	glPushMatrix();
		glTranslatef(x, y, z);//요원 위치
		glRotatef(keyXRot, 1.0, 0.0, 0.0);
		glRotatef(KeyYRot, 0.0, 1.0, 0.0);

		if ((endingAnimation == 1 ||isTouch)&& num == 1)
			glRotatef(70, 0, 1, 0);
		if ( isTouch && num == 2)
			glRotatef(-70, 0, 1, 0);

		//왼 다리
		glPushMatrix();
			glTranslatef(-0.3, 0, -0.25);
			glRotatef(90, 1, 0, 0);
			glColor3f(1.0, 0.2, 0.2);
			gluCylinder(Agent, 0.3, 0.15, 2.0, 26, 13);
			//왼발
			glPushMatrix();
				glTranslatef(0.0, -0.2, 1.8);
				glRotated(-90.0f, 1, 0, 0);
				//신발
				glTranslatef(0, 0, 0.2);
				glColor3f(0.0, 0.0, 0.0);
				glutSolidCube(0.5);
				glTranslatef(0, 0, 0.3);
				glutSolidCube(0.5);
			glPopMatrix();
		glPopMatrix();

		//오른다리
		glPushMatrix();
			glTranslatef(0.3, 0, -0.25);
			glRotatef(90, 1, 0, 0);
			glColor3f(1.0f, 0.2, 0.2);
			gluCylinder(Agent, 0.33, 0.15, 2.0, 26, 13);
			//오른발
			glPushMatrix();
				glTranslatef(0.0, -0.2, 1.8);
				glRotated(-90.0f, 1, 0, 0);
				glTranslatef(0, 0, 0.2);
				glColor3f(0.0, 0.0, 0.0);
				glutSolidCube(0.5);
				glTranslatef(0, 0, 0.3);
				glutSolidCube(0.5);
			glPopMatrix();
		glPopMatrix();

		//몸통
		glPushMatrix();
			glDisable(GL_CULL_FACE);
			glRotatef(90, 1, 0, 0);
			glTranslatef(0, -0.25, 0);
			glColor3f(1.0, 0.2, 0.2);
			glFrontFace(GL_CW);                                        //반시계방향이 앞면이다. *CW = 시계방향이 앞면
			gluDisk(Agent, 0, 0.75, 26, 13);
			glRotatef(-90, 1, 0, 0);
			glTranslatef(0, 1.8, 0);
			glRotatef(90, 1, 0, 0);
			glFrontFace(GL_CCW);                                        //반시계방향이 앞면이다. *CW = 시계방향이 앞면
			gluDisk(Agent, 0, 0.75, 26, 13);
			glEnable(GL_CULL_FACE);
			gluCylinder(Agent, 0.75, 0.75, 2.0, 26, 13);
			//지퍼
			glDisable(GL_CULL_FACE);
			glColor3f(0.0, 0.0, 0.0);
			glBegin(GL_POLYGON);
			glVertex3f(-0.03, 0.75, 0.0);
			glVertex3f(0.03, 0.75, 0.0);
			glVertex3f(0.03, 0.75, 2);
			glVertex3f(-0.03, 0.75, 2);
			glEnd();
			glEnable(GL_CULL_FACE);
			//벨트
			glTranslatef(0.0,0.0,2.0);
			gluCylinder(Agent, 0.76, 0.76, 0.05, 26, 13);
		glPopMatrix();

		//왼팔
		glPushMatrix();
			//왼쪽 상박
			glColor3f(1.0, 0.2, 0.2);
			glTranslatef(0.75, 1.55, 0.0);
			glutSolidSphere(0.3, 10, 10);
			glRotatef(100, 1, 0, 0);
			glRotatef(15, 0, 1, 0);
			glDisable(GL_CULL_FACE);
			glColor3f(1.0, 0.2, 0.2);
			gluCylinder(Agent, 0.14, 0.14, 1.0, 26, 13);

			//왼쪽 하박
			glTranslatef(0.03, 0.0, 1.0);
			glutSolidSphere(0.15, 10, 10);//팔꿈치
			glRotatef(-90, 1, 0, 0);
			glRotatef(-30, 0, 1, 0);
			gluCylinder(Agent, 0.14, 0.14, 0.9, 26, 13);

			//왼손
			glTranslatef(0.01, 0.0, 1.0);
			glColor3f(0.0, 0.0, 0.0);
			glutSolidSphere(0.2, 5, 10);
		glPopMatrix();

		//오른팔
		glPushMatrix();
			glColor3f(1.0, 0.2, 0.2);
			//오른쪽 상박
			glTranslatef(-0.75, 1.55, 0.0);
			glutSolidSphere(0.3, 10, 10);//어깨(?)
			glRotatef(55, 1, 0, 0);
			glRotatef(-20, 0, 1, 0);
			glDisable(GL_CULL_FACE);
			gluCylinder(Agent, 0.14, 0.14, 1.0, 26, 13);

			//오른쪽 하박
			glTranslatef(0.0, 0.0, 1.0);
			glutSolidSphere(0.16, 10, 10);//팔꿈치
			glRotated(-30, 1, 0, 0);
			glRotatef(95, 0, 1, 0);
			gluCylinder(Agent, 0.14, 0.14, 0.9, 26, 13);

			//오른손
			glTranslatef(0.01, 0.0, 1.0);
			glColor3f(0.0, 0.0, 0.0);
			glutSolidSphere(0.2, 5, 10);
			//총 (따로 조명을 넣을까 싶음)
			glPushMatrix();
				glRotatef(-30, 0, 0, 1);
				glTranslatef(0.0, 0.25, 0.0);
				glColor3f(0.15, 0.15, 0.15);//총 몸통 1
				glutSolidCube(0.35);
				glPushMatrix();//개머리판
				glTranslatef(0.0, 0.02, -0.175);
					glRotatef(180, 0, 1, 0);
					gluCylinder(Agent, 0.08, 0.08, 0.5, 20, 20);
					glTranslatef(0.0, 0.1, 0.5);
					glRotated(90, 1, 0, 0);
					gluCylinder(Agent, 0.09, 0.09, 0.25, 20, 20);
				glPopMatrix();
				glTranslatef(0.0, 0.0, 0.35);//총 몸통 2
				glutSolidCube(0.35);
				glPushMatrix();//탄창
					glTranslatef(0.0, -0.175, 0.0);
					glutSolidCube(0.1);
					for (int i = 0; i < 6; i++) {//탄창길이
						glTranslatef(0.0, -0.05, 0.006);
						glutSolidCube(0.1);
					}
				glPopMatrix();
					glTranslatef(0.0, 0.0, 0.35);//총 몸통 3
					glutSolidCube(0.35);
					glTranslatef(0.0, 0.05, 0.175);//총구
					gluCylinder(Agent, 0.05, 0.05, 0.23, 20, 20);
				glPopMatrix();
			glPopMatrix();

		//머리
		glPushMatrix();
			//머리통
			glColor3f(0.0, 0.0, 0.0);
			glTranslatef(0, 2.3, -0.25);
			gluSphere(Agent, 0.6, 26, 13);
			//가면(??)
			glPushMatrix();
				glTranslatef(0.0,0.13,0.6);
				glDisable(GL_CULL_FACE);
				glColor3f(1.0, 1.0, 1.0);
				glBegin(GL_POLYGON);
				glVertex3f(-0.22,0.22, 0.0);
				glVertex3f(0.22, 0.22, 0.0);
				glVertex3f(0.22, -0.22, 0.0);
				glVertex3f(-0.22, -0.22, 0.0);
				glEnd();
				glPushMatrix();
					glTranslatef(0.0, 0.0, 0.001);
					glColor3f(0.0, 0.0, 0.0);
					glBegin(GL_POLYGON);
					glVertex3f(-0.165, 0.165, 0.0);
					glVertex3f(0.165, 0.165, 0.0);
					glVertex3f(0.165, -0.165, 0.0);
					glVertex3f(-0.165, -0.165, 0.0);
					glEnd();
				glPopMatrix();
			glPopMatrix();

			//후드
			glColor3f(1.0, 0.2, 0.2);
			glTranslatef(0, -0.001, -0.15);
			gluSphere(Agent, 0.7, 26, 13);
		glPopMatrix();

	glPopMatrix();
}