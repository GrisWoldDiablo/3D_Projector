
#include <Arduboy2.h>
#include <math.h>
#include <Tinyfont.h>

Arduboy2 arduboy;
Tinyfont ardTiny = Tinyfont(arduboy.sBuffer, Arduboy2::width(), Arduboy2::height());

struct coord2D {
	int x;
	int y;
};
struct aCube {
	int size = 0;
	int x = 0;
	int y = 0;
	int z = 0;
	float theta = 0;
	float phi = 0;
	coord2D p1;
	coord2D p2;
	coord2D p3;
	coord2D p4;
	coord2D p5;
	coord2D p6;
	coord2D p7;
	coord2D p8;
	void updateCoord(float setTheta, float setPhi) {
		theta = setTheta;
		phi = setPhi;
		//0:0:0
		p1.x = (int)(-sinf(theta) * x + cosf(theta) * y) + (WIDTH / 2);
		p1.y = (int)(-cosf(theta) * sinf(phi) * x + (-sinf(theta) * sinf(phi)) * y + cosf(phi) * z) + (HEIGHT / 2);
		//0:1:0
		p2.x = (int)(-sinf(theta) * x + cosf(theta) * (y + size)) + (WIDTH / 2);
		p2.y = (int)(-cosf(theta) * sinf(phi) * x + (-sinf(theta) * sinf(phi)) * (y + size) + cosf(phi) * z) + (HEIGHT / 2);
		//1:0:0
		p3.x = (int)(-sinf(theta) * (x + size) + cosf(theta) * y) + (WIDTH / 2);
		p3.y = (int)(-cosf(theta) * sinf(phi) * (x + size) + (-sinf(theta) * sinf(phi)) * y + cosf(phi) * z) + (HEIGHT / 2);
		//1:1:0
		p4.x = (int)(-sinf(theta) * (x + size) + cosf(theta) * (y + size)) + (WIDTH / 2);
		p4.y = (int)(-cosf(theta) * sinf(phi) * (x + size) + (-sinf(theta) * sinf(phi)) * (y + size) + cosf(phi) * z) + (HEIGHT / 2);
		//0:0:1
		p5.x = (int)(-sinf(theta) * x + cosf(theta) * y) + (WIDTH / 2);
		p5.y = (int)(-cosf(theta) * sinf(phi) * x + (-sinf(theta) * sinf(phi)) * y + cosf(phi) * (z + size)) + (HEIGHT / 2);
		//0:1:1
		p6.x = (int)(-sinf(theta) * x + cosf(theta) * (y + size)) + (WIDTH / 2);
		p6.y = (int)(-cosf(theta) * sinf(phi) * x + (-sinf(theta) * sinf(phi)) * (y + size) + cosf(phi) * (z + size)) + (HEIGHT / 2);
		//1:0:1
		p7.x = (int)(-sinf(theta) * (x + size) + cosf(theta) * y) + (WIDTH / 2);
		p7.y = (int)(-cosf(theta) * sinf(phi) * (x + size) + (-sinf(theta) * sinf(phi)) * y + cosf(phi) * (z + size)) + (HEIGHT / 2);
		//1:1:1
		p8.x = (int)(-sinf(theta) * (x + size) + cosf(theta) * (y + size)) + (WIDTH / 2);
		p8.y = (int)(-cosf(theta) * sinf(phi) * (x + size) + (-sinf(theta) * sinf(phi)) * (y + size) + cosf(phi) * (z + size)) + (HEIGHT / 2);
	}
};

template <typename T, size_t initSize = 0>
struct MyArray {
private:
	T * arrayValue = NULL;
	size_t theSize = initSize;
public:

	MyArray() {
		arrayValue = new T[theSize];
	}
	
	/*MyArray(initializer_list<T> initList) {
		theSize = initList.size();
		arrayValue = new T[theSize];
		for (size_t i = 0; i < theSize; i++) {
			arrayValue[i] = *(initList.begin() + i);
		}
	}*/

	~MyArray() {
		if (arrayValue) {
			delete[] arrayValue;
			arrayValue = NULL;
		}
	}

	T& operator[](size_t i) {
		return arrayValue[i];
	}
	void addValue(T valueToAdd) {
		T *tempArray = new T[++theSize];

		tempArray[theSize - 1] = valueToAdd;

		for (size_t i = 0; i < theSize - 1; i++) {
			tempArray[i] = arrayValue[i];
		}

		delete[] arrayValue;
		arrayValue = tempArray;
	}

