#pragma once
#include "graphics/Shader.h"
#include "graphics/Parts/ConstantBuffer.h"

class DebugWireframe : public btIDebugDraw {
    struct Vertex{
        CVector3 pos;
        CVector3 col;
    };

	ConstantBuffer m_cBuf;//定数バッファ
	ID3D11Buffer* m_vBuf;//頂点バッファ
	Shader m_pShader;//ピクセルシェーダー
	Shader m_vShader;//頂点シェーダー
	ID3D11DepthStencilState* m_depthState;//深度ステンシルステート
	int m_debug = 0;

    static constexpr UINT maxLine = 3000; //一度にDrawする最大の線数
    UINT stockLineCount = 0;  //今Draw待ちの線数
    Vertex stockVertex[maxLine*2];

public:
	DebugWireframe();
	~DebugWireframe();

	//必須
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

	void setDebugMode(int debugMode) override {
		m_debug = debugMode;
	}
	int	getDebugMode() const override {
		return m_debug;
	}

	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)override {};

	void reportErrorWarning(const char* warningString)override {};

	void draw3dText(const btVector3& location, const char* textString)override {};

	void DrawBegin();

    void DrawEnd();
};