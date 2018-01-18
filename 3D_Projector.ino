
#include <Arduboy2.h>
#include <math.h>
template <int size>
struct aCubeCentered {
	bool active = true;
	int cube3D[8][3] =
	{
		{ -size, -size, -size },
		{ -size, size, -size },
		{ size, size, -size },
		{ size, -size, -size },
		{ -size, -size, size },
		{ -size, size, size },
		{ size, size, size },
		{ size, -size, size }
	};
};

template <int size>
struct aCubeOrigin{
	bool active = true;
	int cube3D[8][3] =
	{
		{ 0, 0, 0 },
		{ 0, size, 0 },
		{ size, size, 0 },
		{ size, 0, 0 },
		{ 0, 0, size },
		{ 0, size, size },
		{ size, size, size },
		{ size, 0, size }
	};
};

template <int size, int aX, int aY, int aZ>
struct aCubeAnchor {
	bool active = true;
	int anchor[3] = { aX,aY,aZ };
	int cube3D[8][3] = {
		{ anchor[0],		anchor[1],			anchor[2] },
		{ anchor[0],		anchor[1] + size,	anchor[2] },
		{ anchor[0] + size, anchor[1] + size,	anchor[2] },
		{ anchor[0] + size, anchor[1],			anchor[2] },
		{ anchor[0],		anchor[1],			anchor[2] + size },
		{ anchor[0],		anchor[1] + size,	anchor[2] + size },
		{ anchor[0] + size, anchor[1] + size,	anchor[2] + size },
		{ anchor[0] + size, anchor[1],			anchor[2] + size }
	};
	void setAnchor(int x, int y, int z) {
		anchor[0] = x;
		anchor[1] = y;
		anchor[2] = z;
		// Refresh
		cube3D[0][0] = anchor[0];			cube3D[0][1] = anchor[1];			cube3D[0][2] = anchor[2];
		cube3D[1][0] = anchor[0];			cube3D[1][1] = anchor[1] + size;	cube3D[1][2] = anchor[2];
		cube3D[2][0] = anchor[0] + size;	cube3D[2][1] = anchor[1] + size;	cube3D[2][2] = anchor[2];
		cube3D[3][0] = anchor[0] + size;	cube3D[3][1] = anchor[1];			cube3D[3][2] = anchor[2];
		cube3D[4][0] = anchor[0];			cube3D[4][1] = anchor[1];			cube3D[4][2] = anchor[2] + size;
		cube3D[5][0] = anchor[0];			cube3D[5][1] = anchor[1] + size;	cube3D[5][2] = anchor[2] + size;
		cube3D[6][0] = anchor[0] + size;	cube3D[6][1] = anchor[1] + size;	cube3D[6][2] = anchor[2] + size;
		cube3D[7][0] = anchor[0] + size;	cube3D[7][1] = anchor[1];			cube3D[7][2] = anchor[2] + size;
	}
};
Arduboy2 arduboy;
const int CUBE_SIZE = 10;
const int NUMBER_OF_CUBES = 8;
//aCubeCentered<5> myCube[NUMBER_OF_CUBES];
//aCubeOrigin<CUBE_SIZE> oriCube[NUMBER_OF_CUBES];
aCubeAnchor<CUBE_SIZE, 0, 0, 0> oriCube[NUMBER_OF_CUBES];

//aCube<10> myCube2[5];

//int cube3D[8][3] = 
//	{
//		{ -10, -10, -10 },
//		{ -10, 10, -10 },
//		{ 10, 10, -10 },
//		{ 10, -10, -10 },
//		{ -10, -10, 10 },
//		{ -10, 10, 10 },
//		{ 10, 10, 10 },
//		{ 10, -10, 10 }
//	};


int origin[] = { 0, 0, 0 };

int cubeProjection[8][2];

float theta = 0.5f, phi = 0.5f;
bool updateIt = true;

int selectedCube = 0;

