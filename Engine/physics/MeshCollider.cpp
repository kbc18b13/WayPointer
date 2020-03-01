#include "stdafx.h"
#include "Physics/MeshCollider.h"
#include <fstream>


MeshCollider::MeshCollider() 
{
}


MeshCollider::~MeshCollider()
{

}

/*!
 * @brief	CSkinModel���烁�b�V���R���C�_�[�𐶐��B
 *@param[in]	model		�X�L�����f���B
 */
void MeshCollider::CreateFromSkinModel( const SkinModel& model)
{
	CMatrix mBias;
	m_stridingMeshInterface = std::make_unique<btTriangleIndexVertexArray>();

	model.FindMesh([&](const auto& mesh){

			ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
			//���_�o�b�t�@���쐬�B
			{
				D3D11_MAPPED_SUBRESOURCE subresource;
				HRESULT hr = deviceContext->Map(mesh->vertexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
				if (FAILED(hr)) {
					return;
				}
				D3D11_BUFFER_DESC bufferDesc;
				mesh->vertexBuffer->GetDesc(&bufferDesc);
				int vertexCount = bufferDesc.ByteWidth / mesh->vertexStride;
				char* pData = reinterpret_cast<char*>(subresource.pData);
				VertexBufferPtr vertexBuffer = std::make_unique<VertexBuffer>();
				CVector3 pos;
				for (int i = 0; i < vertexCount; i++) {
					pos = *reinterpret_cast<CVector3*>(pData);
					//�o�C�A�X��������B
					mBias.Mul(pos);
					vertexBuffer->push_back(pos);
					//���̒��_�ցB
					pData += mesh->vertexStride;
				}
				//���_�o�b�t�@���A�����b�N
				deviceContext->Unmap(mesh->vertexBuffer.Get(), 0);
				m_vertexBufferArray.push_back(std::move(vertexBuffer));
			}
			//�C���f�b�N�X�o�b�t�@���쐬�B
			{
				D3D11_MAPPED_SUBRESOURCE subresource;
				//�C���f�b�N�X�o�b�t�@�����b�N�B
				HRESULT hr = deviceContext->Map(mesh->indexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
				if (FAILED(hr)) {
					return;
				}
				D3D11_BUFFER_DESC bufferDesc;
				mesh->indexBuffer->GetDesc(&bufferDesc);
				//@todo cmo�t�@�C���̓C���f�b�N�X�o�b�t�@�̃T�C�Y��2byte�Œ�B
				IndexBufferPtr indexBuffer = std::make_unique<IndexBuffer>();
				int stride = 2;
				int indexCount = bufferDesc.ByteWidth / stride;
				unsigned short* pIndex = reinterpret_cast<unsigned short*>(subresource.pData);
				for (int i = 0; i < indexCount; i++) {
					indexBuffer->push_back(pIndex[i]);
				}
				//�C���f�b�N�X�o�b�t�@���A�����b�N�B
				deviceContext->Unmap(mesh->indexBuffer.Get(), 0);
				m_indexBufferArray.push_back(std::move(indexBuffer));
			}

			//�C���f�b�N�X���b�V�����쐬�B
			btIndexedMesh indexedMesh;
			IndexBuffer* ib = m_indexBufferArray.back().get();
			VertexBuffer* vb = m_vertexBufferArray.back().get();
			indexedMesh.m_numTriangles = (int)ib->size() / 3;
			indexedMesh.m_triangleIndexBase = (unsigned char*)(&ib->front());
			indexedMesh.m_triangleIndexStride = 12;
			indexedMesh.m_numVertices = (int)vb->size();
			indexedMesh.m_vertexBase = (unsigned char*)(&vb->front());
			indexedMesh.m_vertexStride = sizeof(CVector3);
			m_stridingMeshInterface->addIndexedMesh(indexedMesh);
		}
	);
	m_meshShape = std::make_unique<btBvhTriangleMeshShape>(m_stridingMeshInterface.get(), true);
}

template<typename T>
T loadT( std::fstream& input ){
	T value;
	input.read( reinterpret_cast<char*>( &value ), sizeof( T ) );
	return value;
}

template<typename T>
inline void loadT( std::fstream& input, T* dist, UINT arrayLength = 1 ){
	input.read( reinterpret_cast<char*>( dist ), sizeof( T ) * (long long)arrayLength );
}

void skipName( std::fstream& input ){
	long long size = loadT<UINT>( input );
	input.ignore( size * sizeof( wchar_t ) );
}

struct Material3DData{
	DirectX::XMFLOAT4   Ambient;
	DirectX::XMFLOAT4   Diffuse;
	DirectX::XMFLOAT4   Specular;
	float               SpecularPower;
	DirectX::XMFLOAT4   Emissive;
	DirectX::XMFLOAT4X4 UVTransform;
};

void MeshCollider::CreateFromCMOFile( const wchar_t * filePath ){
	m_stridingMeshInterface = std::make_unique<btTriangleIndexVertexArray>();

	//�t�@�C�����I�[�v��
	std::fstream input( filePath, std::ios_base::in | std::ios_base::binary );

	//���b�V�����[�v
	UINT meshCount = loadT<UINT>( input );
	for( UINT iMesh = 0; iMesh < meshCount; iMesh++ ){

		skipName( input );//���b�V����

		//�}�e���A�����[�v
		UINT materialCount = loadT<UINT>( input );
		for( UINT iMate = 0; iMate < materialCount; iMate++ ){

			skipName( input );//�}�e���A����

			input.ignore( sizeof( Material3DData ) );//�}�e���A���f�[�^

			skipName( input );//�s�N�Z���V�F�[�_���B

			//�e�N�X�`���[��8��
			for( int iTex = 0; iTex < 8; iTex++ ){
				skipName( input );
			}
		}

		//���ꂪ�X�L���A�j���[�V�������f���Ȃ�0�ȊO������
		BYTE isAnim = loadT<BYTE>( input );

		//�T�u���b�V��
		struct SubMesh{
			UINT MaterialIndex;
			UINT IndexBufferIndex;
			UINT VertexBufferIndex;
			UINT StartIndex;
			UINT PrimCount;
		};
		UINT submeshCount = loadT<UINT>( input );
		std::unique_ptr<SubMesh[]> submeshArray( new SubMesh[submeshCount] );
		loadT<SubMesh>( input, submeshArray.get(), submeshCount );

		//�C���f�b�N�X�o�b�t�@
		UINT ibCount = loadT<UINT>( input );
		for( UINT iIb = 0; iIb < ibCount; iIb++ ){
			UINT indexCount = loadT<UINT>( input );
			IndexBufferPtr indexBuffer = std::make_unique<IndexBuffer>();
			for( int i = 0; i < indexCount; i++ ){
				indexBuffer->push_back( loadT<USHORT>( input ) );
			}
			m_indexBufferArray.push_back( std::move( indexBuffer ) );
		}

		//���_�o�b�t�@
		UINT vbCount = loadT<UINT>( input );
		{
			struct Vertex{
				DirectX::XMFLOAT3 Position;
				DirectX::XMFLOAT3 Normal;
				DirectX::XMFLOAT4 Tangent;
				UINT color;
				DirectX::XMFLOAT2 TextureCoordinates;
			};
			for( UINT iVb = 0; iVb < vbCount; iVb++ ){
				UINT vertexCount = loadT<UINT>( input );
				VertexBufferPtr vertexBuffer = std::make_unique<VertexBuffer>();
				for( int iv = 0; iv < vertexCount; iv++ ){
					DirectX::XMFLOAT3 pos = loadT<Vertex>( input ).Position;
					vertexBuffer->push_back(CVector3( pos.x, pos.y, pos.z ));
				}
				m_vertexBufferArray.push_back( std::move( vertexBuffer ) );
			}
		}

		//�X�L�����_�B�����ł̓X�L�b�v���邾���B
		{
			struct SkinningVertex{
				UINT boneIndex[4];
				float boneWeight[4];
			};
			UINT skbCount = loadT<UINT>( input );
			for( UINT iSkb = 0; iSkb < skbCount; iSkb++ ){
				UINT skCount = loadT<UINT>( input );
				input.ignore( sizeof( SkinningVertex )* (long long)skCount );//�T�C�Y���X�L�b�v�B
			}
		}

		//���b�V���G�N�X�e���c(�g��Ȃ�)
		{
			struct MeshExtents{
				float CenterX, CenterY, CenterZ;
				float Radius;

				float MinX, MinY, MinZ;
				float MaxX, MaxY, MaxZ;
			};
			input.ignore( sizeof( MeshExtents ) );//�T�C�Y���X�L�b�v�B
		}

		//�{�[���ƃA�j���[�V�������X�L�b�v�B�m���X�L�����f���ł͗p�͖����̂ŁB
		if( isAnim ){
			struct Bone{
				UINT ParentIndex;
				DirectX::XMFLOAT4X4 InvBindPos;
				DirectX::XMFLOAT4X4 BindPos;
				DirectX::XMFLOAT4X4 LocalTransform;
			};
			UINT boneCount = loadT<UINT>( input );
			for( UINT i = 0; i < boneCount; i++ ){
				skipName( input );
				input.ignore( sizeof( Bone ) );//�{�[�����X�L�b�v
			}

			struct Keyframe{
				UINT BoneIndex;
				float Time;
				DirectX::XMFLOAT4X4 Transform;
			};
			UINT clipCount = loadT<UINT>( input );
			for( UINT i = 0; i < clipCount; i++ ){
				skipName( input );
				input.ignore( sizeof( float ) * 2 );//�X�^�[�g�ƃG���h�̃^�C�������X�L�b�v

				UINT keyCount = loadT<UINT>( input );
				for( UINT k = 0; k < keyCount; k++ ){
					input.ignore( sizeof( Keyframe ) );//�L�[�t���[�����X�L�b�v
				}
			}
		}

		//�ǂݍ��ݏI���

		for( int iSub = 0; iSub < submeshCount; iSub++ ){
			//�C���f�b�N�X���b�V�����쐬�B
			btIndexedMesh indexedMesh;
			IndexBuffer* ib = m_indexBufferArray[iSub].get();
			VertexBuffer* vb = m_vertexBufferArray[iSub].get();
			indexedMesh.m_numTriangles = (int)ib->size() / 3;
			indexedMesh.m_triangleIndexBase = (unsigned char*)( &ib->front() );
			indexedMesh.m_triangleIndexStride = 12;
			indexedMesh.m_numVertices = (int)vb->size();
			indexedMesh.m_vertexBase = (unsigned char*)( &vb->front() );
			indexedMesh.m_vertexStride = sizeof( CVector3 );
			m_stridingMeshInterface->addIndexedMesh( indexedMesh );
		}
	}

	m_meshShape = std::make_unique<btBvhTriangleMeshShape>( m_stridingMeshInterface.get(), true );
}