	void reSize(size_t newSize) {
		size_t oldSize = theSize;
		theSize = newSize;
		if (newSize <= 0) {
			delete[] arrayValue;
			arrayValue = NULL;
			return;
		}
		else if (oldSize == 0 && newSize > 0) {
			arrayValue = new T[newSize];
			return;
		}


		T *tempArray = new T[newSize];

		if (newSize > oldSize) {
			for (size_t i = 0; i < oldSize; i++) {
				tempArray[i] = arrayValue[i];
			}

			for (size_t i = oldSize; i < newSize; i++) {
				tempArray[i] = T{};
			}
		}
		else {
			for (size_t i = 0; i < newSize; i++) {
				tempArray[i] = arrayValue[i];
			}
		}

		delete[] arrayValue;
		arrayValue = tempArray;

	}

	size_t size() {
		return theSize;
	}
};

//template <int size>
//struct aCubeCentered {
//	bool active = true;
//	int cube3D[8][3] =
//	{
//		{ -size, -size, -size },
//		{ -size, size, -size },
//		{ size, size, -size },
//		{ size, -size, -size },
//		{ -size, -size, size },
//		{ -size, size, size },
//		{ size, size, size },
//		{ size, -size, size }
//	};
//};
//
//template <int size>
//struct aCubeOrigin{
//	bool active = true;
//	int cube3D[8][3] =
//	{
//		{ 0, 0, 0 },
//		{ 0, size, 0 },
//		{ size, size, 0 },
//		{ size, 0, 0 },
//		{ 0, 0, size },
//		{ 0, size, size },
//		{ size, size, size },
//		{ size, 0, size }
//	};
//};
//
//template <int size, int aX, int aY, int aZ>
//struct aCubeAnchor {
//	bool active = true;
//	int anchor[3] = { aX,aY,aZ };
//	int cube3D[8][3] = {
//		{ anchor[0],		anchor[1],			anchor[2] },
//		{ anchor[0],		anchor[1] + size,	anchor[2] },
//		{ anchor[0] + size, anchor[1] + size,	anchor[2] },
//		{ anchor[0] + size, anchor[1],			anchor[2] },
//		{ anchor[0],		anchor[1],			anchor[2] + size },
//		{ anchor[0],		anchor[1] + size,	anchor[2] + size },
//		{ anchor[0] + size, anchor[1] + size,	anchor[2] + size },
//		{ anchor[0] + size, anchor[1],			anchor[2] + size }
//	};
//	void setAnchor(int x, int y, int z) {
//		anchor[0] = x;
//		anchor[1] = y;
//		anchor[2] = z;
//		// Refresh
//		cube3D[0][0] = anchor[0];			cube3D[0][1] = anchor[1];			cube3D[0][2] = anchor[2];
//		cube3D[1][0] = anchor[0];			cube3D[1][1] = anchor[1] + size;	cube3D[1][2] = anchor[2];
//		cube3D[2][0] = anchor[0] + size;	cube3D[2][1] = anchor[1] + size;	cube3D[2][2] = anchor[2];
//		cube3D[3][0] = anchor[0] + size;	cube3D[3][1] = anchor[1];			cube3D[3][2] = anchor[2];
//		cube3D[4][0] = anchor[0];			cube3D[4][1] = anchor[1];			cube3D[4][2] = anchor[2] + size;
//		cube3D[5][0] = anchor[0];			cube3D[5][1] = anchor[1] + size;	cube3D[5][2] = anchor[2] + size;
//		cube3D[6][0] = anchor[0] + size;	cube3D[6][1] = anchor[1] + size;	cube3D[6][2] = anchor[2] + size;
//		cube3D[7][0] = anchor[0] + size;	cube3D[7][1] = anchor[1];			cube3D[7][2] = anchor[2] + size;
//	}
//};



const int CUBE_SIZE = 20;
const int NUMBER_OF_CUBES = 1;

MyArray<aCube, NUMBER_OF_CUBES> myCubes;

//aCubeCentered<5> myCube[NUMBER_OF_CUBES];
//aCubeOrigin<CUBE_SIZE> oriCube[NUMBER_OF_CUBES];
//aCubeAnchor<CUBE_SIZE, 0, 0, 0> oriCube[NUMBER_OF_CUBES]
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

