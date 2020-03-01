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
 * @brief	CSkinModelからメッシュコライダーを生成。
 *@param[in]	model		スキンモデル。
 */
void MeshCollider::CreateFromSkinModel( const SkinModel& model)
{
	CMatrix mBias;
	m_stridingMeshInterface = std::make_unique<btTriangleIndexVertexArray>();

	model.FindMesh([&](const auto& mesh){

			ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
			//頂点バッファを作成。
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
					//バイアスをかける。
					mBias.Mul(pos);
					vertexBuffer->push_back(pos);
					//次の頂点へ。
					pData += mesh->vertexStride;
				}
				//頂点バッファをアンロック
				deviceContext->Unmap(mesh->vertexBuffer.Get(), 0);
				m_vertexBufferArray.push_back(std::move(vertexBuffer));
			}
			//インデックスバッファを作成。
			{
				D3D11_MAPPED_SUBRESOURCE subresource;
				//インデックスバッファをロック。
				HRESULT hr = deviceContext->Map(mesh->indexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
				if (FAILED(hr)) {
					return;
				}
				D3D11_BUFFER_DESC bufferDesc;
				mesh->indexBuffer->GetDesc(&bufferDesc);
				//@todo cmoファイルはインデックスバッファのサイズは2byte固定。
				IndexBufferPtr indexBuffer = std::make_unique<IndexBuffer>();
				int stride = 2;
				int indexCount = bufferDesc.ByteWidth / stride;
				unsigned short* pIndex = reinterpret_cast<unsigned short*>(subresource.pData);
				for (int i = 0; i < indexCount; i++) {
					indexBuffer->push_back(pIndex[i]);
				}
				//インデックスバッファをアンロック。
				deviceContext->Unmap(mesh->indexBuffer.Get(), 0);
				m_indexBufferArray.push_back(std::move(indexBuffer));
			}

			//インデックスメッシュを作成。
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

	//ファイルをオープン
	std::fstream input( filePath, std::ios_base::in | std::ios_base::binary );

	//メッシュループ
	UINT meshCount = loadT<UINT>( input );
	for( UINT iMesh = 0; iMesh < meshCount; iMesh++ ){

		skipName( input );//メッシュ名

		//マテリアルループ
		UINT materialCount = loadT<UINT>( input );
		for( UINT iMate = 0; iMate < materialCount; iMate++ ){

			skipName( input );//マテリアル名

			input.ignore( sizeof( Material3DData ) );//マテリアルデータ

			skipName( input );//ピクセルシェーダ名。

			//テクスチャーを8つ
			for( int iTex = 0; iTex < 8; iTex++ ){
				skipName( input );
			}
		}

		//これがスキンアニメーションモデルなら0以外が入る
		BYTE isAnim = loadT<BYTE>( input );

		//サブメッシュ
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

		//インデックスバッファ
		UINT ibCount = loadT<UINT>( input );
		for( UINT iIb = 0; iIb < ibCount; iIb++ ){
			UINT indexCount = loadT<UINT>( input );
			IndexBufferPtr indexBuffer = std::make_unique<IndexBuffer>();
			for( int i = 0; i < indexCount; i++ ){
				indexBuffer->push_back( loadT<USHORT>( input ) );
			}
			m_indexBufferArray.push_back( std::move( indexBuffer ) );
		}

		//頂点バッファ
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

		//スキン頂点。ここではスキップするだけ。
		{
			struct SkinningVertex{
				UINT boneIndex[4];
				float boneWeight[4];
			};
			UINT skbCount = loadT<UINT>( input );
			for( UINT iSkb = 0; iSkb < skbCount; iSkb++ ){
				UINT skCount = loadT<UINT>( input );
				input.ignore( sizeof( SkinningVertex )* (long long)skCount );//サイズ分スキップ。
			}
		}

		//メッシュエクステンツ(使わない)
		{
			struct MeshExtents{
				float CenterX, CenterY, CenterZ;
				float Radius;

				float MinX, MinY, MinZ;
				float MaxX, MaxY, MaxZ;
			};
			input.ignore( sizeof( MeshExtents ) );//サイズ分スキップ。
		}

		//ボーンとアニメーションをスキップ。ノンスキンモデルでは用は無いので。
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
				input.ignore( sizeof( Bone ) );//ボーンをスキップ
			}

			struct Keyframe{
				UINT BoneIndex;
				float Time;
				DirectX::XMFLOAT4X4 Transform;
			};
			UINT clipCount = loadT<UINT>( input );
			for( UINT i = 0; i < clipCount; i++ ){
				skipName( input );
				input.ignore( sizeof( float ) * 2 );//スタートとエンドのタイム情報をスキップ

				UINT keyCount = loadT<UINT>( input );
				for( UINT k = 0; k < keyCount; k++ ){
					input.ignore( sizeof( Keyframe ) );//キーフレームをスキップ
				}
			}
		}

		//読み込み終わり

		for( int iSub = 0; iSub < submeshCount; iSub++ ){
			//インデックスメッシュを作成。
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
