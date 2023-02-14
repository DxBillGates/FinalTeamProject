#pragma once
#include <GatesEngine/Header/Util/Math/Vector3.h>
#include <GatesEngine/Header/Util/Math/Vector2.h>
#include <GatesEngine/Header/Util/Math/Matrix4.h>
#include <GatesEngine/Header/Graphics/IGraphicsDeviceDx12.h>

// 3d��ԏ�̂��̂��X�N���[�����W�n�ɕϊ����ĕ\��������N���X�N���X
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
	/// worldPos���X�N���[�����W�n�ɕϊ��������ʂ�calclatedScreenPos�ɑ������
	/// </summary>
	void CalclateScreenPos();
#pragma region �`��֌W
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
	/// CalclateScreenPos�������s����
	/// </summary>
	virtual void Update(float deltaTime);
	/// <summary>
	/// �`��֌W�̉��z�֐������ׂĎ��s
	/// </summary>
	virtual void Draw();

	void SetWorldPosition(const GE::Math::Vector3& setPos);
	void SetScale(const GE::Math::Vector3& setScale);
	void SetColor(const GE::Color& setColor);
};