float theta = 0.5f, phi = -0.5f;
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
	for (int i = 0; i < NUMBER_OF_CUBES; i++) {
		myCubes[i].size = CUBE_SIZE;
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
		drawCubeAnchor(i);
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

//int pointTransferX(int pointT[]) {
//	return (int)(-sinf(theta) * pointT[0] + cosf(theta) * pointT[1]) + (WIDTH / 2); 
//}
//
//int pointTransferY(int pointT[]) {
//	return (int)(-cosf(theta) * sinf(phi) * pointT[0] + (-sinf(theta) * sinf(phi)) * pointT[1] + cosf(phi) * pointT[2]) + (HEIGHT / 2);
//}


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


void drawOrigin() {
	arduboy.drawPixel(
		(int)(-sinf(theta) * 0 + cosf(theta) * 0) + (WIDTH / 2),
		(int)(-cosf(theta) * sinf(phi) * 0 + (-sinf(theta) * sinf(phi)) * 0 + cosf(phi) * 0) + (HEIGHT / 2)
	);
}

void moveCube(int x, int y, int z, int selectedCube) {
	myCubes[selectedCube].x += x;
	myCubes[selectedCube].y += y;
	myCubes[selectedCube].z += z;
}

void drawCubeAnchor(int iS) {

	//		{ 0, 0, 0 },
	//		{ 0, size, 0 },
	//		{ size, size, 0 },
	//		{ size, 0, 0 },
	//		{ 0, 0, size },
	//		{ 0, size, size },
	//		{ size, size, size },
	//		{ size, 0, size }
	myCubes[iS].updateCoord(theta,phi);
	arduboy.drawLine(myCubes[iS].p1.x, myCubes[iS].p1.y, myCubes[iS].p2.x, myCubes[iS].p2.y);
	arduboy.drawLine(myCubes[iS].p1.x, myCubes[iS].p1.y, myCubes[iS].p3.x, myCubes[iS].p3.y);
	arduboy.drawLine(myCubes[iS].p1.x, myCubes[iS].p1.y, myCubes[iS].p5.x, myCubes[iS].p5.y);
	
	if (	(myCubes[iS].p1.x >= myCubes[iS].p2.x && myCubes[iS].p1.y <= myCubes[iS].p5.y) 
		||	(myCubes[iS].p1.x <= myCubes[iS].p2.x && myCubes[iS].p1.y >= myCubes[iS].p5.y)) {
		arduboy.fillTriangle(
			//0:0:0
			(int)(-sinf(theta) * myCubes[iS].x + cosf(theta) * myCubes[iS].y) + (WIDTH / 2),
			(int)(-cosf(theta) * sinf(phi) * myCubes[iS].x + (-sinf(theta) * sinf(phi)) * myCubes[iS].y + cosf(phi) * myCubes[iS].z) + (HEIGHT / 2),
			//0:1:0
			(int)(-sinf(theta) * myCubes[iS].x + cosf(theta) * (myCubes[iS].y + myCubes[iS].size)) + (WIDTH / 2),
			(int)(-cosf(theta) * sinf(phi) * myCubes[iS].x + (-sinf(theta) * sinf(phi)) * (myCubes[iS].y + myCubes[iS].size) + cosf(phi) * myCubes[iS].z) + (HEIGHT / 2),
			//0:0:1
			(int)(-sinf(theta) * myCubes[iS].x + cosf(theta) * myCubes[iS].y) + (WIDTH / 2),
			(int)(-cosf(theta) * sinf(phi) * myCubes[iS].x + (-sinf(theta) * sinf(phi)) * myCubes[iS].y + cosf(phi) * (myCubes[iS].z + myCubes[iS].size)) + (HEIGHT / 2)
		);
		arduboy.drawLine(
			(int)(-sinf(theta) * myCubes[iS].x + cosf(theta) * myCubes[iS].y) + (WIDTH / 2),
			(int)(-cosf(theta) * sinf(phi) * myCubes[iS].x + (-sinf(theta) * sinf(phi)) * myCubes[iS].y + cosf(phi) * myCubes[iS].z) + (HEIGHT / 2),
			(int)(-sinf(theta) * myCubes[iS].x + cosf(theta) * (myCubes[iS].y + myCubes[iS].size)) + (WIDTH / 2),
			(int)(-cosf(theta) * sinf(phi) * myCubes[iS].x + (-sinf(theta) * sinf(phi)) * (myCubes[iS].y + myCubes[iS].size) + cosf(phi) * myCubes[iS].z) + (HEIGHT / 2)
			,BLACK
		);
	}

	//
	//if ((	(int)(-sinf(theta) * myCubes[iS].x + cosf(theta) * myCubes[iS].y) + (WIDTH / 2) 
	//	>=	(int)(-sinf(theta) * (myCubes[iS].x + myCubes[iS].size) + cosf(theta) * myCubes[iS].y) + (WIDTH / 2)
	//	&&	(int)(-cosf(theta) * sinf(phi) * myCubes[iS].x + (-sinf(theta) * sinf(phi)) * (myCubes[iS].y + myCubes[iS].size) + cosf(phi) * myCubes[iS].z) + (HEIGHT / 2) 
	//	<=	(int)(-cosf(theta) * sinf(phi) * myCubes[iS].x + (-sinf(theta) * sinf(phi)) * myCubes[iS].y + cosf(phi) * myCubes[iS].z) + (HEIGHT / 2)) 
	//	|| ((int)(-sinf(theta) * myCubes[iS].x + cosf(theta) * myCubes[iS].y) + (WIDTH / 2) 
	//	<=	(int)(-sinf(theta) * (myCubes[iS].x + myCubes[iS].size) + cosf(theta) * myCubes[iS].y) + (WIDTH / 2)
	//	&&	(int)(-cosf(theta) * sinf(phi) * myCubes[iS].x + (-sinf(theta) * sinf(phi)) * (myCubes[iS].y + myCubes[iS].size) + cosf(phi) * myCubes[iS].z) + (HEIGHT / 2) 
	//	>=	(int)(-cosf(theta) * sinf(phi) * myCubes[iS].x + (-sinf(theta) * sinf(phi)) * myCubes[iS].y + cosf(phi) * myCubes[iS].z) + (HEIGHT / 2))
	//	) {
	//	arduboy.fillTriangle(
	//		(int)(-sinf(theta) * myCubes[iS].x + cosf(theta) * myCubes[iS].y) + (WIDTH / 2),
	//		(int)(-cosf(theta) * sinf(phi) * myCubes[iS].x + (-sinf(theta) * sinf(phi)) * myCubes[iS].y + cosf(phi) * myCubes[iS].z) + (HEIGHT / 2),
	//		(int)(-sinf(theta) * myCubes[iS].x + cosf(theta) * (myCubes[iS].y + myCubes[iS].size)) + (WIDTH / 2),
	//		(int)(-cosf(theta) * sinf(phi) * myCubes[iS].x + (-sinf(theta) * sinf(phi)) * (myCubes[iS].y + myCubes[iS].size) + cosf(phi) * myCubes[iS].z) + (HEIGHT / 2),
	//		(int)(-sinf(theta) * (myCubes[iS].x + myCubes[iS].size) + cosf(theta) * myCubes[iS].y) + (WIDTH / 2),
	//		(int)(-cosf(theta) * sinf(phi) * (myCubes[iS].x + myCubes[iS].size) + (-sinf(theta) * sinf(phi)) * myCubes[iS].y + cosf(phi) * myCubes[iS].z) + (HEIGHT / 2)
	//	);
	//}
	//0:0:0
	arduboy.drawPixel(myCubes[iS].p1.x, myCubes[iS].p1.y);
	ardTiny.setCursor(myCubes[iS].p1.x, myCubes[iS].p1.y);
	ardTiny.print("1");
	//0:1:0
	arduboy.drawPixel(myCubes[iS].p2.x, myCubes[iS].p2.y);
	ardTiny.setCursor(myCubes[iS].p2.x, myCubes[iS].p2.y);
	ardTiny.print("2");
	/*arduboy.drawPixel(
		(int)(-sinf(theta) * myCubes[iS].x + cosf(theta) * (myCubes[iS].y + myCubes[iS].size)) + (WIDTH / 2),
		(int)(-cosf(theta) * sinf(phi) * myCubes[iS].x + (-sinf(theta) * sinf(phi)) * (myCubes[iS].y + myCubes[iS].size) + cosf(phi) * myCubes[iS].z) + (HEIGHT / 2)
	);*/
	//1:0:0
	arduboy.drawPixel(myCubes[iS].p3.x, myCubes[iS].p3.y);
	ardTiny.setCursor(myCubes[iS].p3.x, myCubes[iS].p3.y);
	ardTiny.print("3");
	/*arduboy.drawPixel(
		(int)(-sinf(theta) * (myCubes[iS].x + myCubes[iS].size) + cosf(theta) * myCubes[iS].y) + (WIDTH / 2),
		(int)(-cosf(theta) * sinf(phi) * (myCubes[iS].x + myCubes[iS].size) + (-sinf(theta) * sinf(phi)) * myCubes[iS].y + cosf(phi) * myCubes[iS].z) + (HEIGHT / 2)
	);*/
	//1:1:0
	arduboy.drawPixel(myCubes[iS].p4.x, myCubes[iS].p4.y);
	ardTiny.setCursor(myCubes[iS].p4.x, myCubes[iS].p4.y);
	ardTiny.print("4");
	/*arduboy.drawPixel(
		(int)(-sinf(theta) * (myCubes[iS].x + myCubes[iS].size) + cosf(theta) * (myCubes[iS].y + myCubes[iS].size)) + (WIDTH / 2),
		(int)(-cosf(theta) * sinf(phi) * (myCubes[iS].x + myCubes[iS].size) + (-sinf(theta) * sinf(phi)) * (myCubes[iS].y + myCubes[iS].size) + cosf(phi) * myCubes[iS].z) + (HEIGHT / 2)
	);*/
	//0:0:1
	arduboy.drawPixel(myCubes[iS].p5.x, myCubes[iS].p5.y);
	ardTiny.setCursor(myCubes[iS].p5.x, myCubes[iS].p5.y);
	ardTiny.print("5");
	/*arduboy.drawPixel(
		(int)(-sinf(theta) * myCubes[iS].x + cosf(theta) * myCubes[iS].y) + (WIDTH / 2),
		(int)(-cosf(theta) * sinf(phi) * myCubes[iS].x + (-sinf(theta) * sinf(phi)) * myCubes[iS].y + cosf(phi) * (myCubes[iS].z + myCubes[iS].size)) + (HEIGHT / 2)
	);*/
	//0:1:1
	arduboy.drawPixel(myCubes[iS].p6.x, myCubes[iS].p6.y);
	ardTiny.setCursor(myCubes[iS].p6.x, myCubes[iS].p6.y);
	ardTiny.print("6");
	/*arduboy.drawPixel(
		(int)(-sinf(theta) * myCubes[iS].x + cosf(theta) * (myCubes[iS].y + myCubes[iS].size)) + (WIDTH / 2),
		(int)(-cosf(theta) * sinf(phi) * myCubes[iS].x + (-sinf(theta) * sinf(phi)) * (myCubes[iS].y + myCubes[iS].size) + cosf(phi) * (myCubes[iS].z + myCubes[iS].size)) + (HEIGHT / 2)
	);*/
	//1:0:1
	arduboy.drawPixel(myCubes[iS].p7.x, myCubes[iS].p7.y);
	ardTiny.setCursor(myCubes[iS].p7.x, myCubes[iS].p7.y);
	ardTiny.print("7");
	/*arduboy.drawPixel(
		(int)(-sinf(theta) * (myCubes[iS].x + myCubes[iS].size) + cosf(theta) * myCubes[iS].y) + (WIDTH / 2),
		(int)(-cosf(theta) * sinf(phi) * (myCubes[iS].x + myCubes[iS].size) + (-sinf(theta) * sinf(phi)) * myCubes[iS].y + cosf(phi) * (myCubes[iS].z + myCubes[iS].size)) + (HEIGHT / 2)
	);*/
	//1:1:1
	arduboy.drawPixel(myCubes[iS].p8.x, myCubes[iS].p8.y);
	ardTiny.setCursor(myCubes[iS].p8.x, myCubes[iS].p8.y);
	ardTiny.print("8");
	/*arduboy.drawPixel(
		(int)(-sinf(theta) * (myCubes[iS].x + myCubes[iS].size) + cosf(theta) * (myCubes[iS].y + myCubes[iS].size)) + (WIDTH / 2),
		(int)(-cosf(theta) * sinf(phi) * (myCubes[iS].x + myCubes[iS].size) + (-sinf(theta) * sinf(phi)) * (myCubes[iS].y + myCubes[iS].size) + cosf(phi) * (myCubes[iS].z + myCubes[iS].size)) + (HEIGHT / 2)
	);*/
}
