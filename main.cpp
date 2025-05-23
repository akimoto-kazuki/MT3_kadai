#include <Novice.h>
#include <cstdint>
#define _USE_MATH_DEFINES
#include <cmath>

const char kWindowTitle[] = "LC1C_02_アキモト_カズキ";

struct Vector3
{
	float x;
	float y;
	float z;
};

struct Matrix4x4
{
	float m[4][4];
};

struct Sphere
{
	Vector3 center;
	float radius;
};

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	const float kGridHalfWidth = 2.0f;										// Gridの半分の幅
	const uint32_t kSubdivision = 10;										// 分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision); // 1つ分の長さ
	// 奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex < kSubdivision; ++xIndex)
	{
		// 上の情報を使ってワールド座標系上の始点を終点を求める
		


		// スクリーン座標系まで変換をかける
		


		// 変換した座標を使って表示。色の薄い灰色(0xAAAAAAFF)、原点は黒で良いが、何でも良い
		Novice::DrawLine();
	}
	// 左から右も同じように順々に引いていく
	for (uint32_t zIndex = 0; zIndex < kSubdivision; ++zIndex)
	{
		// 奥から手前が左右に変わるだけ
	}

}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	const uint32_t kSubdivision = 10;						// 分割数
	const float kLonEvery = float(2*M_PI) / kSubdivision;	// 経度分割１つの角度
	const float kLatEvery = float(M_PI) / kSubdivision;		// 緯度分割１つの角度
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
