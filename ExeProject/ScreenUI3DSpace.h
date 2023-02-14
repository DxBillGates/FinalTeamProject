#pragma once
#include <GatesEngine/Header/Util/Math/Vector3.h>
#include <GatesEngine/Header/Util/Math/Vector2.h>
#include <GatesEngine/Header/Util/Math/Matrix4.h>
#include <GatesEngine/Header/Graphics/IGraphicsDeviceDx12.h>

// 3d空間上のものをスクリーン座標系に変換して表示する基底クラスクラス
class ScreenUI3DSpace
{
protected:
	GE::Math::Vector3 worldPos;
	GE::Math::Vector3 scale;
	GE::Math::Vector3 calclatedScreenPos;
	GE::Color color;
	static GE::IGraphicsDeviceDx12* graphicsDevice;
private:
	void FixedCalclatedScreenPos();
protected:
	/// <summary>
	/// worldPosをスクリーン座標系に変換した結果をcalclatedScreenPosに代入する
	/// </summary>
	void CalclateScreenPos();
#pragma region 描画関係
	virtual void SetShader();
	virtual void SetShaderResource();
	virtual void AttachScreenPos();
	virtual void AttachCamera();
	virtual void AttachMaterial();
	virtual void DrawPlane();
#pragma endregion
public:
	ScreenUI3DSpace();
	virtual ~ScreenUI3DSpace();
	static void SetGraphicsDevice(GE::IGraphicsDeviceDx12* pGraphicsDevice);
	virtual void Start();
	/// <summary>
	/// CalclateScreenPosだけ実行する
	/// </summary>
	virtual void Update(float deltaTime);
	/// <summary>
	/// 描画関係の仮想関数をすべて実行
	/// </summary>
	virtual void Draw();

	void SetWorldPosition(const GE::Math::Vector3& setPos);
	void SetScale(const GE::Math::Vector3& setScale);
	void SetColor(const GE::Color& setColor);
};
