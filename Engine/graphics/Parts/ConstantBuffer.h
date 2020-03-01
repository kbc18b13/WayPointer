#pragma once

enum class ShaderType{
	VS,
	PS,
	CS,
};

class ConstantBuffer{
public:
    ConstantBuffer();
    ~ConstantBuffer();

	void Init(size_t size, bool writeCPU, void* initDataP = nullptr);

	void UpdateData(void* dataP);

	void SetToContext( ShaderType type ,int registerNum);

private:
	ID3D11Buffer* m_buffer = nullptr;
	bool m_isWriteCPU = false;
};