void setup()
{
	
	arduboy.boot();
	arduboy.display();
	arduboy.clear();
	arduboy.setTextSize(1);
	arduboy.setFrameRate(30);

	for (int i = 0; i < NUMBER_OF_CUBES; i++) {
		switch (i)
		{
		case 1:
			moveCube(CUBE_SIZE, 0, 0, i);
			break;
		case 2:
			moveCube(0, CUBE_SIZE, 0, i);
			break;
		case 3:
			moveCube(0, 0, CUBE_SIZE, i);
			break;
		case 4:
			moveCube(CUBE_SIZE, CUBE_SIZE, 0, i);
			break;
		case 5:
			moveCube(CUBE_SIZE, 0, CUBE_SIZE, i);
			break;
		case 6:
			moveCube(0, CUBE_SIZE, CUBE_SIZE, i);
			break;
		case 7:
			moveCube(CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, i);
			break;
		default:
			break;
		}
		
	}

}

void loop()
{
	if (!arduboy.nextFrame())
	{
		return;
	}
	arduboy.clear();
	if (arduboy.pressed(UP_BUTTON) && arduboy.pressed(B_BUTTON)) {
		selectedCube++;
		if (selectedCube >= NUMBER_OF_CUBES) {
			selectedCube = 0;
		}
	}
	else if (arduboy.pressed(UP_BUTTON) && arduboy.pressed(A_BUTTON)) {
		moveCube(0, 0, -1, selectedCube);
	}
	else if (arduboy.pressed(UP_BUTTON)) {
		phi += 0.1f;
	}

	if (arduboy.pressed(DOWN_BUTTON) && arduboy.pressed(B_BUTTON)) {
		selectedCube--;
		if (selectedCube < 0) {
			selectedCube = NUMBER_OF_CUBES - 1;
		}
	}
	else if (arduboy.pressed(DOWN_BUTTON) && arduboy.pressed(A_BUTTON)) {
		moveCube(0, 0, 1, selectedCube);
	}
	else if (arduboy.pressed(DOWN_BUTTON)) {
		phi -= 0.1f;
	}

	
	if (arduboy.pressed(RIGHT_BUTTON) && arduboy.pressed(A_BUTTON)) {
		moveCube(0, 1, 0, selectedCube);
	}
	else if (arduboy.pressed(RIGHT_BUTTON) && arduboy.pressed(B_BUTTON)) {
		moveCube(1, 0, 0, selectedCube);
	}
	else if (arduboy.pressed(RIGHT_BUTTON)) {
		theta += 0.1f;
	}

	if (arduboy.pressed(LEFT_BUTTON) && arduboy.pressed(A_BUTTON)) {
		moveCube(0, -1, 0, selectedCube);
	}
	else if (arduboy.pressed(LEFT_BUTTON) && arduboy.pressed(B_BUTTON)) {
		moveCube(-1, 0, 0, selectedCube);
	}
	else if (arduboy.pressed(LEFT_BUTTON)) {
		theta -= 0.1f;
	}

	if (updateIt) {
		updateIt = false;
		//updateView();
	}
	drawOrigin();

	for (int i = 0; i < NUMBER_OF_CUBES; i++) {
		drawCube(*oriCube[i].cube3D);
	}
	//drawCube(*myCube[0].cube3D);
	//drawCube(*myCube2.cube3D);

	String tT = "T:";
	int tI = theta * 100;
	String pT = " P:";
	int pI = phi * 100;
	arduboy.setCursor(0, 0);
	arduboy.print(tT + tI + pT + pI);
	String oR = ":";
	arduboy.setCursor(0, 54);
	arduboy.print(origin[0] + oR + origin[1] + oR + origin[2] + oR + selectedCube);
	arduboy.display();
	
}

int pointTransferX(int pointT[]) {
	return (int)(-sinf(theta) * pointT[0] + cosf(theta) * pointT[1]) + (WIDTH / 2); 
}

int pointTransferY(int pointT[]) {
	return (int)(-cosf(theta) * sinf(phi) * pointT[0] + (-sinf(theta) * sinf(phi)) * pointT[1] + cosf(phi) * pointT[2]) + (HEIGHT / 2);
}


