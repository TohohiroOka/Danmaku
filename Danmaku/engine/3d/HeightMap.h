#pragma once
#include "InterfaceObject3d.h"
#include "TerrainModel.h"

class BaseCollider;
class Camera;
class LightGroup;

class HeightMap : public InterfaceObject3d
{
private: // �G�C���A�X

	struct OBJECT_INFO
	{
		float ratio;
	};

public://�����o�֐�

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="_model">���f��</param>
	/// <returns>�C���X�^���X</returns>
	static std::unique_ptr<HeightMap> Create(TerrainModel* _model);

	/// <summary>
	/// �`��O����
	/// </summary>
	static void PreDraw();

private://�����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// ���f���Z�b�g
	/// </summary>
	/// <param name="_model">���f��</param>
	void SetModel(TerrainModel* _model) {
		model = _model;
	}

public:

	HeightMap() {};
	~HeightMap() {};

	/// <summary>
	/// �ǉ��̒萔�o�b�t�@�̍X�V
	/// </summary>
	void AddConstBufferUpdate(const float _ratio = 1.0f);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �p�C�v���C���̃Z�b�g
	/// </summary>
	/// <param name="_pipeline">�p�C�v���C��</param>
	static void SetPipeline(const GraphicsPipelineManager::GRAPHICS_PIPELINE& _pipeline) { HeightMap::pipeline = _pipeline; }
	
	/// <summary>
	/// ���f���̕ϊ�
	/// </summary>
	/// <param name="_beforeModel">�ύX�O</param>
	/// <param name="afterModel">�ύX��</param>
	/// <param name="_ratio">�ϊ�����</param>
	void SetChangeDrawModel(TerrainModel* _beforeModel, TerrainModel* afterModel, const float _ratio = 1.0f);

	/// <summary>
	/// ���f���̕ϊ�
	/// </summary>
	/// <param name="_beforeModel">�ύX�O</param>
	/// <param name="afterModel">�ύX��</param>
	/// <param name="_ratio">�ϊ�����</param>
	void SetChangeHitModel(TerrainModel* _beforeModel, TerrainModel* afterModel, const float _ratio = 1.0f);

private:

	//�p�C�v���C��
	static GraphicsPipelineManager::GRAPHICS_PIPELINE pipeline;

private:

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBufferOData;
	// ���f��
	TerrainModel* model = nullptr;

public:

	Model* GetModel() { return model->GetModel(); }
	std::vector<Mesh::VERTEX>& GetHitVertices() { return model->GetHitVertices(); }
	std::vector<unsigned long>& GetHitIndices() { return model->GetHitIndices(); }
};