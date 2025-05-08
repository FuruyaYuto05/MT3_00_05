#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "学籍番号";

// ---------- 構造体定義 ----------
struct Vector3 {
	float x;
	float y;
	float z;
};

struct Matrix4x4 {
	float m[4][4];
};

// ---------- 行列作成関数 ----------
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	return {
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1
	};
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result{};
	result.m[0][0] = 1.0f;
	result.m[1][1] = cosf(radian);
	result.m[1][2] = sinf(radian);
	result.m[2][1] = -sinf(radian);
	result.m[2][2] = cosf(radian);
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result{};
	result.m[0][0] = cosf(radian);
	result.m[0][2] = -sinf(radian);
	result.m[1][1] = 1.0f;
	result.m[2][0] = sinf(radian);
	result.m[2][2] = cosf(radian);
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result{};
	result.m[0][0] = cosf(radian);
	result.m[0][1] = sinf(radian);
	result.m[1][0] = -sinf(radian);
	result.m[1][1] = cosf(radian);
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	return {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translate.x, translate.y, translate.z, 1
	};
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result{};
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			result.m[row][col] =
				m1.m[row][0] * m2.m[0][col] +
				m1.m[row][1] * m2.m[1][col] +
				m1.m[row][2] * m2.m[2][col] +
				m1.m[row][3] * m2.m[3][col];
		}
	}
	return result;
}

// アフィン変換行列の作成
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 matScale = MakeScaleMatrix(scale);
	Matrix4x4 matRotX = MakeRotateXMatrix(rotate.x);
	Matrix4x4 matRotY = MakeRotateYMatrix(rotate.y);
	Matrix4x4 matRotZ = MakeRotateZMatrix(rotate.z);

	
	Matrix4x4 matRot = Multiply(Multiply(matRotZ, matRotX), matRotY);
	Matrix4x4 matTrans = MakeTranslateMatrix(translate);


	Matrix4x4 matWorld = Multiply(Multiply(matScale, matRot), matTrans);
	return matWorld;
}

// ---------- 描画用ユーティリティ ----------
static const int kColumnWidth = 60;
static const int kRowHeight = 20;

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; column++) {
			Novice::ScreenPrintf(
				x + column * kColumnWidth,
				y + (row + 1) * kRowHeight,
				"%6.02f", matrix.m[row][column]
			);
		}
	}
}

// ---------- エントリーポイント ----------
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Novice::Initialize(kWindowTitle, 1280, 720);

	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		Vector3 scale{ 1.2f, 0.79f, -2.1f };
		Vector3 rotate{ 0.4f, 1.43f, -0.8f };
		Vector3 translate{ 2.7f, -4.15f, 1.57f };
		Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotate, translate);

		MatrixScreenPrintf(0, 0, worldMatrix, "worldMatrix");

		Novice::EndFrame();
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
