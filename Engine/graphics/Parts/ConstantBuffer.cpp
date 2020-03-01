#include "stdafx.h"
#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(){}


ConstantBuffer::~ConstantBuffer(){
	m_buffer->Release();
}

void ConstantBuffer::Init(size_t size, bool writeCPU, void* initDataP) {
	m_isWriteCPU = writeCPU;

	D3D11_BUFFER_DESC desc{};
	desc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = size;
	desc.CPUAccessFlags = writeCPU ? D3D11_CPU_ACCESS_WRITE : 0;
	desc.Usage = writeCPU ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;

	HRESULT result;

	if (initDataP != nullptr) {
		D3D11_SUBRESOURCE_DATA data{};
		data.pSysMem = initDataP;
		result = g_graphicsEngine->GetD3DDevice()->CreateBuffer(&desc, &data, &m_buffer);
	} else {
		result = g_graphicsEngine->GetD3DDevice()->CreateBuffer(&desc, nullptr, &m_buffer);
	}
	if (FAILED(result))abort();
}

void ConstantBuffer::UpdateData(void * dataP) {
	ID3D11DeviceContext* dc = g_graphicsEngine->GetD3DDeviceContext();

	if (m_isWriteCPU) {
		//CPUアクセス可で作ったならMapを使う
		D3D11_MAPPED_SUBRESOURCE mapped{};
		dc->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		mapped.pData = dataP;
		dc->Unmap(m_buffer, 0);
	} else {
		//CPUアクセス不可で作ったならUpdateSubresourceを使う
		dc->UpdateSubresource(m_buffer, 0, nullptr, dataP, 0, 0);
	}
}

void ConstantBuffer::SetToContext( ShaderType type ,int registerNum) {
	switch (type) {
	case ShaderType::VS:
		g_graphicsEngine->GetD3DDeviceContext()->VSSetConstantBuffers(registerNum, 1, &m_buffer);
		break;
	case ShaderType::PS:
		g_graphicsEngine->GetD3DDeviceContext()->PSSetConstantBuffers(registerNum, 1, &m_buffer);
		break;
	case ShaderType::CS:
		g_graphicsEngine->GetD3DDeviceContext()->CSSetConstantBuffers(registerNum, 1, &m_buffer);
		break;
	}
}