void drawCube(int theCube[]) {

	for (int i = 0; i < 21; i+=3) {
		if (i != 9) {
			arduboy.drawLine(
				(int)(-sinf(theta) * theCube[i] + cosf(theta) * theCube[i + 1]) + (WIDTH / 2),
				(int)(-cosf(theta) * sinf(phi) * theCube[i] + (-sinf(theta) * sinf(phi)) * theCube[i + 1] + cosf(phi) * theCube[i + 2]) + (HEIGHT / 2),
				(int)(-sinf(theta) * theCube[i + 3] + cosf(theta) * theCube[i + 4]) + (WIDTH / 2),
				(int)(-cosf(theta) * sinf(phi) * theCube[i + 3] + (-sinf(theta) * sinf(phi)) * theCube[i + 4] + cosf(phi) * theCube[i + 5]) + (HEIGHT / 2)
			);
		}
	}
	for (int i = 0; i < 12; i+=3) {
		arduboy.drawLine(
			(int)(-sinf(theta) * theCube[i] + cosf(theta) * theCube[i + 1]) + (WIDTH / 2),
			(int)(-cosf(theta) * sinf(phi) * theCube[i] + (-sinf(theta) * sinf(phi)) * theCube[i + 1] + cosf(phi) * theCube[i + 2]) + (HEIGHT / 2),
			(int)(-sinf(theta) * theCube[i + 12] + cosf(theta) * theCube[i + 13]) + (WIDTH / 2),
			(int)(-cosf(theta) * sinf(phi) * theCube[i + 12] + (-sinf(theta) * sinf(phi)) * theCube[i + 13] + cosf(phi) * theCube[i + 14]) + (HEIGHT / 2)

		);
	}
	arduboy.drawLine(
		(int)(-sinf(theta) * theCube[0] + cosf(theta) * theCube[1]) + (WIDTH / 2),
		(int)(-cosf(theta) * sinf(phi) * theCube[0] + (-sinf(theta) * sinf(phi)) * theCube[1] + cosf(phi) * theCube[2]) + (HEIGHT / 2),
		(int)(-sinf(theta) * theCube[9] + cosf(theta) * theCube[10]) + (WIDTH / 2),
		(int)(-cosf(theta) * sinf(phi) * theCube[9] + (-sinf(theta) * sinf(phi)) * theCube[10] + cosf(phi) * theCube[11]) + (HEIGHT / 2)

	);
	arduboy.drawLine(
		(int)(-sinf(theta) * theCube[12] + cosf(theta) * theCube[13]) + (WIDTH / 2),
		(int)(-cosf(theta) * sinf(phi) * theCube[12] + (-sinf(theta) * sinf(phi)) * theCube[13] + cosf(phi) * theCube[14]) + (HEIGHT / 2),
		(int)(-sinf(theta) * theCube[21] + cosf(theta) * theCube[22]) + (WIDTH / 2),
		(int)(-cosf(theta) * sinf(phi) * theCube[21] + (-sinf(theta) * sinf(phi)) * theCube[22] + cosf(phi) * theCube[23]) + (HEIGHT / 2)

	);

}

void testingP(int pointers[]) {
	arduboy.print(pointers[2]);
}

void drawOrigin() {
	arduboy.drawPixel(
		(int)(-sinf(theta) * 0 + cosf(theta) * 0) + (WIDTH / 2),
		(int)(-cosf(theta) * sinf(phi) * 0 + (-sinf(theta) * sinf(phi)) * 0 + cosf(phi) * 0) + (HEIGHT / 2)
	);
}

void moveCube(int x, int y, int z, int cubeNumber) {
	oriCube[cubeNumber].setAnchor(
		oriCube[cubeNumber].anchor[0] + x, 
		oriCube[cubeNumber].anchor[1] + y, 
		oriCube[cubeNumber].anchor[2] + z
	);
}