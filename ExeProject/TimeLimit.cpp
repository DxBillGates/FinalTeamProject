#include "TimeLimit.h"

TimeLimit::TimeLimit(const int& timer)
{
	this->timer = timer * 60 * fps;//������b���ɒ�����144fps������
}

void TimeLimit::Start()
{
}

void TimeLimit::Update()
{
	timer--;//�J�E���g�_�E��
	minutes = timer / fps / 60;//�����̌v�Z
	tenSeconds = timer / fps % 60 / 10;//�b���̏\�̈ʂ̌v�Z
	oneSeconds = timer / fps % 60 % 10;//�b���̈�̈ʂ̌v�Z
}

void TimeLimit::Draw()
{
	//// �`�悷�邽�߂�GraphicsDevice���擾���Ƃ��悤�ɁI
	
	//// �����_�[�L���[��2d�p�ɐ؂�ւ���
	//graphicsDevice.SetCurrentRenderQueue(false);

	//// �e�N�X�`���`��p�̃V�F�[�_�[���Z�b�g
	//graphicsDevice.SetShader("DefaultSpriteWithTextureShader");
	
	//// �`��ʒu�Ƃ��T�C�Y�Ƃ��̐ݒ�
	//auto windowSize = GE::Window::GetWindowSize();
	//GE::Math::Matrix4x4 modelMatrix = GE::Math::Matrix4x4::Scale({ windowSize.x,windowSize.y,0 });
	//modelMatrix *= GE::Math::Matrix4x4::Translate({ 0,0,0 });
	
	//// �摜�̐F�ς����肷��悤
	//GE::Material material;
	//material.color = GE::Color::White();

	//// 2d�p�̃J������� ��{�I�ɔ����Ȃ���ok
	//GE::CameraInfo cameraInfo;
	//cameraInfo.viewMatrix = GE::Math::Matrix4x4::GetViewMatrixLookTo({ 0,0,0 }, { 0,0,1 }, { 0,1,0 });
	//cameraInfo.projMatrix = GE::Math::Matrix4x4::GetOrthographMatrix(GE::Window::GetWindowSize());

	//// �A�j���[�V�����̏��
	//GE::TextureAnimationInfo textureAnimationInfo;
	//GE::Texture* texture = graphicsDevice.GetTextureManager()->Get("�e�N�X�`����");
	//// �摜�̌��T�C�Y
	//textureAnimationInfo.textureSize = texture->GetSize();
	//// ���摜�̃T�C�Y����ǂ�����Đ؂蔲�����@��) ���T�C�Y��100*100�Ŕ��������\���������Ȃ�{50,100}�ɂ���
	//// textureSize�ƈꏏ�ɂ���Ɛ؂蔲���ꂸ�ɕ`�悳���
	//textureAnimationInfo.clipSize = texture->GetSize();
	//// �؂蔲���ۂ̍�����W ��) {0,0}�Ȃ猳�摜�̍��� texture->GetSize()�Ȃ�E���ɂȂ�
	//textureAnimationInfo.pivot = {0,0};

	//renderQueue->AddSetConstantBufferInfo({ 0,cbufferAllocater->BindAndAttachData(0, &modelMatrix, sizeof(GE::Math::Matrix4x4)) });
	//renderQueue->AddSetConstantBufferInfo({ 1,cbufferAllocater->BindAndAttachData(1, &cameraInfo, sizeof(GE::CameraInfo)) });
	//renderQueue->AddSetConstantBufferInfo({ 2,cbufferAllocater->BindAndAttachData(2, &material,sizeof(GE::Material)) });
	//renderQueue->AddSetConstantBufferInfo({ 4,cbufferAllocater->BindAndAttachData(4, &textureAnimationInfo,sizeof(GE::TextureAnimationInfo)) });
	//renderQueue->AddSetShaderResource({ 5,texture.GetSRVNumber() });
	//graphicsDevice.DrawMesh("2DPlane");
}